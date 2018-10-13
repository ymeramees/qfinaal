#include "lehelugeja.h"
#include "ui_lehelugeja.h"

//Lehelugeja::Lehelugeja(QextSerialPort *s, QString harjutus, QWidget *parent) :
//    QDialog(parent),
//    m_ui(new Ui::Lehelugeja)
//{
//    m_ui->setupUi(this);
//    connect(m_ui->portBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(pordiNimiMuutus(QString)));
//    connect(m_ui->uhendaNupp, SIGNAL(clicked()), this, SLOT(avaUhendus()));
//    connect(m_ui->seadistaNupp, SIGNAL(clicked()), this, SLOT(seadista()));
//    connect(m_ui->lopetaNupp, SIGNAL(clicked()), this, SLOT(lopetaUhendus()));
//    serial = s;
//    bool leitud = false;
//    pordid = QextSerialEnumerator::getPorts();
//    m_ui->portBox->clear();
//    foreach(QextPortInfo info, pordid){
//        m_ui->portBox->addItem(info.portName);
//        if((info.friendName.contains("Prolific", Qt::CaseInsensitive) || info.friendName.contains("serial", Qt::CaseInsensitive))
//                && info.friendName.contains("USB", Qt::CaseInsensitive)){
//            m_ui->portBox->setCurrentIndex(m_ui->portBox->count() - 1);
//            leitud = true;
//        }
//    }
//    if(harjutus.contains(QString::fromLatin1("püss"))){ //Automaatselt relva liigi seadistamine, ei toimi, kui avatakse teise relvaliigi finaal
//        m_ui->relvBox->setCurrentIndex(0);
//    }else if(harjutus.contains(QString::fromLatin1("püstol"))){
//        m_ui->relvBox->setCurrentIndex(1);
//    }else if(harjutus.contains(QString::fromLatin1("lamades")) || harjutus.contains(QString::fromLatin1("standard")) || harjutus.contains(QString::fromLatin1("3x"))){
//        m_ui->relvBox->setCurrentIndex(2);
//    }
//    if(!leitud) QMessageBox::warning(this, "Protokollitaja", "Lugemismasina kaablit ei leitud!", QMessageBox::Ok);
//    pordiNimi = m_ui->portBox->currentText();
//}

Lehelugeja::Lehelugeja(LugemisMasin *s, QString harjutus, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::Lehelugeja)
{
    m_ui->setupUi(this);
    connect(m_ui->portBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(pordiNimiMuutus(QString)));
    connect(m_ui->uhendaNupp, SIGNAL(clicked()), this, SLOT(avaUhendus()));
    connect(m_ui->seadistaNupp, SIGNAL(clicked()), this, SLOT(seadista()));
    connect(m_ui->lopetaNupp, SIGNAL(clicked()), this, SLOT(lopetaUhendus()));
//    serial = s;
    lugemisMasin = s;
    bool leitud = false;
    pordid = QextSerialEnumerator::getPorts();
    m_ui->portBox->clear();
    foreach(QextPortInfo info, pordid){
        m_ui->portBox->addItem(info.portName);
        if((info.friendName.contains("Prolific", Qt::CaseInsensitive) || info.friendName.contains("serial", Qt::CaseInsensitive))
                && info.friendName.contains("USB", Qt::CaseInsensitive)){
            m_ui->portBox->setCurrentIndex(m_ui->portBox->count() - 1);
            leitud = true;
        }
    }
    if(harjutus.contains(QString::fromLatin1("püss"))){ //Automaatselt relva liigi seadistamine, ei toimi, kui avatakse teise relvaliigi finaal
        m_ui->relvBox->setCurrentIndex(0);
        lugemisMasin->setRelv(0);
    }else if(harjutus.contains(QString::fromLatin1("püstol"))){
        m_ui->relvBox->setCurrentIndex(1);
        lugemisMasin->setRelv(1);
    }else if(harjutus.contains(QString::fromLatin1("lamades")) || harjutus.contains(QString::fromLatin1("standard")) || harjutus.contains(QString::fromLatin1("3x"))){
        m_ui->relvBox->setCurrentIndex(2);
        lugemisMasin->setRelv(2);
    }
    lugemisMasin->setLaskudeArv(1); //Finaalis on ikka 1 lask lehte
    if(!leitud) QMessageBox::warning(this, "Protokollitaja", "Lugemismasina kaablit ei leitud!", QMessageBox::Ok);
    pordiNimi = m_ui->portBox->currentText();
}

