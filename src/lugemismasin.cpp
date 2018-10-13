#include "lugemismasin.h"

#define EOT 0x04
#define ENQ 0x05 	// Anfrage
#define ACK 0x06
#define STX 0x02	// Start Text
#define NAK 0x15
#define CR 0x0d

char enq = ENQ;
char ack = ACK;

LugemisMasin::LugemisMasin(QObject *parent) : QObject(parent)
{
    laskudeArv = 0;
    relv = 0;
    oliLiigneLask = false;
    vahemuses = false;
    oliKahtlane = false;
    uhendatud = false;
    kaabelLeitud = false;
    saatmiseEtapp = 2;  //kui on vana masinaga ühendus, siis see ei muutu ja võib teksti saata, kuna: 0 - algseis/ACK tulnud (lõpp), 1 - ENQ saadetud, 2 - STX saabunud, 3 - tekst saadetud, 4 - saabus tekst, vaja vastata ACK
    uusLugemismasin = false;    //Kõigepealt proovime, kas on vana masin
    lugemismasinaValimine = false;  //Algul kohe mingit otsimist ei käi

    serial = new QextSerialPort();
    lugeja = new QTimer(this);
    lugeja->setInterval(100);
    lugeja->setSingleShot(false);
    connect(lugeja, SIGNAL(timeout()), this, SLOT(loe()));
//    otsija = new QTimer(this);
//    otsija->setInterval(97);
//    otsija->setSingleShot(false);
//    connect(otsija, SIGNAL(timeout()), this, SLOT(otsiPorti()));
    saatja = new QTimer(this);  //Viivis enne saatmist, et uus lugemismasin jõuaks reageerida
    saatja->setInterval(20);
    saatja->setSingleShot(true);
    connect(saatja, SIGNAL(timeout()), this, SLOT(saada()));
    seadistaja = new QTimer(this);
    seadistaja->setInterval(1000);
    seadistaja->setSingleShot(true);
    connect(seadistaja, SIGNAL(timeout()), this, SLOT(seadista()));

    uuendaPorte();  //Vaadata kohe üle, kas kaabel on ühendatud
}

int LugemisMasin::CRC(QByteArray* s)
{
    int crc = 0;
    for(int i = 0; i < s->size(); i++)
        crc ^=s->at(i);
    return crc;
}

QString LugemisMasin::getCOMport()
{
    return COMport;
}

int LugemisMasin::getLaskudeArv()
{
    return laskudeArv;
}

int LugemisMasin::getRelv()
{
    return relv;
}

void LugemisMasin::loe()
{
    if(uusLugemismasin){    //Kui on uus masin, siis loetakse teise funktsiooniga
        loe2();
        return;
    }
    static bool esimeneKord = true; //Vajalik, et vana masinat proovitaks kaks korda seadistada. Esimesele seadistamisele ei pruugi see reageerida.

    if(serial->bytesAvailable()>0){
        static QString buffer;
        QString tekst;
        buffer.append(serial->readAll());
        if(buffer.contains(13)){
            tekst = buffer.left(buffer.indexOf(13) + 1);
            buffer.remove(0, buffer.indexOf(13) + 1);
        }else return;

        uhendatud = true;   //Kui saabus tekst, siis järelikult on ühendus loodud?
        lugemismasinaValimine = false;  //Vana masin leitud, rohkem ei ole vaja valida
//        ui->logi->append(tr("vastuvõtt:"));
        emit teadeLogisse(tr("vastuvõtt:"));

//        if(!tekst.contains("START") && !tekst.contains("SCHEIBE") && !tekst.contains("Keine") && tekst.contains(';'))
//            emit teadeLogisse(QString("     %1. lask").arg(lask + 1));
//            ui->logi->append(QString("     %1. lask").arg(lask + 1));
        emit teadeLogisse(tekst);
//        ui->logi->append(tekst);

        if(!tekst.contains("TART") && !tekst.contains("SCHEIBE") && !tekst.contains("Keine") && tekst.contains(';')){
                QStringList list = tekst.split(';', QString::KeepEmptyParts);
                if(list.size() < 5){
                    emit teadeLogisse(tr("See rida oli liiga lühike!"));
//                    ui->logi->append(tr("See rida oli liiga lühike!"));
                    return;
                }
                int x = 0, y = 0;
                float fx = 0, fy = 0;
//                if(lask == 0){
//                    joonistaLeht();
//                }
//                ui->logi->append(QString("list.count() = %1").arg(list.count()));
                emit teadeLogisse(QString("list.count() = %1").arg(list.count()));

                x = list.at(3).toFloat() * 100;
                y = list.at(4).toFloat() * 100;
                if(relv == 0){ //Õhupüssi lehed, koordinaadid on 1/100 ringi ning õhupüssil on üks ring 5mm ehk kokku teeb 0,05mm
                    x *= 5;
                    y *= 5;
                }else{  //Õhupüstoli leht
                    x *= 16;
                    y *= 16;
                }
                fx = x;
                fy = y;
                emit teadeLogisse(QString("x = %1, y = %2\n").arg(x).arg(y));

                loetudLask.setX(fx / 200);
                loetudLask.setY(fy / -200);
                loetudLask.setLask(list.at(1));
                emit laskLoetud(loetudLask);    //Teatab, et lask sai loetud

                if(list.count() > 6){
                    emit teadeLogisse(list.at(8));
                    emit teadeLogisse(list.at(9));
                    x = list.at(8).toFloat() * 100;
                    y = list.at(9).toFloat() * 100;
                    if(relv == 0){ //Õhupüssi lehed
                        x *= 5;
                        y *= 5;
                    }else{  //Õhupüstoli leht
                        x *= 16;
                        y *= 16;
                    }
                    fx = x;
                    fy = y;
                    emit teadeLogisse(QString("x = %1, y = %2\n").arg(x).arg(y));

                    loetudLask.setX(fx / 200);
                    loetudLask.setY(fy / -200);
                    loetudLask.setLask(list.at(6));
                    emit laskLoetud(loetudLask);    //Teatab, et lask sai loetud

                }
                if(list.count() > 12){
                    emit teadeLogisse(list.at(13));
                    emit teadeLogisse(list.at(14));
                    x = list.at(13).toFloat() * 100;
                    y = list.at(14).toFloat() * 100;
                    if(relv == 0){ //Õhupüssi lehed
                        x *= 5;
                        y *= 5;
                    }else{  //Õhupüstoli leht
                        x *= 16;
                        y *= 16;
                    }
                    fx = x;
                    fy = y;
                    emit teadeLogisse(QString("x = %1, y = %2\n").arg(x).arg(y));

                    loetudLask.setX(fx / 200);
                    loetudLask.setY(fy / -200);
                    loetudLask.setLask(list.at(11));
                    emit laskLoetud(loetudLask);    //Teatab, et lask sai loetud

                }
                if(list.contains(";M", Qt::CaseSensitive))
                    oliKahtlane = true;
//                ui->silt->setPixmap(QPixmap::fromImage(*pilt).scaled(ui->silt->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
//                this->update();
//                if(lask == laskudeArv /*10*/){ //Seeria sai täis, vaja kirjutada väärtus märgitud lahtrisse
//                    if(aktiivneSeeria != 0){
//                        fl = seeria;    //Kasutame lasu float muutujat seeria jagamiseks 10'ga, seeria enda jagamisel kaovad komakohad ära
//                        //ui->logi->append(QString("Seeria: %1, fl: %2, fl/10: %3").arg(seeria).arg(fl).arg(fl/10));
//                        aktiivneSeeria->setText(QString("%1").arg(fl / 10));
//                        aktiivneSeeria->setText(aktiivneSeeria->text().replace('.', ','));
//                        for(int i = 0; i < seeriaLasud.count(); i++)    //Loetud laskude kirjutamine aktiivsesse seeriasse
//                            lasud[aktiivseSeeriaNr][i]->set(seeriaLasud[i]);
//                        ui->logi->append(ui->nimeBox->currentText() + QString(" %1. seeria: %2").arg(aktiivneSeeria->objectName().at(6)).arg(fl / 10));
//                    }else{
//                        QMessageBox::critical(this, "Viga", "Puudus aktiivne seeria kast!", QMessageBox::Ok);
//                        ui->logi->append("Viga!:" + ui->nimeBox->currentText() + QString(" tundmatu seeria: %1").arg(fl / 10));
//                    }
//                    /*for(int k = 0; k < seeriad.count(); k++)
//                        if(seeriad[k]->hasFocus()){
//                            fl = seeria;    //Kasutame lasu float muutujat seeria jagamiseks 10'ga, seeria enda jagamisel kaovad komakohad ära
//                            //ui->logi->append(QString("Seeria: %1, fl: %2, fl/10: %3").arg(seeria).arg(fl).arg(fl/10));
//                            seeriad[k]->setText(QString("%1").arg(fl / 10));
//                            seeriad[k]->setText(seeriad[k]->text().replace('.', ','));
//                            ui->logi->append(ui->nimeBox->currentText() + QString(" %1. seeria: %2").arg(k + 1).arg(fl / 10));
//                        }*/
////                    if(!ui->sifriEdit->text().isEmpty() && ui->sifriEdit->text() != ui->sifriLabel->text().left(ui->sifriEdit->text().length())){   //Kui on
////                        QApplication::sendEvent(this, new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier)); //juba uus siffer valmis sisestatud, võib kohe
////                        QApplication::sendEvent(this, new QKeyEvent(QEvent::KeyRelease, Qt::Key_Return, Qt::NoModifier));   //automaatselt seadistada uue sifri
////                    }else{
//                    aktiivneSeeria->setFocus();
//                    QApplication::sendEvent(this, new QKeyEvent(QEvent::KeyPress, Qt::Key_Tab, Qt::NoModifier));
//                    QApplication::sendEvent(this, new QKeyEvent(QEvent::KeyRelease, Qt::Key_Tab, Qt::NoModifier));
//                    fookus->start();
////                    }
//                    seeria = 0;
//                    lask = 0;
//                    seadistaja->start();
//                }
                if(oliKahtlane){ //Kui on vaja kontrollida, viskab teate
//                    QMessageBox::warning(this->parent(), "Teade", tr("Lehes olid kahtlased lasud, vaja kontrollida ning seejärel kaks korda Neustart vajutada. Alles siis vajutage siin \"OK\""), QMessageBox::Ok);
                    oliKahtlane = false;
                }
                return;
        }else{
            if(vahemuses && tekst.contains("SCHEIBE")){  //Seadistab tagasi normaalseks
                /*QString s = "111111111";
                if(ui->leheCombo->currentIndex() < 2)  //Relva tüübi kindlaks tegemine
                    s.replace(0, 1, QString("%1").arg(ui->leheCombo->currentIndex() + 1));
                else s.replace(0, 1, "6");
                s.replace(8, 1, QString("%1").arg(ui->laskudeBox->value()));
                saada(s);*/
                seadistaja->start();
                vahemuses = false;
            }else if(oliLiigneLask && tekst.contains("SCHEIBE")){  //Seadistab järgmiseks vähema laskude arvu peale või tagasi normaalseks
                QString s = "111111111";
                if(relv < 2)  //Relva tüübi kindlaks tegemine
                    s.replace(0, 1, QString("%1").arg(relv + 1));
                else s.replace(0, 1, "6");

                //Kui oli ainult 1 lask lehte, siis järgmine leht on nagunii tühi ja nii võib tagasi normaalseks seadistada
                if(laskudeArv == 1)
                    s.replace(8, 1, QString("%1").arg(laskudeArv));
                //Kui oli 2 lasku lehte, on järgmisse vaja 1
                else{
                    s.replace(8, 1, QString("%1").arg(laskudeArv - 1));
                    vahemuses = true;
                }
//                if(ui->kumnendikegaBox->isChecked())   //Kas lugeda laske kümnendiku täpsusega või mitte - alati loetakse kümnendikega
                s.replace(3, 1, "2");
                saada(s);
                oliLiigneLask = false;
            }
        }
    }else if(lugemismasinaValimine){
        if(!esimeneKord) //Vana masin ei pruugi esimene kord vastata, tasub teist korda veel proovida
            uusLugemismasin = true; //Kui teksti vastu ei tulnud, võib arvata, et tegu on uue lugemismasinaga
        esimeneKord = false;
        uhenda();
    }
}