Lehelugeja::~Lehelugeja()
{
    delete m_ui;
}

void Lehelugeja::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Lehelugeja::pordiNimiMuutus(QString s)
{
    pordiNimi = m_ui->portBox->currentText();
}

void Lehelugeja::lisaLogisse(QString s)
{
    m_ui->logiAken->append(s);
}

void Lehelugeja::avaUhendus()
{
    // read in all the settings and open the port
//    serial->setPortName(m_ui->portBox->currentText());
//    serial->setBaudRate(BAUD2400);
//    serial->setDataBits(DATA_8 /*(DataBitsType)(3)*/ );
//    serial->setParity(PAR_NONE /*(ParityType)(0)*/ );
//    serial->setStopBits(STOP_1 /*(StopBitsType)(0)*/ );
//    serial->setFlowControl(FLOW_OFF /*(FlowType)(0)*/ );
//    serial->setTimeout(100);
//    serial->open(QIODevice::ReadWrite);

//    serial->setDtr(true);
//    serial->setRts(true);

//    QByteArray saadetis = QString('V').toLatin1();
//    saadetis.append(13);
//    serial->write(saadetis, saadetis.size());
//    lisaLogisse("<font color=blue>Ühenduse avamine...</font>");
//    lisaLogisse("<font color=blue>Port: " + pordiNimi + "</font>");
//    lisaLogisse("<font color=blue>V</font>");
//    serial->flush();
    lugemisMasin->start();
}

void Lehelugeja::seadista()
{
//    QString s;
//    switch(m_ui->relvBox->currentIndex()){
//        case 0 : {
//            s = "110211111";
//            break;
//        }
//        case 1 : {
//            s = "210211111";
//            break;
//        }
//        default : {
//            s = "610211111";
//            break;
//        }
//    }
//    QByteArray saadetis = s.toLatin1();
//    saadetis.append(13);
//    serial->write(saadetis, saadetis.size());
//    lisaLogisse("<font color=blue>"+s+"</font>");
//    serial->flush();
    lugemisMasin->setRelv(m_ui->relvBox->currentIndex());
    lugemisMasin->setCOMport(pordiNimi);
    lugemisMasin->seadista();
#ifdef PROOV
    qDebug() << "Lehelugeja::Seadistamine";
#endif
}

void Lehelugeja::seadistaVorgule()  //See vist ei tee päris seda, mida peaks (identne seadista() fn'ga)
{
//    QString s;
//    switch(m_ui->relvBox->currentIndex()){
//        case 0 : {
//            s = "210211111";
//            break;
//        }
//        case 1 : {
//            s = "110211111";
//            break;
//        }
//        default : {
//            s = "610211111";
//            break;
//        }
//    }
//    QByteArray saadetis = s.toLatin1();
//    saadetis.append(13);
//    serial->write(saadetis, saadetis.size());
//    lisaLogisse("<font color=blue>"+s+"</font>");
//    serial->flush();
    lugemisMasin->seadista();
}

void Lehelugeja::lopetaUhendus()
{
//    QString s = "EXIT";
//    QByteArray saadetis = s.toLatin1();
//    saadetis.append(13);
//    serial->write(saadetis, saadetis.size());
//    lisaLogisse("<font color=blue>"+s+"</font>");
//    serial->flush();
//    serial->close();
    lugemisMasin->sulgeUhendus();
    emit portSuletud();
    lisaLogisse("<font color=blue>Ühendus suletud!</font>");
}