void LugemisMasin::loe2()
{
    if(serial->bytesAvailable()>0){
        static QString buffer;
        QString tekst;
        //char buffer[256];
        //int rec = serial->readLine(buffer, 255);
        //buffer[rec]='\0';
        buffer.append(serial->readAll());

        if(buffer.contains(CR)){
            tekst = buffer.left(buffer.indexOf(CR) + 1);
            tekst.replace(STX, "");
#ifdef PROOV
            qDebug() << QString("saabus: %1").arg(tekst.replace(CR, "CR"));
#endif
            if(/*saatmiseEtapp == 0 &&*/ tekst.contains("SCH=")){ //Kui ei ole saatmine pooleli ja saabub mingi tekst, siis lugemismasin saadab midagi
                QStringList read = tekst.split(';');    //SCH=1;7.2;2973.0;164.7;G#CR
                if(read.count() >= 4){
                    bool onnestus = false;
                    QLineF koordinaadid = QLineF::fromPolar(read.at(2).left(read.at(2).indexOf('.')).toInt(), read.at(3).toDouble(&onnestus) - 90);   //QLineF'i 0 on kella 3'e suunas, Disag'il otse üles
                    if(!onnestus)   //Kui punktiga ei õnnestu double'iks teha, peab olema koma
                        koordinaadid = QLineF::fromPolar(read.at(2).toInt(), read.value(3).replace(".", ",").toDouble(&onnestus) - 90);
                    if(!onnestus)   //Kui ikka ei õnnestunud, siis on mingi jama
                        emit teadeLogisse(tr("Rida vigane, ei õnnestu koordinaate lugeda!"));
//                        ui->logi->append(tr("Rida vigane, ei õnnestu koordinaate lugeda!"));

//                    seeriaLasud[lask]->setX(koordinaadid.p2().x() / 100);   //Lasu koordinaadid kõigepealt siin lisada
//                    seeriaLasud[lask]->setY(koordinaadid.p2().y() / 100);
//                    seeriaLasud[lask]->setLask(read.at(1));  //Seejärel lasu väärtus
                    loetudLask.setX(koordinaadid.p2().x() / 100);
                    loetudLask.setY(koordinaadid.p2().y() / 100);
                    loetudLask.setLask(read.at(1));
                    emit laskLoetud(loetudLask);    //Teatab, et lask sai loetud
#ifdef PROOV
            qDebug() << "loetudLask:" << loetudLask.getSLask() << ", " << loetudLask.X() << loetudLask.Y();
#endif
//                    joonistaLask(koordinaadid.p2() / 100, true);    //Ühikuks mm
//                    lask++;
//                    if(lask == laskudeArv)  //Kui seeria on täis, on vaja see kokku liita ja ära saata
//                        liidaSeeria();
                }else
                    emit teadeLogisse(tr("Rida liiga l�hike!"));
//                    ui->logi->append(tr("Rida liiga l�hike!"));
                saatmiseEtapp = 4;  //Et saadetaks vastuseks kinnitus
                saada("");  //Saadab ACK'i;
                seadistaja->setInterval(600);   //Siin v�ib kiiremini seadistada
                seadistaja->start();    //Vaja seadistada, et j�rgmist lehte vastu v�etaks
            }
//            saatmiseEtapp = 4;  //Et saadetaks vastuseks kinnitus
//            saada("");  //Saadab ACK'i;

//            bytes = rida.toLatin1();
            buffer.remove(0, buffer.indexOf(CR) + 1);
        }else if(buffer.contains(STX)){
            tekst = buffer.left(buffer.indexOf(STX) + 1);
            tekst.replace(STX, "STX");
//            ui->logi->append("Vastuvõtt: STX");
            if(saatmiseEtapp == 1){
#ifdef PROOV
        qDebug() << "saabus: STX\n";
#endif
                saatmiseEtapp = 2;  //STX saabunud
#ifdef PROOV
        qDebug() << "saatmiseEtapp = 2";
#endif
                saada("");  //Kui vastati STX, siis ilmselt on masin valmis teksti vastuvõtmiseks
//            bytes = rida.toLatin1();
            }
            buffer.remove(0, buffer.indexOf(STX) + 1);
        }else if(buffer.contains(NAK)){
            tekst = buffer.left(buffer.indexOf(NAK) + 1);
            tekst.replace(NAK, "NAK");
            if(saatmiseEtapp == 3){
#ifdef PROOV
        qDebug() << "saabus: NAK\n";
#endif
                saatmiseEtapp = 0;  //NAK saabunud ja midagi tuksis -> algseis
#ifdef PROOV
        qDebug() << "saatmiseEtapp = 0";
#endif
//            bytes = rida.toLatin1();
            }
            buffer.remove(0, buffer.indexOf(NAK) + 1);
        }else if(buffer.contains(ACK)){
            tekst = buffer.left(buffer.indexOf(ACK) + 1);
            tekst.replace(ACK, "ACK");
//            ui->logi->append("Vastuvõtt: ACK");
//            bytes = rida.toLatin1();
            if(saatmiseEtapp == 3){
#ifdef PROOV
        qDebug() << "saabus: ACK";
#endif
                saatmiseEtapp = 0;  //ACK saabunud -> algseis
#ifdef PROOV
        qDebug() << "saatmiseEtapp = 0";
#endif
//                saada("");  //Kui vastati ACK, siis on masin teksti vastu võtnud
//            bytes = rida.toLatin1();
            }
            buffer.remove(0, buffer.indexOf(ACK) + 1);
        }else{
#ifdef PROOV
            qDebug() << "saabus: midagi muud";
#endif
            emit teadeLogisse(tr("Vastuvõtt: %1").arg(buffer));
//            ui->logi->append(tr("Vastuvõtt: %1").arg(buffer));
            return;
        }
        emit teadeLogisse(tr("Vastuvõtt: %1").arg(tekst));
//        ui->logi->append(tr("Vastuvõtt: %1").arg(tekst));
    }
}

//void LugemisMasin::otsiPorti()    //Lisame hiljem
//{
//    //ui->logi->clear();
//    if(uhendatud){
//        otsija->stop();
//        QMessageBox::warning(this, "Protokollitaja", tr("Ühendus juba loodud! Kui on mingi jama ja tahate uuesti ühendada, tuleb enne vajutada \"Sulge ühendus\"."), QMessageBox::Ok);
//        return;
//    }
//    static int i = 0;
//    loe();
//    serial->close();
//    if(ui->logi->toPlainText().split(QRegExp("\\n")).last().contains("H")){
//        otsija->stop();
//        uhenda();
//        i = 0;
//        return;
//    }
//    otsija->stop();
//    if(i >= ui->comPort->count()){
//        i = 0;
//        return;
//    }
//    ui->comPort->setCurrentIndex(i);
//    serial->setPortName(ui->comPort->currentText());
//    serial->setBaudRate(BAUD2400);
//    serial->setDataBits(DATA_8 /*(DataBitsType)(3)*/ );
//    serial->setParity(PAR_NONE /*(ParityType)(0)*/ );
//    serial->setStopBits(STOP_1 /*(StopBitsType)(0)*/ );
//    serial->setFlowControl(FLOW_OFF /*(FlowType)(0)*/ );
//    serial->setTimeout(100);
//    serial->open(QIODevice::ReadWrite);

//    serial->setDtr(true);
//    serial->setRts(true);

//    saada(QString("%1. korda: %2").arg(++i).arg(ui->comPort->currentText()));
//    serial->flush();
//    otsija->start();
//}

void LugemisMasin::saada()
{
    switch (saatmiseEtapp) {    //Olenevalt millises etapis parasjagu ollakse (vana masina puhul on see alati 2)
    case 0:
        serial->write(&enq);
#ifdef PROOV
        qDebug() << "saadan: ENQ";
#endif
        emit teadeLogisse("<font color=blue>ENQ</font>");
//        ui->logi->append("<font color=blue>ENQ</font>");
        serial->flush();
        saatmiseEtapp = 1;  //ENQ saadetud
#ifdef PROOV
        qDebug() << "saatmiseEtapp = 1";
#endif
        return;
        break;
    case 2:
        if(uusLugemismasin)
            saadetis.append(CRC(&saadetis));
        saadetis.append(CR);
        //serial->writeData(saadetis, saadetis.size());
        serial->write(saadetis);
#ifdef PROOV
        qDebug() << QString("saadan: %1").arg(QString(saadetis));
#endif
        emit teadeLogisse("<font color=blue>"+saadetis+"</font>");
//        ui->logi->append("<font color=blue>"+saadetis+"</font>");
        serial->flush();
        if(uusLugemismasin){
            saatmiseEtapp = 3;  //tekst saadetud
#ifdef PROOV
        qDebug() << "saatmiseEtapp = 3";
#endif
        }
        break;
    case 4:
        serial->write(&ack);
#ifdef PROOV
        qDebug() << "saadan: ACK";
#endif
        emit teadeLogisse("<font color=blue>ACK</font>");
//        ui->logi->append("<font color=blue>ACK</font>");
        serial->flush();
        saatmiseEtapp = 0;  //ACK saadetud
#ifdef PROOV
        qDebug() << "saatmiseEtapp = 0";
#endif
        return;
        break;
    default:
        break;
    }//0 - algseis/ACK tulnud (lõpp), 1 - ENQ saadetud, 2 - STX saabunud, 3 - tekst saadetud, 4 - saabus tekst, vaja vastata ACK
}

void LugemisMasin::saada(QString s)
{
//    static QByteArray saadetis = s.toLatin1();

    if(!s.isEmpty())    //Kui s on tühi, siis ilmselt on tegu saatmise teise? etapiga
        saadetis = s.toLatin1();
    saatja->start();

    /*switch (saatmiseEtapp) {    //Olenevalt millises etapis parasjagu ollakse (vana masina puhul on see alati 2)
    case 0:
        serial->write(&enq);
#ifdef PROOV
        qDebug() << "saadan: ENQ";
#endif
        ui->logi->append("<font color=blue>ENQ</font>");
        serial->flush();
        saatmiseEtapp = 1;  //ENQ saadetud
#ifdef PROOV
        qDebug() << "saatmiseEtapp = 1";
#endif
        return;
        break;
    case 2:
        if(uusLugemismasin)
            saadetis.append(CRC(&saadetis));
        saadetis.append(CR);
        //serial->writeData(saadetis, saadetis.size());
        serial->write(saadetis);
#ifdef PROOV
        qDebug() << QString("saadan: %1").arg(QString(saadetis));
#endif
        ui->logi->append("<font color=blue>"+saadetis+"</font>");
        serial->flush();
        if(uusLugemismasin){
            saatmiseEtapp = 3;  //tekst saadetud
#ifdef PROOV
        qDebug() << "saatmiseEtapp = 3";
#endif
        }
        break;
    case 4:
        serial->write(&ack);
#ifdef PROOV
        qDebug() << "saadan: ACK";
#endif
        ui->logi->append("<font color=blue>ACK</font>");
        serial->flush();
        saatmiseEtapp = 0;  //ACK saadetud
#ifdef PROOV
        qDebug() << "saatmiseEtapp = 0";
#endif
        return;
        break;
    default:
        break;
    }*///0 - algseis/ACK tulnud (lõpp), 1 - ENQ saadetud, 2 - STX saabunud, 3 - tekst saadetud, 4 - saabus tekst, vaja vastata ACK
}

void LugemisMasin::seadista()
{
    seadistaja->setInterval(1000);  //Algv��rtuse taastamine seeria l�pus seadistamise jaoks
    if(uusLugemismasin){
        seadista2();
        return;
    }
    QString s = "111111111";
    if(relv < 2)
        s.replace(0, 1, QString("%1").arg(relv + 1)); //Milliseid lehti loetakse (harjutus)
    else s.replace(0, 1, "6");
    s.replace(8, 1, QString("%1").arg(laskudeArv));   //Mitu lasku lehes on
//    if(ui->kumnendikegaBox->isChecked())   //Kas lugeda laske kümnendiku täpsusega või mitte
        s.replace(3, 1, "2");
    saada(s);
}

void LugemisMasin::seadista2(){
    saatmiseEtapp = 0;
//    QString s = "SCH=KK50;TEA=KT;RIA=ZR;SSC=2;SZI=10;SGE=10;"; //"SCH=LP;TEA=KT;TEG=1500;SSC=2;SGE=10;";//"SCH=LGES;TEA=ZT;SSC=1;SZI=10;SGE=10;KSD;";
    QString s;
    switch(relv){  //Lehe tüübi (harjutuse) valimine, 0 - õhupüss, 1 - õhupüstol, 2 - sportpüss
    case 0 : s = "SCH=LGES;";
        break;
    case 1 : s = "SCH=LP;";
        break;
    case 2 : s = "SCH=KK50;";
        break;
    default : s = "SCH=LGES;";
    }
    s.append("TEA=KT;RIA=ZR;"); //Teiler täisarvudega ja väärtus kümnendikega
    s.append(QString("SSC=%1;").arg(laskudeArv));  //Laskude arv lehes
    s.append(QString("SGE=%1;SZI=%1;").arg(laskudeArv));    //Laskude arv seerias ja vahesummas
#ifdef PROOV
    s.append("KSD;");
    qDebug() << "Seadistamine: " << s;
#endif
//    if(!ui->trukkimiseBox->isChecked())   //Kas trükkida lehele lasud ja tulemused või ei
//        s.append("KSD;");
    saada(s);
}

void LugemisMasin::setCOMport(QString p)
{
    COMport = p;
}

void LugemisMasin::setLaskudeArv(int l)
{
    laskudeArv = l;
}

void LugemisMasin::setRelv(int r)
{
    relv = r;
}

void LugemisMasin::start()
{
    uhenda();
    lugeja->start();
}

void LugemisMasin::stop()
{
    lugeja->stop();
}

void LugemisMasin::sulgeUhendus()
{
    saada("EXIT");
    serial->close();
    lugeja->stop();
    uhendatud = false;
}

void LugemisMasin::uhenda()
{
    if(uusLugemismasin){
        uhenda2();
        return;
    }
    if(uhendatud){
//        QMessageBox::warning(this, "Protolehelugeja", tr("Ühendus juba loodud! Kui on mingi jama ja tahate uuesti ühendada, tuleb enne vajutada \"Sulge ühendus\"."), QMessageBox::Ok);
        saada("V");
        return;
    }
    if(!kaabelLeitud)   //Kui lugemismasina kaablit algul ei leitud, siis äkki vahepeal on see külge ühendatud
        uuendaPorte();

    emit teadeLogisse("Ühendamine: " + COMport);
//    ui->logi->append("Ühendamine: " + ui->comPort->currentText());
    serial->setPortName(COMport);
    serial->setBaudRate(BAUD2400);
    serial->setDataBits(DATA_8 /*(DataBitsType)(3)*/ );
    serial->setParity(PAR_NONE /*(ParityType)(0)*/ );
    serial->setStopBits(STOP_1 /*(StopBitsType)(0)*/ );
    serial->setFlowControl(FLOW_OFF /*(FlowType)(0)*/ );
    serial->setTimeout(100);
    serial->open(QIODevice::ReadWrite);

    serial->setDtr(true);
    serial->setRts(true);

    saada("V");
    serial->flush();
    lugeja->start();
    lugemismasinaValimine = true;
//    uhendatud = true;
}

void LugemisMasin::uhenda2()
{
    serial->flush();
    serial->close();

    serial->setPortName(COMport);
    serial->setBaudRate(BAUD9600);
    serial->setDataBits(DATA_8 /*(DataBitsType)(3)*/ );
    serial->setParity(PAR_NONE /*(ParityType)(0)*/ );
    serial->setStopBits(STOP_1 /*(StopBitsType)(0)*/ );
    serial->setFlowControl(/*FLOW_HARDWARE FLOW_XONXOFF*/ FLOW_OFF /*(FlowType)(0)*/ );
    serial->setTimeout(100);
    serial->open(QIODevice::ReadWrite);

    saatmiseEtapp = 0;
    saada("SNR=");
//    serial->flush();
    lugeja->start();
    uhendatud = true;
    QByteArray s = "ACK:";
    s.append(ACK);
    s.append(" NAK:");
    s.append(NAK);
    s.append(" CR");
    s.append(CR);
    s.append(" STX:");
    s.append(STX);
    s.append(" EOT:");
    s.append(EOT);
    s.append(" ENQ:");
    s.append(ENQ);
    emit teadeLogisse(QString(s));
//    ui->logi->append(QString(s));
}

void LugemisMasin::uuendaPorte()
{
    kaabelLeitud = false;
    pordid = QextSerialEnumerator::getPorts();
    COMport.clear();
//    ui->comPort->clear();
    foreach(QextPortInfo info, pordid){
//        ui->comPort->addItem(info.portName);  //Kõiki pole vaja salvestada, kuna nagunii ainult üks neist on huvitav
        emit teadeLogisse(QString("%1, %2").arg(info.portName).arg(info.friendName));
//        ui->logi->append(QString("%1, %2").arg(info.portName).arg(info.friendName));
        if((info.friendName.contains("Prolific", Qt::CaseInsensitive) || info.friendName.contains("serial", Qt::CaseInsensitive))
                && info.friendName.contains("USB", Qt::CaseInsensitive)){
            COMport = info.portName;
//            ui->comPort->setCurrentIndex(ui->comPort->count() - 1);
            kaabelLeitud = true;
        }
    }
//    if(!kaabelLeitud) QMessageBox::warning(this, "Protokollitaja", "Lugemismasina kaablit ei leitud!", QMessageBox::Ok);
}

