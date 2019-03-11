#include <QtGui>
#include "qfinaalmain.h"

/////////////////////////////////////////////////////////////////////////////
/// ToDo list:
/// Portide otsimine, kui kaablit nime järgi ei leita, ei tööta
/// Võrku lugemine ilmselt ei tööta
/// Ei kontrollita, et lehes oleks ainult üks lask (veateadet ei tule enam)
/// Marklehe tüübi valik ei tööta, kuna peale uut valikut tulevad valged lehed ja laskusid ei joonistata....ok
///
/////////////////////////////////////////////////////////////////////////////

//#define _TTY_WIN_

extern QString programmiNimi;
extern QString versioon;
extern QString argument;
extern QString aasta;
//extern QDir asukoht;

QFinaalMain::QFinaalMain()
{
	est = new QLocale(QLocale::Estonian, QLocale::Estonia);
	server = 0;
	socket = 0;
	siusDataSocket = 0;
	mituKohal = 8;
        mituLasknud = 0;    //Mitu inimest on lasknud

    //connect(socket, SIGNAL(readyRead()), this, SLOT(loeVorgust()));
	uusAct = new QAction(QIcon(":/images/new.png"), tr("&Uus"), this);
	uusAct->setShortcut(QKeySequence::New);
	uusAct->setStatusTip(tr("Loo uus fail, avab vastava faili kui sellenimeline on juba "
	"olemas"));
	connect(uusAct, SIGNAL(triggered()), this, SLOT(uus()));
	avaAct = new QAction(QIcon(":/images/open.png"), tr("&Ava"), this);
	avaAct->setShortcut(QKeySequence::Open);
	avaAct->setStatusTip(tr("Ava fail"));
	connect(avaAct, SIGNAL(triggered()), this, SLOT(ava()));
	saveAct = new QAction(QIcon(":/images/save.png"), tr("&Salvesta"), this);
	saveAct->setShortcut(QKeySequence::Save);
	saveAct->setStatusTip(tr("Salvesta fail"));
	connect(saveAct, SIGNAL(triggered()), this, SLOT(salvesta()));
	nsaveAct = new QAction(QIcon(":/images/salvesta_kui.png"), tr("S&alvesta nimega..."), this);
	nsaveAct->setShortcut(tr("Ctrl+a"));
        nsaveAct->setStatusTip(QString::fromLatin1("Salvesta koopia failist määratud nimega"));
	connect(nsaveAct, SIGNAL(triggered()), this, SLOT(salvestaNimega()));
        peidaNaitaLehedAct = new QAction(QString::fromLatin1("Märklehed"), this);
        peidaNaitaLehedAct->setStatusTip(QString::fromLatin1("Peidab või toob nähtavale märklehed tulemuste aknas"));
        connect(peidaNaitaLehedAct, SIGNAL(triggered()), this, SLOT(peidaNaitaLehed()));
        peidaNaitaNimedAct = new QAction(QString::fromLatin1("Nimed"), this);
        peidaNaitaNimedAct->setStatusTip(QString::fromLatin1("Peidab või toob nähtavale nimed tulemuste aknas"));
        connect(peidaNaitaNimedAct, SIGNAL(triggered()), this, SLOT(peidaNaitaNimed()));
	printAct = new QAction(QIcon(":/images/print.png"), tr("&Prindi..."), this);
	printAct->setShortcut(QKeySequence::Print);
	printAct->setStatusTip(tr("Prindi tulemuse aken"));
	connect(printAct, SIGNAL(triggered()), this, SLOT(print()));
        valjuAct = new QAction(QIcon(":/images/exit.png"), QString::fromLatin1("&Välju"), this);
	valjuAct->setShortcut(tr("Ctrl+q"));
        valjuAct->setStatusTip(QString::fromLatin1("Välju programmist"));
	connect(valjuAct, SIGNAL(triggered()), this, SLOT(close()));
    eksportAct = new QAction(QIcon(":/images/eksport.png"), tr("&Ekspordi..."), this);
	eksportAct->setShortcut(tr("Ctrl+e"));
	eksportAct->setStatusTip(tr("Ekspordi tulemused .txt faili"));
	connect(eksportAct, SIGNAL(triggered()), this, SLOT(eksport()));
    importAct = new QAction(QIcon(":/images/import.png"), tr("&Impordi..."), this);
	importAct->setShortcut(tr("Ctrl+i"));
	importAct->setStatusTip(tr("Impordi tulemused eelmisest Finaali failist."));
	connect(importAct, SIGNAL(triggered()), this, SLOT(import()));
    impordiSiusStartListAct = new QAction(tr("Import Sius startlist..."), this);
    impordiSiusStartListAct->setStatusTip(tr("Impordi Sius'i startlisti fail"));
    connect(impordiSiusStartListAct, SIGNAL(triggered()), this, SLOT(impordiSiusStartList()));
	naitaTul = new QAction(QIcon(":/images/tulemuse_aken.png"), tr("Tulemuste aken"), this);
	naitaTul->setStatusTip(tr("Näitab tulemuste akna"));
	connect(naitaTul, SIGNAL(triggered()), this, SLOT(naitaTulemused()));
    infoAct = new QAction(tr("Programmist..."), this);
	infoAct->setStatusTip(tr("Kuvab infot programmi kohta"));
	connect(infoAct, SIGNAL(triggered()), this, SLOT(naitaInfo()));
    abiWAct = new QAction(tr("Abi..."), this);
        abiWAct->setStatusTip(QString::fromLatin1("Näitab, kuidas Windowsis kahele monitorile seadistada"));
	connect(abiWAct, SIGNAL(triggered()), this, SLOT(naitaWAbi()));
	seadedAct = new QAction(tr("Seaded..."), this);
	seadedAct->setStatusTip(tr("Avab seadete akna"));
	connect(seadedAct, SIGNAL(triggered()), this, SLOT(naitaSeaded()));
	seadistaAct = new QAction(tr("Seadista"), this);
        seadistaAct->setStatusTip(QString::fromLatin1("Seadistab lehelugemismasina, kui ühendus on avatud"));
	connect(seadistaAct, SIGNAL(triggered()), this, SLOT(seadistaLehelugemismasin()));
        vahemalluAct = new QAction(QString::fromLatin1("Vahemällu"), this);
        vahemalluAct->setStatusTip(QString::fromLatin1("Kopeeri tulemused vahemällu"));
	connect(vahemalluAct, SIGNAL(triggered()), this, SLOT(vahemallu()));
        kaivitaServerAct = new QAction(QString::fromLatin1("Käivita server"), this);
        kaivitaServerAct->setStatusTip(QString::fromLatin1("Käivitab serveri, et saata tulemused üle võrgu"));
	connect(kaivitaServerAct, SIGNAL(triggered()), this, SLOT(kaivitaServer()));
    kustutaLasudAct = new QAction(tr("Kustuta lasud"), this);
    kustutaLasudAct->setStatusTip(QString::fromLatin1("Kustutab kõik lasud!"));
    connect(kustutaLasudAct, SIGNAL(triggered()), this, SLOT(kustutaLasud()));
	lehelugejaAct = new QAction(tr("Lehelugemismasin..."), this);
	lehelugejaAct->setStatusTip(tr("Loe lehti lehelugemismasinaga"));
	connect(lehelugejaAct, SIGNAL(triggered()), this, SLOT(lehelugeja()));
	loeSiiaAct = new QAction(tr("Loe siia"), this);
	loeSiiaAct->setStatusTip(tr("Lehtede lugemine käib siia finaali aknasse"));
	connect(loeSiiaAct, SIGNAL(triggered()), this, SLOT(loeSiia()));
        loeVorkuAct = new QAction(QString::fromLatin1("Loe võrku"), this);
        loeVorkuAct->setStatusTip(QString::fromLatin1("Lehtede lugemine käib üle võrgu teise arvutisse"));
	connect(loeVorkuAct, SIGNAL(triggered()), this, SLOT(loeVorku()));
        uhenduServerigaAct = new QAction(QString::fromLatin1("Ühendu serveriga"), this);
        uhenduServerigaAct->setStatusTip(QString::fromLatin1("Ühendub võrgus oleva tulemuste serveriga"));
	connect(uhenduServerigaAct, SIGNAL(triggered()), this, SLOT(uhenduServeriga()));
	uhenduSiusDatagaAct = new QAction(tr("SiusData"), this);
        uhenduSiusDatagaAct->setStatusTip(QString::fromLatin1("Ühendub SiusDataga"));
    connect(uhenduSiusDatagaAct, SIGNAL(triggered()), this, SLOT(uhenduSiusDataga()));
//    connect(uhenduSiusDatagaAct, SIGNAL(triggered()), this, SLOT(loeSiusDatast()));
	xlsAct = new QAction(QIcon(":/images/eksport.png"), tr(".xls fail"), this);
	//xlsAct->setShortcut(tr("Ctrl+x"));
	xlsAct->setStatusTip(tr("Ekspordi tulemused .xls faili"));
	connect(xlsAct, SIGNAL(triggered()), this, SLOT(xls()));

    aeg = new QTimer(this);		//Timer, et ValikVast'is saaks kasutada qApp->quit() slot'i
    aeg->setSingleShot(true);
    connect(aeg, SIGNAL(timeout()), qApp, SLOT(quit()));
    aeg2 = new QTimer(this);
    connect(aeg2, SIGNAL(timeout()), this, SLOT(naitaWAbi()));
    aeg2->setSingleShot(true);
	
	failMenu = menuBar()->addMenu("&Fail");
	failMenu->addAction(uusAct);
	failMenu->addAction(avaAct);
	failMenu->addAction(saveAct);
	failMenu->addAction(nsaveAct);
	failMenu->addSeparator();
	failMenu->addAction(printAct);
	failMenu->addSeparator();
	failMenu->addAction(valjuAct);
	
	editMenu = menuBar()->addMenu("&Tulemused");
	editMenu->addAction(eksportAct);
	editMenu->addAction(xlsAct);
	editMenu->addAction(importAct);
    editMenu->addAction(impordiSiusStartListAct);
	editMenu->addAction(vahemalluAct);
	editMenu->addAction(lehelugejaAct);
	editMenu->addAction(naitaTul);
	editMenu->addSeparator();
	editMenu->addAction(seadedAct);
#ifdef PROOV
    editMenu->addAction(kustutaLasudAct);
#endif
	
	helpMenu = menuBar()->addMenu("&Abi");
	helpMenu->addAction(abiWAct);
	helpMenu->addAction(infoAct);
	
	loeSiiaAct->setCheckable(true);
	loeVorkuAct->setCheckable(true);
	
	toolBar = new QToolBar(this);
	toolBar->addAction(uusAct);
	toolBar->addAction(avaAct);
	toolBar->addAction(saveAct);
	toolBar->addAction(lehelugejaAct);
	toolBar->addAction(naitaTul);
	toolBar->addAction(seadistaAct);
	toolBar->addAction(uhenduSiusDatagaAct);
        toolBar->addAction(peidaNaitaLehedAct);
        toolBar->addAction(peidaNaitaNimedAct);
	//toolBar->addAction(kaivitaServerAct);
	//toolBar->addAction(uhenduServerigaAct);
	
	this->addToolBar(toolBar);

    tulemus = 0; //initTulemuseaken() tuleb hiljem
	tabel = new QFinaal(this);
	setCentralWidget(tabel);

	this->setWindowTitle(programmiNimi);
	this->setWindowIcon(QIcon(":/images/Finaal.ico"));
    this->setGeometry(0, 20, 1400/*this->width()*/, 600/*this->height()*/);
//    this->resize(this->width(), 800);
	(void) new QShortcut(Qt::Key_Escape, this, SLOT(close()));
	setStatusBar(statusBar());
	seaded = new Seaded(this);
	seaded->setModal(true);
	valik = new ValikKast(this);
	connect(seaded->ui.olguNupp, SIGNAL(clicked()), this, SLOT(salvestaSeaded()));
	lugeja = 0;
    lugemisMasin = 0;
    uhendumiseAken = 0; //SiusDataga ühendumiseks
    radadeAken = 0; //Rajanumbrite kontrollimiseks ja määramiseks
//	timer = new QTimer(this);
//	timer->setInterval(100);
//	connect(timer, SIGNAL(timeout()), this, SLOT(loeLehti()));
    seadistaja = new QTimer(this);
    seadistaja->setInterval(1000);
    seadistaja->setSingleShot(true);
    connect(seadistaja, SIGNAL(timeout()), this, SLOT(seadistaLehelugemismasin()));
    lehtedePeitja = new QTimer(this);   //Peidab lehed peale viimast lasku
    lehtedePeitja->setInterval(seaded->ui.marklehtedeAegBox->value()*1000);
    lehtedePeitja->setSingleShot(true);
    connect(lehtedePeitja, SIGNAL(timeout()), this, SLOT(peidaLehed()));
	//QMessageBox::critical(this, programmiNimi, QString("%1").arg(argv[0]), QMessageBox::Ok);
	//QDir di(dir);
	//di.cdUp();
#ifdef PROOV
            qDebug() << "Vahepunkt1";
#endif

	QFile fail(qApp->applicationDirPath() + "/Data/Alg.ini");
	if(fail.open(QIODevice::ReadOnly | QIODevice::Text)){
		QTextStream sisse(&fail);
		sisse.setCodec("UTF-8");
		sisse.readLine();
		sisse.readLine();
		//valik->ui.voistluseNimi->setText(sisse.readLine());
		//valik->ui.alaNimi->setText(sisse.readLine());
		valik->ui.failiNimi->setText(sisse.readLine());
		//seaded->ui.lineEdit->setText(sisse.readLine());
		seaded->ui.comboBox->setCurrentIndex(sisse.readLine().toInt());
        fail.close();
        fail.setFileName(valik->ui.failiNimi->text());
        if(fail.open(QIODevice::ReadOnly)){
            QDataStream sisse(&fail);
//            while(!sisse.atEnd()){
                quint32 kontroll, versioon;
                sisse >> kontroll >> versioon;
                if(kontroll != 0x00FA3058){
                    QMessageBox::critical(this, programmiNimi, QString::fromLatin1("Vigane või vale fail!"), QMessageBox::Ok);
#ifdef PROOV
                    qDebug() << "Vigane fail, kontroll: " << kontroll;
#endif
                    return;
                }
                if(versioon == 11){
                    QString nimi = fail.fileName().section("/", -1, -1);
                    if(nimi.endsWith(".fnl"))
                        nimi.chop(QString(".fnl").length());
                    valik->ui.alaNimi->setText(nimi);
                    nimi = fail.fileName().section("/", -2, -2);
                    valik->ui.voistluseNimi->setText(nimi);
                }else if(versioon >= 12 && versioon <= 15){
                    QString rida;
                    sisse >> rida;
                    valik->ui.voistluseNimi->setText(rida);
                    rida.clear();
                    sisse >> rida;
                    valik->ui.alaNimi->setText(rida);
                }
                /*if(versioon >= 11 && versioon <= 14){
                    int laskudeArv = 0;
                    if(versioon == 11 || versioon == 12)
                    laskudeArv = 10;	//Vanade reeglite järgi on finaalis 10 lasku
                    else laskudeArv = 20;	//uute järgi 20
                    for(int i = 0; i < 8; i++){
                        QString rida[laskudeArv + 2];
#ifdef PROOV
                        qDebug() << "Vahepunkt2, i = " << i << "tabel->id.count = " << tabel->id.count();
#endif
                        if(versioon >= 14)
                            sisse >> tabel->id[i];
#ifdef PROOV
            qDebug() << "Vahepunkt3";
#endif
                        sisse >> rida[0] >> rida[1];
                        tabel->nimi[i]->setText(rida[0]);
                        tabel->tulemus[i]->setText(rida[1]);
                        for(int j = 0; j < laskudeArv; j++){
                            sisse >> rida[j+2];
                            tabel->lask[j][i]->setText(rida[j+2]);
                        }
                        for(int j = 0; j < laskudeArv; j++){
                            sisse >> rida[j+2];
                            tabel->lisaLask[j][i]->setText(rida[j+2]);
                        }
                    }
                }*/else QMessageBox::critical(this, programmiNimi, tr("Vale versiooni fail!"), QMessageBox::Ok);
//            }
        }
	}else QMessageBox::critical(this, programmiNimi, tr("Ei leia seadete faili!"), QMessageBox::Ok);

	connect(tabel->tulemus[0], SIGNAL(editingFinished()), this, SLOT(summ0()));
	connect(tabel->tulemus[1], SIGNAL(editingFinished()), this, SLOT(summ1()));
	connect(tabel->tulemus[2], SIGNAL(editingFinished()), this, SLOT(summ2()));
	connect(tabel->tulemus[3], SIGNAL(editingFinished()), this, SLOT(summ3()));
	connect(tabel->tulemus[4], SIGNAL(editingFinished()), this, SLOT(summ4()));
	connect(tabel->tulemus[5], SIGNAL(editingFinished()), this, SLOT(summ5()));
	connect(tabel->tulemus[6], SIGNAL(editingFinished()), this, SLOT(summ6()));
	connect(tabel->tulemus[7], SIGNAL(editingFinished()), this, SLOT(summ7()));
    for(int j = 0; j < 24; j++){
		connect(tabel->lask[j][0], SIGNAL(editingFinished()), this, SLOT(summ0()));
		connect(tabel->lisaLask[j][0], SIGNAL(editingFinished()), this, SLOT(summ0()));
		connect(tabel->lask[j][1], SIGNAL(editingFinished()), this, SLOT(summ1()));
		connect(tabel->lisaLask[j][1], SIGNAL(editingFinished()), this, SLOT(summ1()));
		connect(tabel->lask[j][2], SIGNAL(editingFinished()), this, SLOT(summ2()));
		connect(tabel->lisaLask[j][2], SIGNAL(editingFinished()), this, SLOT(summ2()));
		connect(tabel->lask[j][3], SIGNAL(editingFinished()), this, SLOT(summ3()));
		connect(tabel->lisaLask[j][3], SIGNAL(editingFinished()), this, SLOT(summ3()));
		connect(tabel->lask[j][4], SIGNAL(editingFinished()), this, SLOT(summ4()));
		connect(tabel->lisaLask[j][4], SIGNAL(editingFinished()), this, SLOT(summ4()));
		connect(tabel->lask[j][5], SIGNAL(editingFinished()), this, SLOT(summ5()));
		connect(tabel->lisaLask[j][5], SIGNAL(editingFinished()), this, SLOT(summ5()));
		connect(tabel->lask[j][6], SIGNAL(editingFinished()), this, SLOT(summ6()));
		connect(tabel->lisaLask[j][6], SIGNAL(editingFinished()), this, SLOT(summ6()));
		connect(tabel->lask[j][7], SIGNAL(editingFinished()), this, SLOT(summ7()));
		connect(tabel->lisaLask[j][7], SIGNAL(editingFinished()), this, SLOT(summ7()));
	}
	
	//tab = new QKeyEvent(QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier);

	for(int i = 0; i < 8; i++){
        eelmineRida << "Rida";
		connect(tabel->kohal[i], SIGNAL(toggled(bool)), this, SLOT(kasKohal()));
		connect(tabel->nimi[i], SIGNAL(returnPressed()), this, SLOT(vajutaTab()));
		connect(tabel->tulemus[i], SIGNAL(returnPressed()), this, SLOT(vajutaTab()));
        for(int j = 0; j < 24; j++){
			connect(tabel->lask[j][i], SIGNAL(returnPressed()), this, SLOT(vajutaTab()));
			connect(tabel->lisaLask[j][i], SIGNAL(returnPressed()), this, SLOT(vajutaTab()));
		}
	}

#ifdef PROOV
    QMessageBox::information(this, "Teade", "Debug versioon!", QMessageBox::Ok);
#endif
	argument = qApp->arguments().last();
    if(argument == "8946Protokollitajast" || argument.endsWith(".fnl")){
        if(argument != "8946Protokollitajast")
            valik->ui.failiNimi->setText(argument);
        //ava3(qApp->arguments()[qApp->argc() - 1]);
        setWindowTitle(programmiNimi + " - " + valik->ui.failiNimi->text());
        loeFail();
        salvestaSeaded();
        initTulemuseaken();
    }else{
        do{
            if(valik->exec() == QDialog::Accepted){
                setWindowTitle(programmiNimi + " - " + valik->ui.failiNimi->text());
#ifdef PROOV
            qDebug() << "setWindowTitle()";
#endif
                loeFail();
#ifdef PROOV
            qDebug() << "loeFail()";
#endif
                salvestaSeaded();
#ifdef PROOV
            qDebug() << "salvestaSeaded()";
#endif
                initTulemuseaken();
            }else{
                valik->ui.voistluseNimi->setText("ProtokollitajaKood::Väljumine<Välju>return code 0021947");
                aeg->start(50);
                return;
            }
        }while(valik->ui.voistluseNimi->text().isEmpty());

        if(valik->ui.voistluseNimi->text() != "ProtokollitajaKood::Väljumine<Välju>return code 0021947" && !valik->ui.voistluseNimi->text().isEmpty()){
            //Logi, kuhu pannakse lasud ja muudatuste info
            logi = new QFile(QDir::toNativeSeparators(valik->ui.failiNimi->text().left(QDir::fromNativeSeparators(valik->ui.failiNimi->text()).lastIndexOf('/')) + QString("/Finaal logi %1.log").arg(QDate::currentDate().toString(Qt::ISODate))));
#ifdef PROOV
            qDebug() << "logi: " << logi->fileName();
#endif
            if(logi->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){ //Uue sessiooni alustamine
                QTextStream logiValja(logi);
                logiValja << "///////////////////////////////////////////////" << QDateTime::currentDateTime().toString() <<  "//////////////////////////////////////////////////\n";
            }
            logi->close();
            loeFail();
        }
    }
    if(valik->ui.voistluseNimi->text().isEmpty()){
        aeg->start(50);
        return;
    }
	/*if(qApp->arguments().count() > 1){
		if(qApp->arguments()[qApp->argc() - 1] != "8946Protokollitajast"){
		ava3(qApp->arguments()[qApp->argc() - 1]);
		}else{
		ava();
		}
	}else aeg->start(50);*/
}

QFinaalMain::~QFinaalMain()
{
}

void QFinaalMain::uuendaLasuNrit()
{
    mitmesLask = 0;    //Mitmes lask parasjagu on, alates 1'st
    mituPuudu = 0;  //Mitu inimest ei ole kohal
    mituLasknud = 0;    //Mitu inimest on lasknud
    koikLasknud = true;
    for(int j = 0; j < 8; j++){
        if(tabel->lask[0][j]->text() == "00")
        mituPuudu++;
        for(int k = 0; k < 24; k++)
        if(tabel->lask[k][j]->text() != "00"){
            if(mitmesLask <= k)
            mitmesLask = k+1;
        }
    }
    if(mitmesLask != 0) //Kui ei ole esimene lask
        for(int i = 0; i < 8; i++)
            if(tabel->kohal[i]->isChecked() && tabel->lask[mitmesLask - 1][i]->text() == "00")
                koikLasknud = false;
    if((mitmesLask == 5 || mitmesLask == 10 || mitmesLask > 10) && koikLasknud && seaded->ui.marklehtedeVahetamiseBox->currentIndex() == 0){
        lehtedePeitja->setInterval(seaded->ui.marklehtedeAegBox->value()*1000);
        lehtedePeitja->start(); //Kui kõik on lasknud, siis lehed peale aja möödumist uuesti peitu, et nimed näha oleks, kui seadetes see sisse on lülitatud
    }else lehtedePeitja->stop(); //Et uue seeria esimese lasu järel lehti ära ei peidetaks

    tulemus->mitmesLask = mitmesLask;
}

void QFinaalMain::uuendaSeaded()
{
    lehtedePeitja->setInterval(seaded->ui.marklehtedeAegBox->value()*1000);

    if(seaded->ui.marklehtedeVahetamiseBox->currentIndex() == 1)
        lehtedePeitja->stop();

    for(int i = 0; i < 8; i++){
        tulemus->marklehed[i]->init(seaded->ui.relvaValik->currentIndex()); //Uuendab lehtede tüüpi
    }
}

void QFinaalMain::uus()
{
	salvesta();
	for(int i = 0; i < 8; i++){
        //tabel->kohal[i]->setChecked(true);    //Millegipärast rikkus see sama faili uuesti laadimise ära
        tabel->id[i] = "";
		tabel->nimi[i]->setText("");
		tabel->tulemus[i]->setText("0");
		tabel->seeria1[i]->setText("0,0");
		tabel->seeria2[i]->setText("0,0");
		tabel->seeria[i]->setText("0,0");
		//tabel->summa[i]->setText("0");
		tabel->iSumma[i] = 0;
//		//*tulemus->summa[i] = "0";
//		*tulemus->seeria[i] = "0";
//		*tulemus->nimi[i] = "";
//		//*tulemus->rada[i] = QString("%1").arg(i+1);
//		*tulemus->rada[i] = QString(QChar(i + 65));
//		*tulemus->lask[i] = "...";
//		*tulemus->vahe[i] = "0";
//		*tulemus->koht[i] = QString("%1.").arg(i+1);
        for(int j = 0; j < 24; j++){
			tabel->lask[j][i]->setText("00");
			tabel->lisaLask[j][i]->setText("00");
			tabel->lisaLask[j][i]->hide();
		}                        
		tabel->lisaLabel[i]->hide();
		/*if(tabel->lisaLask[0][i]->text() != "00"){
			for(int k = 0; k < 20; k++)
			tabel->lisaLask[k][i]->show();
			tabel->lisaLabel[i]->show();
		}*/
	}
    puhastaTulemuseaken();
	do{
		if(valik->exec() == QDialog::Accepted){
			setWindowTitle(programmiNimi + " - " + valik->ui.failiNimi->text());
			salvestaSeaded();
			}else{
			aeg->start(50);
		}
	}while(valik->ui.voistluseNimi->text().isEmpty());
	if(valik->ui.voistluseNimi->text().isEmpty()){
		aeg->start(50);
		return;
	}
	loeFail();
}

void QFinaalMain::loeFail()
{
    if(valik->ui.failiNimi->text().isEmpty()) return;
    qApp->setOverrideCursor(Qt::WaitCursor);
    /*QDir di(dir);
                QMessageBox::critical(this, programmiNimi, tr(QString("Dir: .%1")).arg(di.absolutePath()), QMessageBox::Ok);
                di.cdUp();
                QMessageBox::critical(this, programmiNimi, tr(QString("Dir: .%1")).arg(di.absolutePath()), QMessageBox::Ok);
                if(seaded->ui.lineEdit->text().isEmpty()){
		if(!di.cd("Tulemused"))
		di.mkdir("Tulemused");
		di.cd("Tulemused");
		}else{
		if(!di.cd(seaded->ui.lineEdit->text()))
		di.mkdir(seaded->ui.lineEdit->text());
		di.cd(seaded->ui.lineEdit->text());
		}
		if(!di.cd(valik->ui.voistluseNimi->text()))
		di.mkdir(valik->ui.voistluseNimi->text());
	di.cd(valik->ui.voistluseNimi->text());*/
    QFile fail(valik->ui.failiNimi->text());
    if(fail.open(QIODevice::ReadOnly)){
        voistlus = false;
        for(int i = 0; i < 8; i++){
            tabel->kohal[i]->setChecked(true);
            tabel->id[i] = "";
            tabel->nimi[i]->setText("");
            tabel->tulemus[i]->setText("0");
            tabel->seeria1[i]->setText("0,0");
            tabel->seeria2[i]->setText("0,0");
            tabel->seeria[i]->setText("0,0");
            //tabel->summa[i]->setText("0");
            tabel->iSumma[i] = 0;
            //*tulemus->summa[i] = "0";
//			*tulemus->seeria[i] = "0";
//			*tulemus->nimi[i] = "";
            //*tulemus->rada[i] = QString("%1").arg(i+1);
//			*tulemus->rada[i] = QString(QChar(i + 65));
//			*tulemus->lask[i] = "...";
//			*tulemus->vahe[i] = "0";
//			*tulemus->koht[i] = QString("%1.").arg(i+1);
            for(int j = 0; j < 24; j++){
                tabel->lask[j][i]->setText("00");
                tabel->lisaLask[j][i]->setText("00");
            }
            for(int k = 0; k < 24; k++)
            tabel->lisaLask[k][i]->hide();
            tabel->lisaLabel[i]->hide();
            /*if(tabel->lisaLask[0][i]->text() != "00"){    //Milleks see siin vajalik on?
                for(int k = 0; k < 20; k++)
                tabel->lisaLask[k][i]->show();
                tabel->lisaLabel[i]->show();
                }*/
        }
        puhastaTulemuseaken();
        QDataStream sisse(&fail);
        while(!sisse.atEnd()){
            quint32 kontroll, versioon;
            sisse >> kontroll >> versioon;
            if(kontroll != 0x00FA3058){
                QMessageBox::critical(this, programmiNimi, tr("Vigane või vale fail!"), QMessageBox::Ok);
#ifdef PROOV
                qDebug() << "Loefail(): Vigane fail, kontroll: " << kontroll;
#endif
                return;
            }
#ifdef PROOV
                qDebug() << "Loefail(): kontroll " << kontroll << ", versioon: " << versioon;
#endif
            if(versioon >= 12 && versioon <= 15){
                QString rida;
                sisse >> rida;
                valik->ui.voistluseNimi->setText(rida);
                rida.clear();
                sisse >> rida;
                valik->ui.alaNimi->setText(rida);
            }else if(versioon == 11){
                QString nimi = fail.fileName().section("/", -1, -1);
                if(nimi.endsWith(".fnl"))
                    nimi.chop(QString(".fnl").length());
                valik->ui.alaNimi->setText(nimi);
                nimi = fail.fileName().section("/", -2, -2);
                valik->ui.voistluseNimi->setText(nimi);
            }

            maaraRelv(); //Märklehtede ümber seadistamine, kõigepealt relva valik

            if(versioon >= 11 && versioon <= 15){
                int laskudeArv = 0;
                if(versioon == 11 || versioon == 12)
                    laskudeArv = 10;	//Vanade reeglite järgi on finaalis 10 lasku
                else if(versioon == 13 || versioon == 14)
                    laskudeArv = 20;	//uute järgi 20
                else
                    laskudeArv = 24;	//2017a reeglite järgi 24

                for(int i = 0; i < 8; i++){
                    QString rida[laskudeArv + 2];
                    if(versioon >= 14)
                        sisse >> tabel->id[i];
                    sisse >> rida[0] >> rida[1];
                    tabel->nimi[i]->setText(rida[0]);
                    tabel->tulemus[i]->setText(rida[1]);
                    for(int j = 0; j < laskudeArv; j++){
                        sisse >> rida[j+2];
                        tabel->lask[j][i]->setText(rida[j+2]);
                    }
                    for(int j = 0; j < laskudeArv; j++){
                        sisse >> rida[j+2];
                        tabel->lisaLask[j][i]->setText(rida[j+2]);
                    }
                    //Märklehtede ümberseadistamine
                    if(tulemus != 0){   //Algul tulemuse akent veel ei ole
                        tulemus->marklehed[i]->setNimi(tabel->nimi[i]->text());
                        tulemus->marklehed[i]->setRada(QString("%1").arg(QChar(65+i)));
                        tulemus->marklehed[i]->setTulemus(tabel->seeria[i]->text());
                        tulemus->marklehed[i]->init(relv); //Määrab ära, mis tüüpi lehed on
                    }
                }
            }else QMessageBox::critical(this, programmiNimi, tr("Vale versiooni fail!"),QMessageBox::Ok);
        }
        this->setWindowTitle(programmiNimi + " - " + valik->ui.failiNimi->text());
        for(int i = 0; i < 8; i++)
            summ(i);
        /*for(int i = 0; i < 8; i++){
            for(int k = 0; k < 20; k++)
            tabel->lisaLask[k][i]->hide();
            tabel->lisaLabel[i]->hide();
            if(tabel->lisaLask[0][i]->text() != "00"){
			for(int k = 0; k < 20; k++)
			tabel->lisaLask[k][i]->show();
			tabel->lisaLabel[i]->show();
			}
		}*/
        jarjesta();
    }else{
        salvesta();
        QMessageBox::critical(this, programmiNimi, tr("Ei leia faili!"), QMessageBox::Ok);
    }
    qApp->restoreOverrideCursor();
}

void QFinaalMain::ava()
{
	QString failiNimi;
	if(failiNimi.isEmpty())
	failiNimi = QFileDialog::getOpenFileName(this, tr("Ava"), "",
	tr("Finaali fail (*.fnl)"));
	if(failiNimi.isEmpty()) return;
	valik->ui.failiNimi->setText(failiNimi);
	setWindowTitle(programmiNimi + " - " + valik->ui.failiNimi->text());
	loeFail();
	salvestaSeaded();
}

/*void QFinaalMain::ava2()
	{
	//ava3("");
	}
	
	void QFinaalMain::ava3(QString s)
	{
	QString failiNimi;
	if(failiNimi.isEmpty())
	failiNimi = QFileDialog::getOpenFileName(this, tr("Ava"), "Tulemused",
	tr("Finaali fail (*.fnl)"));
	if(failiNimi.isEmpty()) return;
	QFile fail(failiNimi);
	qApp->setOverrideCursor(Qt::WaitCursor);
	if(fail.open(QIODevice::ReadOnly)){
	for(int i = 0; i < 8; i++){
	tabel->nimi[i]->setText("");
	tabel->tulemus[i]->setText("0");
	tabel->seeria[i]->setText("0,0");
	tabel->summa[i]->setText("0");
	tabel->iSumma[i] = 0;
	*tulemus->summa[i] = "0";
	*tulemus->seeria[i] = "0";
	*tulemus->nimi[i] = "";
	*tulemus->rada[i] = QString("%1").arg(i+1);
	*tulemus->lask[i] = "...";
	*tulemus->vahe[i] = "0";
	*tulemus->koht[i] = QString("%1").arg(i+1);
	for(int j = 0; j < 10; j++){
	tabel->lask[j][i]->setText("00");
	tabel->lisaLask[j][i]->setText("00");
	}
	for(int k = 0; k < 10; k++)
	tabel->lisaLask[k][i]->hide();
	tabel->lisaLabel[i]->hide();
	if(tabel->lisaLask[0][i]->text() != "00"){
	for(int k = 0; k < 10; k++)
	tabel->lisaLask[k][i]->show();
	tabel->lisaLabel[i]->show();
	}
	}
	QDataStream sisse(&fail);
	while(!sisse.atEnd()){
	quint32 kontroll, versioon;
	sisse >> kontroll >> versioon;
	if(kontroll != 0x00FA3058){
	QMessageBox::critical(this, programmiNimi, tr("Vigane või vale fail!"),
	QMessageBox::Ok);
	return;
	}
	if(versioon == 11){
	for(int i = 0; i < 8; i++){
	QString rida[12];
	sisse >> rida[0] >> rida[1];
	tabel->nimi[i]->setText(rida[0]);
	tabel->tulemus[i]->setText(rida[1]);
	for(int j = 0; j < 10; j++){
	sisse >> rida[j+2];
	tabel->lask[j][i]->setText(rida[j+2]);
	}
	for(int j = 0; j < 10; j++){
	sisse >> rida[j+2];
	tabel->lisaLask[j][i]->setText(rida[j+2]);
	}
	}
	}else QMessageBox::critical(this, programmiNimi, tr("Vale versiooni fail!"),
	QMessageBox::Ok);
	}
	QString nimi = fail.fileName().section("/", -1, -1);
	if(nimi.endsWith(".fnl"))
	nimi.chop(QString(".fnl").length());
	valik->ui.alaNimi->setText(nimi);
	QDir dir(fail.fileName());
	dir.cdUp();
	valik->ui.voistluseNimi->setText(dir.dirName());
	this->setWindowTitle(programmiNimi + " - " + valik->ui.failiNimi->text());
	salvestaSeaded();
	}else if(valik->avamine){
	aeg->start(50);
	}else QMessageBox::critical(this, programmiNimi, tr("Ei õnnestu faili avada!"), QMessageBox::Ok);
	for(int i = 0; i < 8; i++)
	summ(i);
	for(int i = 0; i < 8; i++){
	for(int k = 0; k < 10; k++)
	tabel->lisaLask[k][i]->hide();
	tabel->lisaLabel[i]->hide();
	if(tabel->lisaLask[0][i]->text() != "00"){
	for(int k = 0; k < 10; k++)
	tabel->lisaLask[k][i]->show();
	tabel->lisaLabel[i]->show();
	}
	}
	jarjesta();
	qApp->restoreOverrideCursor();
}*/

void QFinaalMain::eksport()
{
	/*QMessageBox::information(this, "Teade", QString("Asukoht: x = %1; y = %2\n reso: %3x%4")
		.arg(this->pos().x()).arg(this->pos().y()).arg(qApp->desktop()->width())
	.arg(qApp->desktop()->height()), QMessageBox::Ok);*/
	QString failiNimi = QFileDialog::getSaveFileName(this, tr("Ekspordi"), "Tulemused/" +
	valik->ui.alaNimi->text() + ".txt",	tr("Teksti dokument (*.txt)"));
	if(failiNimi.isEmpty()) return;
	if(!failiNimi.endsWith(".txt")) failiNimi.append(".txt");
	QFile fail(failiNimi);
	fail.open(QIODevice::WriteOnly | QIODevice::Text);
	QTextStream out(&fail);
	for(int i = 0; i < 8; i++){
		out << tabel->nimi[i]->text() << "\t" << tabel->tulemus[i]->text();
        for(int j = 0; j < 24; j++)
		out << "\t" << tabel->lask[j][i]->text();
		out << "\n";
	}
	/*	QFile xls("proov.xls");
		xls.open(QIODevice::ReadOnly | QIODevice::Text);
		QTextStream sxls(&xls);
		QFile xls2("ProovOut.xls");
		xls2.open(QIODevice::WriteOnly | QIODevice::Text);
		QTextStream vxls(&xls2);
		while(!sxls.atEnd()){
		vxls << sxls.readLine();
	}*/
	QMessageBox::information(this, programmiNimi, tr("Tulemused eksporditud"),	QMessageBox::Ok);
}

void QFinaalMain::impordiSiusStartList()
{
    QString failiNimi = QFileDialog::getOpenFileName(this, tr("Ava startlist"), valik->ui.failiNimi->text().left(QDir::fromNativeSeparators(valik->ui.failiNimi->text()).lastIndexOf("/")), tr("Comma separated file (*.csv)"));
    if(failiNimi.isEmpty())
        return;

    QFile fail(failiNimi);
    if(fail.open(QIODevice::ReadOnly | QIODevice::Text)){
        if(!radadeAken)
            radadeAken = new RadadeAken(this);
        radadeAken->clear();    //Vanad nimed igaks juhuks eest ära

        QTextStream sisse(&fail);
        //QString rida;
        QStringList read;
        while(!sisse.atEnd()){  //Kogu faili sisse lugemine
            read.append(sisse.readLine());
        }
        for(int i = 0; i < 8 && i < read.count(); i++){
//            read.append(sisse.readLine());
            //rida = sisse.readLine();
            //ID no;Startno;Name;Firstname;Disp name;Nat;Cat;Group;Team;Bay;Target;Relay;Starttime;BoxTg;Active;Q Tot;Avg;Rank;G1;...;G12;10s;...;0s;Mouches
            //QStringList jupid = rida.split(";");
            QStringList jupid = read.at(i).split(";");
            if(jupid.count() < 15)
                break; //Rida liiga lühike, järelikult on mingi viga
            //radadeAken->setRada(i, jupid[10].toInt() - 1); //Rajanumbrid on numbritena, olgugi, et näidatakse tähti
            //radadeAken->setNimi(i, jupid[4].replace("\"", ""));
            if(!radadeAken->setRida(i, jupid[10].toInt() - 1, jupid[4].replace("\"", ""), jupid[1], jupid[15])){ //Radade akna täitmine
#ifdef PROOV
                qDebug() << "laskur: " << i;
#endif
                QMessageBox::critical(this, "Viga", "Vigane Sius startlisti fail!", QMessageBox::Ok);
                return;
            }
            //int rajaNr = jupid[10].toInt() - 1;
//#ifdef PROOV
//        qDebug() << "rajaNr = " << rajaNr;
//#endif
//            if(rajaNr < 0 || rajaNr >= tabel->id.count()){
//                QMessageBox::critical(this, "Viga", "Vigane Sius startlisti fail!\n\nRaja nr on liiga suur või puudub!", QMessageBox::Ok);
//                return;
//            }
            /*if(!tabel->id[i].isEmpty() && tabel->id[i] != read[1]){
                if(QMessageBox::warning(this, "Hoiatus", QString("Laskuril %1 on juba teistsugune ID olemas!\n\nKas soovite vana ID "
                        "(%2) asendada uuega (%3)?").arg(tabel->nimi[i]->text()).arg(tabel->id[i]).arg(read[1]),
                        QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok)
                    tabel->id[i] = read[1];
            }else{
                tabel->id[i] = read[1];
            }
            if(tabel->nimi[i]->text().isEmpty()){*/
//            tabel->id[rajaNr] = jupid[1];
//            tabel->nimi[rajaNr]->setText(jupid[4].replace("\"", ""));
//            tabel->tulemus[rajaNr]->setText(jupid[15]);
//            }else{
//                QMessageBox::warning(this, "Hoiatus", "Kuna nimed on juba olemas, siis neid ei uuendatud! Kui soovite nimed failist sisse lugeda, siis kõigepealt kustutage nimede lahtrid tühjaks ja seejärel importige uuesti!", QMessageBox::Ok);
//            }
        }
        if(radadeAken->exec()){
            for(int rajaNr = 0; rajaNr < 8; rajaNr++){  //Rida Finaali aknas
                for(int j = 0; j < 8; j++){ //Rida radade aknas
                    if(radadeAken->getRada(j) == rajaNr + 1){
                        tabel->id[rajaNr] = radadeAken->getId(j);
                        tabel->nimi[rajaNr]->setText(radadeAken->getNimi(j));
                        tabel->tulemus[rajaNr]->setText(radadeAken->getTulemus(j));
                    }
                }
            }
        }
        initTulemuseaken(); // Kole moodus, et nimed märklehtedele tuleks
    }else
        QMessageBox::critical(this, "Viga", "Ei õnnestunud faili avada!", QMessageBox::Ok);
}

void QFinaalMain::jarjesta()
{
    if(tulemus == 0) return;    //Et enne tulemuseakna loomist programm katki ei läheks
    //QMessageBox::information(this, "Teade", QString("Laius = %1 ja kõrgus = %2").arg(width())
    //	.arg(height()), QMessageBox::Ok);

    uuendaLasuNrit();   //Mitmes lask parasjagu käsil on

    if(mitmesLask == 5){
        *tulemus->las = "1. seeria";
        }else if(mitmesLask == 10){
        *tulemus->las = "2. seeria";
        }else{
        *tulemus->las = (est->toString(mitmesLask) + ". lask");
    }
    if(mitmesLask == 0){
        *tulemus->las = "Tulemus";
        for(int i = 0; i < 8; i++){
            *tulemus->koht[i] = *tulemus->rada[i];
        }
    }else for(int i = 0; i < 8; i++){
        *tulemus->koht[i] = QString("%1.").arg(i+1);
    }
    int larv = 0;
    for(int j = 0; j < 8; j++){
        for(int k = 0; k < 24; k++)
        if(tabel->lisaLask[k][j]->text() != "00"){
            if(larv <= k)
            larv = k+1;
            *tulemus->las = /*"Lisalask"*/(est->toString((larv / 2) + 1) + ". lisal.");
        }
    }
    if((*tulemus->las == "1. lisal." && mitmesLask > 12) || (*tulemus->las == "2. lisal." && mitmesLask > 14)
            || (*tulemus->las == "3. lisal." && mitmesLask > 16) || (*tulemus->las == "4. lisal." && mitmesLask > 18)
            || (*tulemus->las == "5. lisal." && mitmesLask > 20) || (*tulemus->las == "6. lisal." && mitmesLask > 22))
            *tulemus->las = (est->toString(mitmesLask) + ". lask");
    peidaLisaLasud();
    /*for(int i = 0; i < 8; i++){ //Vaja ümber teha
        if(tabel->lask[9][i]->text() == "00" && tabel->lask[11][i]->text() == "00"
        && tabel->lask[13][i]->text() == "00" && tabel->lask[15][i]->text() == "00"
        && tabel->lask[17][i]->text() == "00" && tabel->lask[19][i]->text() == "00"){
        for(int j = 0; j < 20; j++)
        tabel->lisaLask[j][i]->hide();
        tabel->lisaLabel[i]->hide();
        }
    }*/
    resize(width(), 455);
    QList<int>nimekiri;		//Vajalik summade järjekorda panekuks
    bool readTais[8] = {0,0,0,0,0,0,0,0};	//Näitab, kas sellel real on juba tulemus
    bool naidatud[8] = {0,0,0,0,0,0,0,0};	//Näitab, kas see tulemus on juba näidatud
    //bool onnestus = false;
    //QMessageBox::information(this, "Teade", QString("Arv = %1").arg(arv), QMessageBox::Ok);
    for(int i = 0; i < 8; i++){
        nimekiri << tabel->iSumma[i];
    }
    qSort(nimekiri.begin(), nimekiri.end());//Seab summad suuruse järgi kasvavas järjestuses
    //for(int s = 0; s < 4; s++)
    //QMessageBox::information(this, "Teade", QString("Summa: %1").arg(nimekiri[0]), QMessageBox::Ok);
    for(int i = 0; i < 8; i++){		//Rea nr tulemuse aknas
        *tulemus->lask[i] = "...";
        for(int j = 0; j < 8; j++){	//Rea nr tabelis
            int summa = tabel->iSumma[j];
            if(summa == nimekiri[7-i]){//Alustada tuleb nimekirja lõpust, kuna seal on suuremad summad
                if(readTais[i] == false && naidatud[j] == false){//Kui mõlemad on false,
                    //tähendab see, et tulemus on näitamata ja tulemuste aknas on vastav rida tühi
                    if(seaded->ui.valjalangemiseBox->isChecked()){  //Kui seadetest on automaatne väljalangemine sisse lülitatud
                        switch(mitmesLask){    //Kui on väljalangemise lask (8., 10., 12. jne)
                            case 12:{
                                //Kas on kõik lasknud?
                                mituLasknud = mituPuudu;    //Need, kes on puudu, on tinglikult juba lasknud
                                for(int i = 0; i < 8; i++)
                                if(tabel->lask[mitmesLask - 1][i]->text() != "00")
                                mituLasknud++;
                                //Kõige väiksema tulemusega laskur
                                if(summa == nimekiri[0] && summa != nimekiri[1] && mituLasknud == 8){  //Kui eesolev summa ei ole sama
                                    //Võtta ära kohalolemise linnuke
                                    tabel->kohal[j]->setChecked(false);
                                }
                                break;
                            }
                            case 14:{
                                //Kas on kõik lasknud?
                                if(mituPuudu > 1)
                                mituLasknud = mituPuudu - 1;
                                else
                                mituLasknud = 0;

                                for(int i = 0; i < 8; i++)
                                if(tabel->lask[mitmesLask - 1][i]->text() != "00")
                                mituLasknud++;

                                //Kõige väiksema tulemusega laskur
                                if(summa == nimekiri[1] && summa != nimekiri[2] && mituLasknud == 7){  //Kui eesolev summa ei ole sama
                                    //Võtta ära kohalolemise linnuke
                                    tabel->kohal[j]->setChecked(false);
                                }
                                break;
                            }
                            case 16:{
                                //Kas on kõik lasknud?
                                if(mituPuudu > 2)
                                mituLasknud = mituPuudu - 2;
                                else
                                mituLasknud = 0;

                                for(int i = 0; i < 8; i++)
                                if(tabel->lask[mitmesLask - 1][i]->text() != "00")
                                mituLasknud++;
                                //Kõige väiksema tulemusega laskur
                                if(summa == nimekiri[2] && summa != nimekiri[3] && mituLasknud == 6){  //Kui eesolev summa ei ole sama
                                    //Võtta ära kohalolemise linnuke
                                    tabel->kohal[j]->setChecked(false);
                                }
                                break;
                            }
                            case 18:{
                                //Kas on kõik lasknud?
                                if(mituPuudu > 3)
                                mituLasknud = mituPuudu - 3;
                                else
                                mituLasknud = 0;

                                for(int i = 0; i < 8; i++)
                                if(tabel->lask[mitmesLask - 1][i]->text() != "00")
                                mituLasknud++;
                                //Kõige väiksema tulemusega laskur
                                if(summa == nimekiri[3] && summa != nimekiri[4] && mituLasknud == 5){  //Kui eesolev summa ei ole sama
                                    //Võtta ära kohalolemise linnuke
                                    tabel->kohal[j]->setChecked(false);
                                }
                                break;
                            }
                            case 20:{
                                //Kas on kõik lasknud?
                                if(mituPuudu > 4)
                                mituLasknud = mituPuudu - 4;
                                else
                                mituLasknud = 0;

                                for(int i = 0; i < 8; i++)
                                if(tabel->lask[mitmesLask - 1][i]->text() != "00")
                                mituLasknud++;
                                //Kõige väiksema tulemusega laskur
                                if(summa == nimekiri[4] && summa != nimekiri[5] && mituLasknud == 4){  //Kui eesolev summa ei ole sama
                                    //Võtta ära kohalolemise linnuke
                                    tabel->kohal[j]->setChecked(false);
                                }
                                break;
                                }case 22:{
                                //Kas on kõik lasknud?
                                if(mituPuudu > 5)
                                mituLasknud = mituPuudu - 5;
                                else
                                mituLasknud = 0;

                                for(int i = 0; i < 8; i++)
                                if(tabel->lask[mitmesLask - 1][i]->text() != "00")
                                mituLasknud++;
                                //Kõige väiksema tulemusega laskur
                                if(summa == nimekiri[5] && summa != nimekiri[6] && mituLasknud == 3){  //Kui eesolev summa ei ole sama
                                    //Võtta ära kohalolemise linnuke
                                    tabel->kohal[j]->setChecked(false);
                                }
                                break;
                            }
                        }
//#ifdef PROOV
//                        qDebug() << "mituPuudu = " << mituPuudu << "; mituLasknud = " << mituLasknud;
//#endif
                    }
                    tulemus->kohal[i] = tabel->kohal[j]->isChecked();
                    *tulemus->nimi[i] = tabel->nimi[j]->text();
                    //*tulemus->rada[i] = est->toString(j + 1) + ".";
                    *tulemus->rada[i] = QString(QChar(j + 65));
                    *tulemus->seeria[i] = tabel->seeria[j]->text();
                    //*tulemus->summa[i] = tabel->summa[j]->text();
                    if(mitmesLask > 0){
                        if(tulemus->las->endsWith("lask")){
                            //for(int k = 0; k < 10; k++)
                            if(tabel->lask[mitmesLask - 1][j]->text() != "00")
                            *tulemus->lask[i] = tabel->lask[mitmesLask - 1][j]->text();
                            }else if(tulemus->las->contains("1. seeria") && tabel->seeria1[j]->text() != "0,0"){
                            *tulemus->lask[i] = tabel->seeria1[j]->text();
                            }else if(tulemus->las->contains("2. seeria") && tabel->seeria2[j]->text() != "0,0"){
                            *tulemus->lask[i] = tabel->seeria2[j]->text();
                            }else{
                            //for(int k = 0; k < 10; k++)
                            if(larv > 0 && tabel->lisaLask[larv - 1][j]->text() != "00")
                            *tulemus->lask[i] = tabel->lisaLask[larv - 1][j]->text();
                        }
                    }else if(!tabel->tulemus[j]->text().isEmpty() && *tulemus->las == "Tulemus"){
                        *tulemus->lask[i] = tabel->tulemus[j]->text();
                    }
                    readTais[i] = true;		//Kogu see jama on vajalik juhuks, kui on võrdsed
                    naidatud[j] = true;		//tulemused
                }
            }
        }
    }
    for(int i = 0; i < 7; i++){ // Lisalaskude järgi järjestamine - pigem ikka lihtsalt laskude järgi järjestamine?
        QString lask1 = *tulemus->lask[i];
        QString lask2 = *tulemus->lask[i+1];
        int tulemus1 = lask1.replace(',', "").toInt();
        int tulemus2 = lask2.replace(',', "").toInt();
        if(*tulemus->seeria[i] == *tulemus->seeria[i+1] && larv > 0 && tulemus1 < tulemus2){
            bool vanaKohal = tulemus->kohal[i];
            QString vanaNimi = *tulemus->nimi[i];
            QString vanaRada = *tulemus->rada[i];
            QString vanaSeeria = *tulemus->seeria[i];
            //QString vanaSumma = *tulemus->summa[i];
            QString vanaLask = *tulemus->lask[i];
            tulemus->kohal[i] = tulemus->kohal[i+1];
            *tulemus->nimi[i] = *tulemus->nimi[i+1];
            *tulemus->rada[i] = *tulemus->rada[i+1];
            *tulemus->seeria[i] = *tulemus->seeria[i+1];
            //*tulemus->summa[i] = *tulemus->summa[i+1];
            *tulemus->lask[i] = *tulemus->lask[i+1];
            tulemus->kohal[i] = vanaKohal;
            *tulemus->nimi[i+1] = vanaNimi;
            *tulemus->rada[i+1] = vanaRada;
            *tulemus->seeria[i+1] = vanaSeeria;
            //*tulemus->summa[i+1] = vanaSumma;
            *tulemus->lask[i+1] = vanaLask;
        }
    }
    float vahe = 0;
    int svahe = 0;
    float esimene = 0;
    float teine = 0;
    //QString onn = "Sitt lugu";
    //QMessageBox::information(this, "Teade", "Vahede arvutamine", QMessageBox::Ok);
    QList<int>iNimekiri;
    for(int i = 0; i < 8; i++)
    iNimekiri << tabel->iSumma[i];
    qSort(iNimekiri.begin(), iNimekiri.end(), qGreater<int>());
    if(seaded->ui.comboBox->currentIndex() == 0){
        for(int i = 1; i < 8; i++){
            esimene = iNimekiri[i-1];
            teine = iNimekiri[i];
            svahe = esimene - teine;
            vahe = svahe;
            vahe /= 10;
            if(svahe % 10 != 0)//Kas on täisarv, et vajadusel lisada lõppu ,0
            *tulemus->vahe[i] = est->toString(vahe);
            else *tulemus->vahe[i] = (est->toString(vahe) + ",0");
        //}else tabel->summa[i]->setText("Viga");
    }
    }else{
        for(int i = 1; i < 8; i++){
            esimene = iNimekiri[0];
            teine = iNimekiri[i];
            svahe = esimene - teine;
            vahe = svahe;
            vahe /= 10;
            if(svahe % 10 != 0)//Kas on täisarv, et vajadusel lisada lõppu ,0
            *tulemus->vahe[i] = est->toString(vahe);
            else *tulemus->vahe[i] = (est->toString(vahe) + ",0");
        }
    }
#ifndef PROOV
    salvesta();
#endif
    //QMessageBox::information(this, "Teade", "Salvestatud", QMessageBox::Ok);
    *tulemus->pealKiri = valik->ui.alaNimi->text();
    //if(*tulemus->las == "8. lask")
    tulemus->joonista();

    int mitmelLisaLask = 0;
    //Kui vahe on 0, on järelikult võrdsed tulemused ja vaja ümber lasta, et selgitada välja kukkuja:
    if((*tulemus->las == "12. lask" || *tulemus->las == "1. lisal.") && *tulemus->vahe[7] == "0,0"){
        for(int i = 0; i < 8; i++){
            if(tabel->seeria[i]->text() == *tulemus->seeria[7]){
                tabel->lisaLask[0][i]->show();
                tabel->lisaLask[1][i]->show();
                tabel->lisaLabel[i]->show();
                mitmelLisaLask++;
            }
        }
    }else if((*tulemus->las == "14. lask" || *tulemus->las == "2. lisal.") && *tulemus->vahe[6] == "0,0"){
        for(int i = 0; i < 8; i++){
            if(tabel->seeria[i]->text() == *tulemus->seeria[6]){
                tabel->lisaLask[2][i]->show();
                tabel->lisaLask[3][i]->show();
                tabel->lisaLabel[i]->show();
                mitmelLisaLask++;
            }
        }
    }else if((*tulemus->las == "16. lask" || *tulemus->las == "3. lisal.") && *tulemus->vahe[5] == "0,0"){
        for(int i = 0; i < 8; i++){
            if(tabel->seeria[i]->text() == *tulemus->seeria[5]){
                tabel->lisaLask[4][i]->show();
                tabel->lisaLask[5][i]->show();
                tabel->lisaLabel[i]->show();
                mitmelLisaLask++;
            }
        }
    }else if((*tulemus->las == "18. lask" || *tulemus->las == "4. lisal.") && *tulemus->vahe[4] == "0,0"){
        for(int i = 0; i < 8; i++){
            if(tabel->seeria[i]->text() == *tulemus->seeria[4]){
                tabel->lisaLask[6][i]->show();
                tabel->lisaLask[7][i]->show();
                tabel->lisaLabel[i]->show();
                mitmelLisaLask++;
            }
        }
    }else if((*tulemus->las == "20. lask" || *tulemus->las == "5. lisal.") && *tulemus->vahe[3] == "0,0"){
        for(int i = 0; i < 8; i++){
            if(tabel->seeria[i]->text() == *tulemus->seeria[3]){
                tabel->lisaLask[8][i]->show();
                tabel->lisaLask[9][i]->show();
                tabel->lisaLabel[i]->show();
                mitmelLisaLask++;
            }
        }
    }else if((*tulemus->las == "22. lask" || *tulemus->las == "6. lisal.") && *tulemus->vahe[2] == "0,0"){
        for(int i = 0; i < 8; i++){
            if(tabel->seeria[i]->text() == *tulemus->seeria[2]){
                tabel->lisaLask[10][i]->show();
                tabel->lisaLask[11][i]->show();
                tabel->lisaLabel[i]->show();
                mitmelLisaLask++;
            }
        }
    }else if((*tulemus->las == "24. lask" || *tulemus->las == "7. lisal.") && *tulemus->vahe[1] == "0,0"){
        for(int i = 0; i < 8; i++){
            if(tabel->seeria[i]->text() == *tulemus->seeria[1]){
                tabel->lisaLask[12][i]->show();
                tabel->lisaLask[13][i]->show();
                tabel->lisaLabel[i]->show();
                mitmelLisaLask++;
            }
        }
    }
        switch(mitmelLisaLask){
        case 0 : tabel->area->setMinimumHeight(395+50);
            break;
        case 2 : tabel->area->setMinimumHeight(480+50);
            break;
        case 3 : tabel->area->setMinimumHeight(525+50);
            break;
        case 4 : tabel->area->setMinimumHeight(565+50);
            break;
        case 5 : tabel->area->setMinimumHeight(610+50);
            break;
        case 6 : tabel->area->setMinimumHeight(650+50);
            break;
        case 7 : tabel->area->setMinimumHeight(695+50);
            break;
        case 8 : tabel->area->setMinimumHeight(735+50);
            break;
        }
    /*if(*tulemus->las == "8. lask" || *tulemus->las == "10. lask" || *tulemus->las == "12. lask" || *tulemus->las == "14. lask"
        || *tulemus->las == "16. lask" || *tulemus->las == "18. lask" || *tulemus->las == "20. lask"){
        for(int i = 0; i < 8; i++){
        if(i < 7)
        if(*tulemus->seeria[i] == *tulemus->seeria[i+1] && *tulemus->seeria[i] != "0,0"){
        //tabel->vKast->setSpacing(16);
        for(int u = 0; u < 8; u++){
        if(*tulemus->seeria[i] == tabel->seeria[u]->text() && !tabel->seeria[u]->text().isEmpty() &&
        tabel->seeria[u]->text() != "0,0" && tabel->lask[19][u]->text() != "00"){
        for(int k = 0; k < 20; k++)
        tabel->lisaLask[k][u]->show();
        tabel->lisaLabel[u]->show();
        }
        }
        }
        if(i != 0)
        if(*tulemus->seeria[i] == *tulemus->seeria[i-1] && *tulemus->seeria[i] != "0,0"){
        //tabel->vKast->setSpacing(16);
        for(int u = 0; u < 8; u++){
        if(*tulemus->seeria[i] == tabel->seeria[u]->text() && !tabel->seeria[u]->text().isEmpty() &&
        tabel->seeria[u]->text() != "0,0" && tabel->lask[19][u]->text() != "00"){
        for(int k = 0; k < 20; k++)
        tabel->lisaLask[k][u]->show();
        tabel->lisaLabel[u]->show();
        }
        }
        }
        }
    }*/
    //emit resizeEvent(new QResizeEvent(*(new QSize), *(new QSize)));
}

void QFinaalMain::closeEvent(QCloseEvent *event){
    if(QMessageBox::warning(this, programmiNimi, QString::fromLatin1("Oled kindel, et soovid programmist väljuda?"),
            QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok)==QMessageBox::Cancel)
        event->ignore();
	else{
        if(lugemisMasin != 0)
            lugemisMasin->sulgeUhendus();
		if(server != 0)
		server->close();
		qApp->quit();
	}
}

void QFinaalMain::summ(int i)
{
    if(tabel->nimi[i]->text() == "") return;
    qApp->setOverrideCursor(Qt::WaitCursor);

	bool onnestus = false;
    for(int x = 0; x < 24; x++){
		if(tabel->lask[x][i]->text() == "" || tabel->lask[x][i]->text() == " "
		|| tabel->lask[x][i]->text() == "  ")
		tabel->lask[x][i]->setText("00");
		if(tabel->lisaLask[x][i]->text() == "" || tabel->lisaLask[x][i]->text() == " "
		|| tabel->lisaLask[x][i]->text() == "  ")
		tabel->lisaLask[x][i]->setText("00");
		if(tabel->lask[x][i]->text().contains('.'))
		tabel->lask[x][i]->setText(tabel->lask[x][i]->text().replace('.', ','));
		if(tabel->lask[x][i]->text().contains("-"))
		tabel->lask[x][i]->setText(tabel->lask[x][i]->text().replace("-", ","));
		if(tabel->lask[x][i]->text().contains("m"))
		tabel->lask[x][i]->setText(tabel->lask[x][i]->text().replace("m", ","));
		if(tabel->lisaLask[x][i]->text().contains('.'))
		tabel->lisaLask[x][i]->setText(tabel->lisaLask[x][i]->text().replace('.', ','));
		if(tabel->lisaLask[x][i]->text().contains("-"))
		tabel->lisaLask[x][i]->setText(tabel->lisaLask[x][i]->text().replace("-", ","));
		if(tabel->lisaLask[x][i]->text().contains("m"))
		tabel->lisaLask[x][i]->setText(tabel->lisaLask[x][i]->text().replace("m", ","));
	}
	if(tabel->lask[0][i]->text() == "00"){
		//tabel->summa[i]->setText(tabel->tulemus[i]->text());
		tabel->iSumma[i] = qRound(est->toFloat(tabel->seeria[i]->text(), &onnestus)*10);
                jarjesta();
                qApp->restoreOverrideCursor();
		return;
	}
	//int sum = 0;
	int seeria1 = 0;
	int seeria2 = 0;
	int seeria = 0;
	int iarv = 0;
	float ssum = 0;
	float arv = 0;
	arv = 0/*tabel->tulemus[i]->text().toFloat(&onnestus)*/;
	/*QString onn;
		if(onnestus) onn = "Edu";
		else onn = "Sitt lugu";
	QMessageBox::information(this, "Teade", onn, QMessageBox::Ok);*/
	iarv = arv * 10;
	//sum = iarv;
	//QMessageBox::information(this, "Teade", QString::number(sum), QMessageBox::Ok);
    for(int j = 0; j < 24; j++){
		arv = est->toFloat(tabel->lask[j][i]->text(), &onnestus);
		/*if(onnestus) onn = "Edu";
			else onn = "Sitt lugu";
		QMessageBox::information(this, "Teade", onn, QMessageBox::Ok);*/
		iarv = qRound(arv * 10);
		//QString::number(arv);
		//QMessageBox::information(this, "Teade", QString::number(arv), QMessageBox::Ok);
		//sum += iarv;
		//QString::number(sum);
		//QMessageBox::information(this, "Teade", QString::number(sum), QMessageBox::Ok);
		//arv = tabel->lask[j][i]->text().toFloat(&onnestus);
		//arv *= 10;
		seeria += iarv;
		//QMessageBox::information(this, "Teade", QString("arv = %1\niarv = %2\nseeria = %3").arg(arv).arg(iarv).arg(seeria), QMessageBox::Ok);
	}
    for(int j = 0; j < 5; j++){
		arv = est->toFloat(tabel->lask[j][i]->text(), &onnestus);
		iarv = qRound(arv * 10);
		seeria1 += iarv;
	}
    for(int j = 5; j < 10; j++){
		arv = est->toFloat(tabel->lask[j][i]->text(), &onnestus);
		iarv = qRound(arv * 10);
		seeria2 += iarv;
	}
	/*if(onnestus == true){
		tabel->iSumma[i] = sum;
		ssum = sum;
		ssum /= 10;
		if(sum % 10 != 0)
		tabel->summa[i]->setText(est->toString(ssum));
		else tabel->summa[i]->setText(est->toString(ssum) + ",0");
	}else tabel->summa[i]->setText("Viga");*/
	if(onnestus == true){
        tabel->iSumma[i] = seeria;
        ssum = seeria1;
        ssum /= 10;
        if(seeria1 % 10 != 0)
        tabel->seeria1[i]->setText(est->toString(ssum));
        else tabel->seeria1[i]->setText(est->toString(ssum) + ",0");
        ssum = seeria2;
        ssum /= 10;
        if(seeria2 % 10 != 0)
        tabel->seeria2[i]->setText(est->toString(ssum));
        else tabel->seeria2[i]->setText(est->toString(ssum) + ",0");
        ssum = seeria;
        ssum /= 10;
        if(seeria % 10 != 0)
        tabel->seeria[i]->setText(est->toString(ssum));
        else tabel->seeria[i]->setText(est->toString(ssum) + ",0");
    }else{
        tabel->seeria1[i]->setText("Viga");
        tabel->seeria2[i]->setText("Viga");
        tabel->seeria[i]->setText("Viga");
    }
        jarjesta();
        qApp->restoreOverrideCursor();
}

void QFinaalMain::summ0()
{
//	if(tabel->nimi[0]->text() == "") return;
//	qApp->setOverrideCursor(Qt::WaitCursor);
	summ(0);
//	jarjesta();
//	qApp->restoreOverrideCursor();
}

void QFinaalMain::summ1()
{
//	if(tabel->nimi[1]->text() == "") return;
//	qApp->setOverrideCursor(Qt::WaitCursor);
	summ(1);
//	jarjesta();
//	qApp->restoreOverrideCursor();
}

void QFinaalMain::summ2()
{
//	if(tabel->nimi[2]->text() == "") return;
//	qApp->setOverrideCursor(Qt::WaitCursor);
	summ(2);
//	jarjesta();
//	qApp->restoreOverrideCursor();
}

void QFinaalMain::summ3()
{
//	if(tabel->nimi[3]->text() == "") return;
//	qApp->setOverrideCursor(Qt::WaitCursor);
	summ(3);
//	jarjesta();
//	qApp->restoreOverrideCursor();
}

void QFinaalMain::summ4()
{
//	if(tabel->nimi[4]->text() == "") return;
//	qApp->setOverrideCursor(Qt::WaitCursor);
	summ(4);
//	jarjesta();
//	qApp->restoreOverrideCursor();
}

void QFinaalMain::summ5()
{
//	if(tabel->nimi[5]->text() == "") return;
//	qApp->setOverrideCursor(Qt::WaitCursor);
	summ(5);
//	jarjesta();
//	qApp->restoreOverrideCursor();
}

void QFinaalMain::summ6()
{
//	if(tabel->nimi[6]->text() == "") return;
//	qApp->setOverrideCursor(Qt::WaitCursor);
	summ(6);
//	jarjesta();
//	qApp->restoreOverrideCursor();
}

void QFinaalMain::summ7()
{
//	if(tabel->nimi[7]->text() == "") return;
//	qApp->setOverrideCursor(Qt::WaitCursor);
	summ(7);
//	jarjesta();
//	qApp->restoreOverrideCursor();
}

void QFinaalMain::naitaValik()
{
	/*//emit resizeEvent(new QResizeEvent(*(new QSize), *(new QSize)));
		valik->exec();
		if(valik->avamine){
		ava2();
		valik->avamine = false;
		}else
	ava();*/
}

void QFinaalMain::naitaTulemused()
{
    tulemus->setGeometry(20, 20, 640, 480);
	tulemus->show();
}

void QFinaalMain::vajutaTab()
{
	QApplication::sendEvent(tabel->nimi[0], new QKeyEvent(QEvent::KeyPress, Qt::Key_Tab, Qt::NoModifier));
	QApplication::sendEvent(tabel->nimi[0], new QKeyEvent(QEvent::KeyRelease, Qt::Key_Tab, Qt::NoModifier));
}

void QFinaalMain::peidaLehed()
{
    tulemus->peidaLehed();
    tulemus->naitaNimed();
}

void QFinaalMain::peidaNaitaLehed()
{
    if(!tulemus->marklehed[0]->isHidden()){
        tulemus->peidaLehed();
        tulemus->naitaNimed();  //Et ekraan tühjaks ei jääks
    }else tulemus->naitaLehed();
}

void QFinaalMain::peidaNaitaNimed()
{
    if(!tulemus->silt->isHidden()){
        tulemus->peidaNimed();
        tulemus->naitaLehed();  //Et ekraan tühjaks ei jääks
    }else tulemus->naitaNimed();
}

void QFinaalMain::peidaLisaLasud()
{
    for(int i = 0; i < 8; i++){
        //if(tabel->lask[9][i]->text() == "00" && tabel->lask[11][i]->text() == "00" && tabel->lask[13][i]->text() == "00"
        //&& tabel->lask[15][i]->text() == "00" && tabel->lask[17][i]->text() == "00" && tabel->lask[19][i]->text() == "00"){
        for(int j = 0; j < 7; j++){
            if(tabel->lisaLask[j*2][i]->text() == "00" && tabel->lisaLask[j*2+1][i]->text() == "00"){
                tabel->lisaLask[j*2][i]->hide();
                tabel->lisaLask[j*2+1][i]->hide();
            }
        }
        bool olemasLask = false;
        for(int j = 0; j < 24; j++)
            if(tabel->lisaLask[j][i]->text() != "00")
                olemasLask = true;
        if(!olemasLask){
            tabel->lisaLabel[i]->hide();    //Kui ei ole ühtegi lisalasku, võib sildi ära peita
        }
    //}
    }
}

void QFinaalMain::print()
{
	QPrinter printer(QPrinter::HighResolution);
	printer.setOrientation(QPrinter::Landscape);
	QPrintDialog dialog(&printer, this);
	if(dialog.exec() == QDialog::Accepted){
		QPainter painter(&printer);
		int w = printer.pageRect().width();
		int h = printer.pageRect().height();
		QRect page(0, 0, w, h);
		
        painter.drawImage(page, *tulemus->resultsImage);
	}
}

void QFinaalMain::puhastaTulemuseaken()
{
    if(tulemus != 0){ //Miks see tingimus vajalik on?
        for(int i = 0; i < 8; i++){
            //*tulemus->summa[i] = "0";
            *tulemus->seeria[i] = "0";
            *tulemus->nimi[i] = "";
            //*tulemus->rada[i] = QString("%1").arg(i+1);
            *tulemus->rada[i] = QString(QChar(i + 65));
            *tulemus->lask[i] = "...";
            *tulemus->vahe[i] = "0";
            *tulemus->koht[i] = QString("%1.").arg(i+1);
        }
    }
#ifdef PROOV
            qDebug() << "puhastaTulemuseaken(): " << tulemus;
#endif
}

void QFinaalMain::import()
{
	QString failiNimi = QFileDialog::getOpenFileName(this, tr("Impordi"), "",
	tr("Finaal 1.0 fail (*.fin)")); //Vajab üle vaatamist (faili nimi?)
	if(failiNimi.isEmpty()) return;
	QFile fail(failiNimi);
	qApp->setOverrideCursor(Qt::WaitCursor);
	if(fail.open(QIODevice::ReadOnly)){
		QTextStream sisse(&fail);
		QStringList read = sisse.readLine().split(';');
		for(int i = 0; i < 8; i++){
			tabel->nimi[i]->setText(read[i]);
		}
		QString tul;
		for(int i = 0; i < 8; i++){
			sisse >> tul;
			tabel->tulemus[i]->setText(tul);
			tul = "";
		}
		for(int i = 0; i < 10; i++){
			for(int j = 0; j < 8; j++){
				sisse >> tul;
				tabel->lask[i][j]->setText(tul);
				tul = "";
			}
		}
		for(int i = 0; i < 8; i++){
			sisse >> tul;
			tabel->lisaLask[0][i]->setText(tul);
			tul = "";
		}
		sisse.readLine();
		tul = sisse.readLine();
		valik->ui.alaNimi->setText(tul);
		if(failiNimi.at(failiNimi.length() - 4) == '.')
		failiNimi.chop(QString(".fin").length());
		valik->ui.voistluseNimi->setText(failiNimi.section('/', -1));
		valik->ui.failiNimi->setText(failiNimi + ".fnl");
		
		this->setWindowTitle(programmiNimi + " - " + valik->ui.failiNimi->text());
	}else QMessageBox::critical(this, programmiNimi, tr("Ei õnnestu faili avada!"),
	QMessageBox::Ok);
	for(int i = 0; i < 8; i++)
	summ(i);
	jarjesta();
        QMessageBox::information(this, programmiNimi, tr("Fail imporditud!\nKuidas õnnestus, selles veenduge ise."), QMessageBox::Ok);
	aeg->start(50);
	qApp->restoreOverrideCursor();
}

void QFinaalMain::initTulemuseaken()
{
#ifdef PROOV
    qDebug() << "initTulemuseaken()";
#endif
    if(tulemus == 0)
        tulemus = new TulemuseAken;
    tulemus->setWindowTitle(programmiNimi + " tulemused");
    tulemus->setWindowIcon(QIcon(":/images/Finaal.ico"));
//    QMessageBox::information(this, programmiNimi, tr("Enne märklehtede initseerimist"), QMessageBox::Ok);

    maaraRelv();
    seaded->ui.relvaValik->addItems(tulemus->marklehed[0]->getLehetuubid());    //seadete aknasse lehtede valiku tekitamine

    for(int i = 0; i < 8; i++){
        tulemus->marklehed[i]->setNimi(tabel->nimi[i]->text());
        tulemus->marklehed[i]->setRada(QString("%1").arg(QChar(65+i)));
        tulemus->marklehed[i]->setTulemus(tabel->seeria[i]->text());
        tulemus->marklehed[i]->init(relv); //Määrab ära, mis tüüpi lehed on
    }
//    QMessageBox::information(this, programmiNimi, tr("Peale märklehtede initseerimist"), QMessageBox::Ok);
#ifdef PROOV
            qDebug() << "initTulemuseaken(), marklehed loodud";
#endif

    if(qApp->desktop()->numScreens() >= 2 && qApp->desktop()->isVirtualDesktop() && qApp->desktop()->screenNumber(this) == 0){
        tulemus->move(qApp->desktop()->screenGeometry(this).width() + 100, 100);
        if(qApp->desktop()->screenNumber(this) != qApp->desktop()->screenNumber(tulemus))
        tulemus->showFullScreen();
        if(tulemus->isFullScreen())
        QMessageBox::information(this, "Teade", QString::fromLatin1("Tulemuse aken näidatud teisel ekraanil"), QMessageBox::Ok);
    }else if(qApp->desktop()->numScreens() >= 2 && qApp->desktop()->isVirtualDesktop() && qApp->desktop()->screenNumber(this) == 1){
        tulemus->move(-1000, 100);
        if(qApp->desktop()->screenNumber(this) != qApp->desktop()->screenNumber(tulemus))
        tulemus->showFullScreen();
        else{
            tulemus->move(qApp->desktop()->screenGeometry(this).width() + 100, 100);
            if(qApp->desktop()->screenNumber(this) != qApp->desktop()->screenNumber(tulemus))
            tulemus->showFullScreen();
        }
        if(tulemus->isFullScreen())
        QMessageBox::information(this, "Teade", QString::fromLatin1("Tulemuse aken näidatud teisel ekraanil"), QMessageBox::Ok);
    }else if(QMessageBox::critical(this, "Viga", "Teist ekraani ei leitud. Programmi korralikuks"
    " funktsioneerimiseks on vajalik kahe ekraani olemasolu.\nJuhendi kuidas Windows'i "
    "seadistada leiate abi alt", QMessageBox::Ok | QMessageBox::Help) == QMessageBox::Help)
        aeg2->start(75);
}

void QFinaalMain::maaraRelv()
{
#ifdef PROOV
                qDebug() << "maaraRelv(), valik->ui.alaNimi->text() = " << valik->ui.alaNimi->text();
#endif
    relv = 0; //Vaikimisi on õhupüssi leht, 0 - õhupüss; 1 - õhupüstol; 2 - sportpüss
    if(valik->ui.alaNimi->text().contains(QString::fromLatin1("hupüss"))){
        relv = 0;
    }else if(valik->ui.alaNimi->text().contains(QString::fromLatin1("hupüstol"))){
        relv = 1;
    }else if(valik->ui.alaNimi->text().contains("tandard") || valik->ui.alaNimi->text().contains("amades")){
        relv = 2;
    }else{
        bool ok = false;
        QString leheValik;
        do{
            leheValik = QInputDialog::getItem(this, "Märklehe valik", QString::fromLatin1("Vali märklehe tüüp:"), Markleht::lehetuubid, relv, false, &ok);
        }while(!ok);
        relv = Markleht::lehetuubid.indexOf(leheValik);
        if(relv == -1){
            QMessageBox::critical(this, "QFinaal", "relv = -1", QMessageBox::Ok);
            relv = 0;
        }
//        if(leheValik.contains(QString::fromLatin1("hupüss"))){
//            relv = 0;
//        }else if(leheValik.contains(QString::fromLatin1("hupüstol"))){
//            relv = 1;
//        }else if(leheValik.contains(QString::fromLatin1("portpüss"))){
//            relv = 2;
//        }
    }
}

void QFinaalMain::naitaInfo()
{
        QMessageBox::about(this, programmiNimi, "Versioon " + versioon + "\n\n" + programmiNimi + QString::fromLatin1(" on vabavaraline "
	"programm, mis on loodud kasutamiseks laskevõistlustel finaali läbi viimiseks. Kasutamine "
	"omal vastutusel. Autor ei võta endale mingit vastutust kahjude eest, mis programmi "
	"kasutamisega võivad kaasneda!\n\nMuidu kasutage, seda nii kuidas ise heaks arvate. Välja "
	"arvatud programmi looja maine ning heaolu kahjustamise või rahalise kasu saamise eesmärkidel."
    "\n\nAutor: Ümeramees\nymeramees@gmail.com\nTallinn ") + aasta);
}

void QFinaalMain::naitaSeaded()
{
    //seaded->vanaDir = seaded->ui.lineEdit->text();
    seaded->vanaIndex = seaded->ui.comboBox->currentIndex();
    seaded->vanaLangemine = seaded->ui.valjalangemiseBox->isChecked();
    seaded->vanaMarklehtedeVahetamine = seaded->ui.marklehtedeVahetamiseBox->currentIndex();
    seaded->vanaMarklehtedeAeg = seaded->ui.marklehtedeAegBox->value();
    seaded->ui.relvaValik->setCurrentIndex(tulemus->marklehed[0]->getRelv());
    connect(seaded, SIGNAL(accepted()), this, SLOT(uuendaSeaded()));
    seaded->show();
}

void QFinaalMain::salvestaSeaded()
{
	QDir dir(qApp->applicationDirPath());
	if(!dir.cd("Data"))
	dir.mkdir("Data");
	QFile seadedF(qApp->applicationDirPath() + "/Data/Alg.ini");
	if(seadedF.open(QIODevice::WriteOnly | QIODevice::Text)){
		QTextStream valja(&seadedF);
		valja.setCodec("UTF-8");
		valja << programmiNimi + " seadete fail.\n";
        valja << tr("Kui olete midagi siin ära rikkunud, siis lihtsalt kustutage see fail"
		" ja programm teeb uue.\n");
		valja << valik->ui.failiNimi->text() << "\n" << seaded->ui.comboBox->currentIndex();
    }else QMessageBox::critical(this, "Viga", tr("Ei õnnestu seadete faili avada, seaded jäid salvestamata")
	, QMessageBox::Ok);
	seaded->close();
	/*QDir di(dir);
		di.cdUp();
		QFile fail(di.absolutePath() + "/Alg.ini");
		if(fail.open(QIODevice::WriteOnly | QIODevice::Text)){
		QTextStream valja(&fail);
		valja << programmiNimi + " seadete fail.\n";
		valja << "Kui olete midagi siin ära rikkunud, siis lihtsalt kustutage see fail"
		" ja programm teeb uue.\n";
		valja << valik->ui.voistluseNimi->text() << "\n" << valik->ui.alaNimi->text() << "\n"
		<< valik->ui.failiNimi->text() << "\n" << seaded->ui.lineEdit->text() << "\n"
		<< seaded->ui.comboBox->currentIndex();
		}else QMessageBox::critical(this, "Viga",
		"Ei õnnestu seadete faili avada, seaded jäid salvestamata", QMessageBox::Ok);
	seaded->close();*/
}

void QFinaalMain::naitaWAbi()
{
	QPixmap *pildike = new QPixmap(":/images/Abi.bmp");
	QLabel *pilt = new QLabel();
	pilt->setPixmap(*pildike);
	pilt->setGeometry(pildike->rect());
	pilt->setWindowModality(Qt::ApplicationModal);
	pilt->show();
}

void QFinaalMain::vahemallu()
{
	QString tekst;
	for(int i = 0; i < 8; i++)
	if(!tabel->nimi[i]->text().isEmpty()){
		tekst.append(tabel->nimi[i]->text() + "\t");
		tekst.append(tabel->tulemus[i]->text());
        for(int j = 0; j < 24; j++)
		tekst.append("\t" + tabel->lask[j][i]->text());
		tekst.append("\n");
	}
	QClipboard *vahemalu = QApplication::clipboard();
	vahemalu->setText(tekst);
}

void QFinaalMain::kaivitaServer()
{
	if(server == 0){
		server = new QTcpServer(this);
		if (!server->listen()) {
			QMessageBox::critical(this, tr("Finaal"), tr("Unable to start the server: %1.")
			.arg(server->errorString()));
			return;
		}
		ipAadress = server->serverAddress().toString();
		toolBar->addAction(loeSiiaAct);
		toolBar->addAction(loeVorkuAct);
		loeSiiaAct->setChecked(true);
		statusBar()->showMessage(tr("Server käivitatud, ip: %1").arg(ipAadress), 2000);
		connect(server, SIGNAL(newConnection()), this, SLOT(uusUhendus()));
	}else QMessageBox::information(this, "Finaal", tr("Server töötab, \naadress: %1\nport: %2")
	.arg(server->serverAddress().toString()).arg(server->serverPort()), QMessageBox::Ok);
}

void QFinaalMain::kasKohal()
{
	mituKohal = 0;
	for(int i = 0; i < 8; i++){
		if(!tabel->kohal[i]->isChecked()){
			tabel->nimi[i]->setEnabled(false);
			tabel->tulemus[i]->setEnabled(false);
            for(int k = 0; k < 24; k++)
			tabel->lask[k][i]->setEnabled(false);
            tulemus->marklehed[i]->setAktiivne(false);
			}else{
			mituKohal++;
			tabel->nimi[i]->setEnabled(true);
			tabel->tulemus[i]->setEnabled(true);
            for(int k = 0; k < 24; k++)
			tabel->lask[k][i]->setEnabled(true);
            tulemus->marklehed[i]->setAktiivne(true);
		}
	}
    summ0();    //Pärast on vaja kõik summeerida, et ekraanile uuendus ilmuks
    summ1();
    summ2();
    summ3();
    summ4();
    summ5();
    summ6();
    summ7();
}

void QFinaalMain::kustutaLasud()
{
    for(int i = 0; i < 8; i++){
        tabel->seeria1[i]->setText("0,0");
        tabel->seeria2[i]->setText("0,0");
        tabel->seeria[i]->setText("0,0");
        tabel->iSumma[i] = 0;
        for(int j = 0; j < 24; j++){
            tabel->lask[j][i]->setText("00");
            tabel->lisaLask[j][i]->setText("00");
            tabel->lisaLask[j][i]->hide();
        }
        tabel->lisaLabel[i]->hide();
    }
    puhastaTulemuseaken();
}

void QFinaalMain::lehelugeja()
{
    if(logi->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){
        QTextStream logiValja(logi);
        logiValja << QString::fromLatin1("--Lehtede lugemine masinaga--") << "\n";
    }
    //    qDebug() << "Lehelugeja()";
    if(lugemisMasin == 0){
//          serial = new QextSerialPort();
        lugemisMasin = new LugemisMasin(this);
		//        qDebug() << "QextSerialPort";
        lugeja = new Lehelugeja(lugemisMasin, valik->ui.alaNimi->text(),this);
		//        qDebug() << "new Lehelugeja";
//        connect(lugemisMasin, SIGNAL(portSuletud()), this, SLOT(portSuletud()));
        connect(lugemisMasin, SIGNAL(laskLoetud(Lask)), this, SLOT(loeLask(Lask)));
        connect(lugemisMasin, SIGNAL(teadeLogisse(QString)), lugeja, SLOT(lisaLogisse(QString)));
//		serial->close();
	}
//	timer->start();
	lugeja->exec();
    for(int i = 0; i < 8; i++)
        tulemus->marklehed[i]->setRelv(lugemisMasin->getRelv());
}

void QFinaalMain::loeLask(Lask l)
{
    static int mitmesKord = 0;  //Muutmaks automaatselt seda, kas loetakse siia või võrku

    if(server != 0 && mitmesKord >= mituKohal && loeSiiaAct->isChecked()){  //Kui siinsete laskurite tulemused on loetud, lülitub võrku lugemise peale
        loeVorku();
        mitmesKord = 0;
    }
    else if(server != 0 && mitmesKord >= 16 && loeVorkuAct->isChecked()){
        loeSiia();
        mitmesKord = 0;
    }

    if(server == 0 || loeSiiaAct->isChecked()){
        for(int i = 0; i < 8; i++)
            for(int j = 0; j < 24; j++)
                if(tabel->lask[j][i]->hasFocus()){
                    tabel->lask[j][i]->setText(l.getSLask());
//                    tulemus->marklehed[i]->joonistaLask(l);
                    mitmesKord++;
                    if(logi->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){
                        QTextStream logiValja(logi);
                        logiValja << "laskur " << i  << " lask " << j << " = " << l.getSLask() << "\n";
                    }
                    vajutaTab();
                    seadistaja->start();
                    //#ifdef PROOV
                    if(seaded->ui.marklehtedeVahetamiseBox->currentIndex() == 0){
                        if(tulemus->marklehed[i]->isHidden())   //Algul ei ole märklehti näha
                            tulemus->naitaLehed();  //Puhastab ja toob märklehed nähtavale
                        tulemus->peidaNimed();
                        lehtedePeitja->start(); //Lehed peale aja möödumist uuesti peitu, et nimed näha oleks
                    }
                    //                        summ(j);    //Peab kontrollima, ega see siin midagi ära ei riku
                    //                        tulemus->marklehed[j]->setTulemus(tabel->seeria[j]->text());
                    tulemus->marklehed[i]->joonistaLask(l);
                    //#endif

                    return;
                }else if(tabel->lisaLask[j][i]->hasFocus()){
                    tabel->lisaLask[j][i]->setText(l.getSLask());
//                    tulemus->marklehed[i]->joonistaLask(l);
                    mitmesKord++;
                    if(logi->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){
                        QTextStream logiValja(logi);
                        logiValja << "laskur " << i  << " lisalask " << j << " = " << l.getSLask() << "\n";
                    }
                    vajutaTab();
                    seadistaja->start();
                    //#ifdef PROOV
                    if(seaded->ui.marklehtedeVahetamiseBox->currentIndex() == 0){
                        if(tulemus->marklehed[i]->isHidden())   //Algul ei ole märklehti näha
                            tulemus->naitaLehed();  //Puhastab ja toob märklehed nähtavale
                        tulemus->peidaNimed();
                        lehtedePeitja->start(); //Lehed peale aja möödumist uuesti peitu, et nimed näha oleks
                    }
                    //                        summ(j);    //Peab kontrollima, ega see siin midagi ära ei riku
                    //                        tulemus->marklehed[j]->setTulemus(tabel->seeria[j]->text());
                    tulemus->marklehed[i]->joonistaLask(l);
                    //#endif

                    return;
                }
    }else{
        saadaLask(l.getSLask());
    }
    logi->close();
}

//void QFinaalMain::loeLehti()
//{
//    if(serial == 0) return;
//    if(serial->bytesAvailable()>0){
//        static QString buffer;
//        QString tekst;
//        //char buffer[256];
//        //int rec = serial->readLine(buffer, 255);
//        //buffer[rec]='\0';
//        buffer.append(serial->readAll());
//        if(buffer.contains(13)){
//            tekst = buffer.left(buffer.indexOf(13) + 1);
//            buffer.remove(0, buffer.indexOf(13) + 1);
//        }else return;

//        lugeja->lisaLogisse("vastuvõtt:");
//        //char buffer[256];
//        //int rec = serial->read(buffer, 255);
//        //buffer[rec]='\0';
//        //QString tekst(buffer);
//        lugeja->lisaLogisse(tekst);
//        if(logi->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){
//            QTextStream logiValja(logi);
//            logiValja << tekst << "\n";
//        }

//        static int mitmesKord = 0;  //Muutmaks automaatselt seda, kas loetakse siia või võrku

//        if(!tekst.contains("START") && !tekst.contains("SCHEIBE") && !tekst.contains("Keine") && tekst.contains(';')){
//            QStringList list = tekst.split(';', QString::KeepEmptyParts);
//            if(list.size() < 5){
//                if(logi->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){
//                    QTextStream logiValja(logi);
//                    logiValja << QString::fromLatin1("See rida oli liiga lühike!") << "\n";
//                }
//                lugeja->lisaLogisse(QString::fromLatin1("See rida oli liiga lühike!"));
//                return;
//            }
//            lugeja->lisaLogisse(QString("list.count() = %1").arg(list.count()));
//            lugeja->lisaLogisse(list.at(1));

//            if(list.count() > 6){
//                if(logi->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){
//                    QTextStream logiValja(logi);
//                    logiValja << "Laskude arv lehes liiga suur: " << list.count() << "\n";
//                }
//                QMessageBox::critical(this, "Finaal", "Finaalis peab laskude arv lehes olema 1, mitte rohkem!", "OK");
//            }
//            if(server != 0 && mitmesKord >= mituKohal && loeSiiaAct->isChecked()){  //Kui siinsete laskurite tulemused on loetud, lülitub võrku lugemise peale
//                loeVorku();
//                mitmesKord = 0;
//            }
//            else if(server != 0 && mitmesKord >= 16 && loeVorkuAct->isChecked()){
//                loeSiia();
//                mitmesKord = 0;
//            }

//            if(server == 0 || loeSiiaAct->isChecked()){
//                for(int i = 0; i < 8; i++)
//                    for(int j = 0; j < 24; j++)
//                        if(tabel->lask[j][i]->hasFocus()){
//                            tabel->lask[j][i]->setText(list.at(1));
//                            mitmesKord++;
//                            if(logi->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){
//                                QTextStream logiValja(logi);
//                                logiValja << "laskur " << i  << " lask " << j << " = " << list.at(1) << "\n";
//                            }
//					/*int lisa = i;
//						int lisa2 = i + 1;
//						if(lisa2 >= 8) lisa2 = 0;
//						if(!tabel->kohal[lisa2]->isChecked()){
//						do{
//						lisa++;
//						vajutaTab();
//						if(lisa >= 8) lisa = 0;
//						}while(!tabel->kohal[lisa]->isChecked() && lisa != i);
//						}else{
//						vajutaTab();
//						return;
//					}*/
//                            vajutaTab();
//                            return;
//                        }else if(tabel->lisaLask[j][i]->hasFocus()){
//                            tabel->lisaLask[j][i]->setText(list.at(1));
//                            mitmesKord++;
//                            if(logi->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){
//                                QTextStream logiValja(logi);
//                                logiValja << "laskur " << i  << " lisalask " << j << " = " << list.at(1) << "\n";
//                            }
//                            vajutaTab();
//                            return;
//                        }
//            }else{
//                saadaLask(list.at(1));
//            }
//        }else if(tekst.contains("ENDE")){
//            seadistaja->start();
//        }
//        logi->close();
//    }
//}

void QFinaalMain::loeSiia()
{
	loeSiiaAct->setChecked(true);
	loeVorkuAct->setChecked(false);
}

void QFinaalMain::loeSiusDatast()
{
#ifdef PROOV
        qDebug() << "loeSiusDatast()";
#endif
    if(siusDataSocket == 0){
        QMessageBox::critical(this, "Viga", "Ühendus SiusData'ga on loomata!", QMessageBox::Ok);
        return;
    }
    if(siusDataSocket->bytesAvailable() > 5){  //Kui on liiga vähe infot, ei ole mõtet lugeda
        while(siusDataSocket->bytesAvailable() > 2){
            QCoreApplication::processEvents();
//        QDataStream in(siusDataSocket);
        QString info;
        info.append(siusDataSocket->readLine());  //Rea algus 5f, lõpp 0d 0a
        statusBar()->showMessage(tr("Saabus info: %1").arg(info), 2000);
//        in >> info;
#ifdef PROOV
        qDebug() << "Saabus info: " << info;
#endif
//    }
//    QFile file("siusData.txt");
//    if (file.open(QFile::ReadOnly)){
//        siusDatast = file.readAll();
//        QString rida;
#ifndef PROOV
        if(!siusLogi->isOpen()) //Kui on juba avatud, ei ole vaja uuesti avada
        if(siusLogi->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){ //Saabunud võrguliikluse logi
            QTextStream valja(siusLogi);
//            valja << siusDatast;
            valja << info;
            siusLogi->close();
        }
#endif
        logi->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append); //Saabunud muudatuste ja laskude logifail
        QTextStream logiValja(logi);

//        if(siusDatast.indexOf(13, 0) == -1) return;    //Ei ole terve rida kohale jõudnud
        if(!info.contains((QChar)10)) return;  //Rea lõppu ei ole, järelikult ei ole terve rida kohale jõudnud

//        do{ //Kõigepealt on vaja leida failist koht, kust algab laskurite info
//            rida = siusDatast.left(siusDatast.indexOf(13, 1));
//            siusDatast.remove(0, siusDatast.indexOf(13, 1) + 2);
//            logiValja << rida << "\n";
//#ifdef PROOV
//        qDebug() << "Teine, indexOf(13, 1): " << siusDatast.indexOf(13, 1) << "rida: " << rida;
//#endif
//        }while(!rida.startsWith("_NAME") && siusDatast.indexOf(13, 1) != -1);

        static int leitud = 0; //Laskurite arv, kellele on ID leitud ja lisatud

        if(info.startsWith("_NAME")){
#ifdef PROOV
        qDebug() << "Nimed";
#endif
//        for(int i = 0; i < 8; i++){ //Seejärel on vaja sisse lugeda laskurite ID'd
            QStringList read = info.split(';');
            for(int j = 0; j < 8; j++){
                if(read[5].left(tabel->nimi[j]->text().length() - 1) == tabel->nimi[j]->text().replace("ð", "s").replace("þ", "z").replace("Ð", "S").replace("Þ", "Z") || read[5].left(tabel->nimi[j]->text().length() - 1) == tabel->nimi[j]->text().remove('.')){   //Nime kontroll, asendades katusega tähed ilma vastu
#ifdef PROOV
        qDebug() << "Nimi leitud" << tabel->nimi[j]->text();
#endif
                    if(!tabel->id[j].isEmpty() && tabel->id[j] != read[3]){
                        if(QMessageBox::warning(this, "Hoiatus", QString::fromLatin1("Laskuril %1 on juba teistsugune ID olemas!\n\nKas soovite "
                                "olemasoleva ID (%2) asendada uuega (%3)?\n\nKui te seda ei muuda, ei tule Sius'ist ka lasud kohale!\n\nKui teate, et ID on õige ja peale põhilaskmist\nei ole siusDatat kinni pandud, vajutage Cancel")
                                .arg(tabel->nimi[j]->text()).arg(tabel->id[j]).arg(read[3]), QMessageBox::Ok | QMessageBox::Cancel)
                                == QMessageBox::Ok){
                            tabel->id[j] = read[3];
                            logiValja << "#ID muutus: " << tabel->nimi[j]->text() << "uus ID: " << tabel->id[j] << "\n";
                        }
                    }else{
                        tabel->id[j] = read[3];
                        logiValja << "#ID muutus: " << tabel->nimi[j]->text() << "uus ID: " << tabel->id[j] << "\n";
                    }
                    leitud++;
                }
            }
//            rida = siusDatast.left(siusDatast.indexOf(13, 1));
//            siusDatast.remove(0, siusDatast.indexOf(13, 1) + 2);
//            logiValja << info << "\n";
        }
//#ifdef PROOV
//        qDebug() << "ID'd loetud: " << tabel->id[0] << ", " << tabel->id[1] << ", " << tabel->id[2] << ", " << tabel->id[3] << ", " << tabel->id[4]
//                 << ", " << tabel->id[5] << ", " << tabel->id[6] << ", " << tabel->id[7];
//#endif

//        while(!voistlus/*!voistlus[0] || !voistlus[1] || !voistlus[2] || !voistlus[3] || !voistlus[4] || !voistlus[5] || !voistlus[6] || !voistlus[7]*/){ 	//Proovilaskude lugemine, kuni esimese võistluslasuni

//            rida.clear();
//            rida = siusDatast.left(siusDatast.indexOf(13, 1));
//            siusDatast.remove(0, siusDatast.indexOf(13, 1) + 2);
//            logiValja << rida << "\n";

//#ifdef PROOV
//        qDebug() << "Rida: " << rida;
//        qDebug() << "Eelminerida[0]: " << eelmineRida[0];
//#endif
        else if(info.startsWith("_SHOT") && !voistlus){   //Lasu rida, proovilasud
            static bool idVigaNaidatud = false; //Et errorit näidataks ainult üks kord
            if(leitud != 8 && !idVigaNaidatud){
                //QMessageBox::warning(this, "Viga", "ID'de lisamisega on probleem - ei lisatud 8 ID'd.", QMessageBox::Ok); //See praegu ei tööta ja annab asjatuid veateateid
                logiValja << "#Ei leitud 8 ID'd\n";
                idVigaNaidatud = true;
            }
            QStringList read = info.split(';');
            for(int j = 0; j < 8; j++){
                if(read[3] == tabel->id[j]){
                    eelmineRida[j] = info;
                    logiValja << "#" << read[3] << ": Proovilask\n";

                    Lask lask;
                    if(read[11].toInt() == 0){  //Kui [11] on 0, siis järelikult loetakse komakohtadega ja lasu väärtus on [10]'s, kui ei ole 0, siis loetakse täisarvudega
                        lask.set10Lask(read[10]);
                    }else lask.set10Lask(read[11]);
                    bool onnestus = false;
                    float f = 0;
                    f = read[14].toFloat(&onnestus);
                    if(!onnestus){
                        if(read[14].contains(',')){
                            read[14].replace(',', '.');
                        }else if(read[14].contains('.')){
                            read[14].replace('.', ',');
                        }
                        f = read[14].toFloat(&onnestus);
                    }
                    f *= 1000; //Tundub, et Sius'i koordinaadid on meetrites, vaja teha millimeetrid
                    lask.setX(f);
                    onnestus = false;
                    f = 0;
                    f = read[15].toFloat(&onnestus);
                    if(!onnestus){
                        if(read[15].contains(',')){
                            read[15].replace(',', '.');
                        }else if(read[15].contains('.')){
                            read[15].replace('.', ',');
                        }
                        f = read[15].toFloat(&onnestus);
                    }
                    f *= 1000; //Tundub, et Sius'i koordinaadid on meetrites, vaja teha millimeetrid
                    lask.setY(f);

                    if(tulemus->marklehed[j]->isHidden())   //Algul ei ole märklehti näha
                        tulemus->naitaLehed();  //Puhastab ja toob märklehed nähtavale, proovilaskude ajal on need koos nimedega
//                    tulemus->peidaNimed();
//                    tulemus->marklehed[j]->setTulemus(tabel->seeria[j]->text());
                    tulemus->marklehed[j]->joonistaLask(lask);
                    lehtedePeitja->setInterval(seaded->ui.marklehtedeAegBox->value()*1000);
                    if(seaded->ui.marklehtedeVahetamiseBox->currentIndex() == 0)
                        lehtedePeitja->start(); //Lehed peale aja möödumist uuesti peitu, et nimed näha oleks
#ifdef PROOV
        qDebug() << "Read[3]: " << read[3] << ", rida: " << info;
        //qDebug() << "Eelminerida[0]: " << eelmineRida[0];
#endif
                }
            }
        }else if(info.startsWith("_TOTL") && !voistlus){   //Tulemuse rida
            QStringList read = info.split(';');

            if(read.count() <=12) return;   //Kui rida ei olnud piisavalt pikk, on tegu ilmselt kogusummaga, mis meid ei huvita
            if(read[12] != "0"){   //Kui tegu on proovilasuga, on see 0, kui võistluslasuga, siis on seal seeria summa
                for(int j = 0; j < 8; j++){ //Kuna eelmine rida oli juba esimene võistluslask, on vaja see kellelegi lisada
//#ifdef PROOV
//        qDebug() << "Rida: " << rida;
//        qDebug() << "Read[3]: " << read[3] << " tabel.id[" << j << "]: " << tabel->id[j];
//        qDebug() << "Eelminerida[" << j << "]: " << eelmineRida[j];
//#endif
                    if(read[3] == tabel->id[j] && eelmineRida[j].startsWith("_SHOT")){
                        voistlus = true;    //Edasi tulevad selle vahetuse võistluslasud
                        lehtedePeitja->stop();  //Kuna joonistatakse esimene võistluslask, siis ei tohi lehed peitu minna

                        tulemus->puhastaLehed();    //Lehed tuleb proovilaskudest puhastada

                        logiValja << "#read[6] = " << read[12] << " - algavad võistluslasud\n\n";

                        QStringList eelmisedRead = eelmineRida[j].split(';');
                        Lask lask;
                        if(eelmisedRead[11].toInt() == 0){  //Kui [11] on 0, siis järelikult loetakse komakohtadega ja lasu väärtus on [10]'s, kui ei ole 0, siis loetakse täisarvudega
                            lask.set10Lask(eelmisedRead[10]);
                        }else lask.set10Lask(eelmisedRead[11]);
                        bool onnestus = false;
                        float f = 0;
                        f = eelmisedRead[14].toFloat(&onnestus);
                        if(!onnestus){
                            if(eelmisedRead[14].contains(',')){
                                eelmisedRead[14].replace(',', '.');
                            }else if(eelmisedRead[14].contains('.')){
                                eelmisedRead[14].replace('.', ',');
                            }
                            f = eelmisedRead[14].toFloat(&onnestus);
                        }
                        f *= 1000; //Tundub, et Sius'i koordinaadid on meetrites, vaja teha millimeetrid
                        lask.setX(f);
                        onnestus = false;
                        f = 0;
                        f = eelmisedRead[15].toFloat(&onnestus);
                        if(!onnestus){
                            if(eelmisedRead[15].contains(',')){
                                eelmisedRead[15].replace(',', '.');
                            }else if(eelmisedRead[15].contains('.')){
                                eelmisedRead[15].replace('.', ',');
                            }
                            f = eelmisedRead[15].toFloat(&onnestus);
                        }
                        f *= 1000; //Tundub, et Sius'i koordinaadid on meetrites, vaja teha millimeetrid
                        lask.setY(f);

                        if(seaded->ui.marklehtedeVahetamiseBox->currentIndex() == 0){
                            if(tulemus->marklehed[j]->isHidden())   //Algul ei ole märklehti näha
                                tulemus->naitaLehed();  //Toob märklehed nähtavale
                            tulemus->peidaNimed();
                        }
    //                        summ(j);    //Peab kontrollima, ega see siin midagi ära ei riku
    //                        tulemus->marklehed[j]->setTulemus(tabel->seeria[j]->text());
                        tulemus->marklehed[j]->joonistaLask(lask);
//                        lehtedePeitja->start(); //Lehed peale aja möödumist uuesti peitu, et nimed näha oleks

                        tabel->lask[0][j]->setText(lask.getSLask());
//                        tabel->lask[0][j]->setText(tabel->lask[0][j]->text().insert(tabel->lask[0][j]->text().length()-1, ','));
                        logiValja << "#" << tabel->nimi[j]->text() << " lask 1 = " << tabel->lask[0][j]->text() << "\n";
#ifdef PROOV
        qDebug() << "read[6]: " << read[12] << "voistlus = true" << "eelmisedRead[10]: " << eelmisedRead[10] << "\n\n";
#endif
                    }
                }
            }
        }
//        if(siusDatast.isEmpty() || rida.isEmpty()){
//#ifdef PROOV
//        qDebug() << "break;";
//#endif
//                break;
//        }
//        }

//        while(!siusDatast.isEmpty() && !rida.isEmpty()){ //Ülejäänud võistluslaskude lugemine
//            rida.clear();
//            rida = siusDatast.left(siusDatast.indexOf(13, 1));
//            siusDatast.remove(0, siusDatast.indexOf(13, 1) + 2);

//            logiValja << info << "\n";

        else if(info.startsWith("_SHOT") && voistlus){   //Võistluslasu rida
            QStringList read = info.split(';');
            for(int j = 0; j < 8; j++){
                if(read[3] == tabel->id[j]){

                    int lisaLaske = 0;
                    for(int i = 0; i < 24; i+=2)
                        if(tabel->lisaLask[i][j]->text() != "00")
                            lisaLaske++;

                    int lasuNr = read[13].toInt() - 1 - lisaLaske;  //Siusist tulnud lasu Nr, koodi lühendamise eesmärgil
                    Lask lask;
                    if(read[11].toInt() == 0){  //Kui [11] on 0, siis järelikult loetakse komakohtadega ja lasu väärtus on [10]'s, kui ei ole 0, siis loetakse täisarvudega
                        lask.set10Lask(read[10]);
                    }else lask.set10Lask(read[11]);
                    bool onnestus = false;
                    float f = 0;
                    f = read[14].toFloat(&onnestus);
                    if(!onnestus){
                        if(read[14].contains(',')){
                            read[14].replace(',', '.');
                        }else if(read[14].contains('.')){
                            read[14].replace('.', ',');
                        }
                        f = read[14].toFloat(&onnestus);
                    }
                    f *= 1000; //Tundub, et Sius'i koordinaadid on meetrites, vaja teha millimeetrid
                    lask.setX(f);
                    onnestus = false;
                    f = 0;
                    f = read[15].toFloat(&onnestus);
                    if(!onnestus){
                        if(read[15].contains(',')){
                            read[15].replace(',', '.');
                        }else if(read[15].contains('.')){
                            read[15].replace('.', ',');
                        }
                        f = read[15].toFloat(&onnestus);
                    }
                    f *= 1000; //Tundub, et Sius'i koordinaadid on meetrites, vaja teha millimeetrid
                    lask.setY(f);

                    uuendaLasuNrit();   //Vaja ka uuendada, mitmes lask käsil on, enne, kui uus lask lisatakse
#ifdef PROOV
        qDebug() << "loeSiusDatast(), lasuNr = " << lasuNr << ", tulemus->mitmesLask = " << tulemus->mitmesLask;
#endif

                    if((lasuNr == 5 && tulemus->mitmesLask == 5) || (lasuNr == 10 && tulemus->mitmesLask == 10) || (lasuNr == 12 && tulemus->mitmesLask == 12) || (lasuNr == 14 && tulemus->mitmesLask == 14) || (lasuNr == 16 && tulemus->mitmesLask == 16) || (lasuNr == 18 && tulemus->mitmesLask == 18) || (lasuNr == 20 && tulemus->mitmesLask == 20) || (lasuNr == 22 && tulemus->mitmesLask == 22) || (lasuNr == 24 && tulemus->mitmesLask == 24)){ //Peale esimest seeriat on vaja lehed puhastada
                        tulemus->puhastaLehed();
                    }

                    if(seaded->ui.marklehtedeVahetamiseBox->currentIndex() == 0){
                        if(tulemus->marklehed[j]->isHidden())   //Algul ei ole märklehti näha
                            tulemus->naitaLehed();  //Toob märklehed nähtavale
                        tulemus->peidaNimed();
                    }
//                        summ(j);    //Peab kontrollima, ega see siin midagi ära ei riku
//                        tulemus->marklehed[j]->setTulemus(tabel->seeria[j]->text());
                    tulemus->marklehed[j]->joonistaLask(lask);

                    //Kontroll, et laskude tabel lõhki ei läheks, samas viimase lasu järel võib tulla lisalask
                    if(lasuNr <= 24 || ((lasuNr + 1) <= 24 && tabel->lisaLask[12][j]->isVisible())){
                        //Kui mõni lisalasu kast on nähtav, aga tekst on "00", siis ilmselt on tegu lisalasuga
                        if(lasuNr == 12 && tabel->lisaLask[0][j]->isVisible() && tabel->lisaLask[0][j]->text() == "00"){
                            tabel->lisaLask[0][j]->setText(read[10]);
                            tabel->lisaLask[0][j]->setText(tabel->lisaLask[0][j]->text().insert(tabel->lisaLask[0][j]->text().length()-1, ','));
                            logiValja << "#" << tabel->nimi[j]->text() << " lisa lask " << 1 << " = " << tabel->lisaLask[0][j]->text() << "\n";
                        }else if(lasuNr == 14 && tabel->lisaLask[2][j]->isVisible() && tabel->lisaLask[2][j]->text() == "00"){
                            tabel->lisaLask[2][j]->setText(read[10]);
                            tabel->lisaLask[2][j]->setText(tabel->lisaLask[2][j]->text().insert(tabel->lisaLask[2][j]->text().length()-1, ','));
                            logiValja << "#" << tabel->nimi[j]->text() << " lisa lask " << 3 << " = " << tabel->lisaLask[2][j]->text() << "\n";
                        }else if(lasuNr == 16 && tabel->lisaLask[4][j]->isVisible() && tabel->lisaLask[4][j]->text() == "00"){
                            tabel->lisaLask[4][j]->setText(read[10]);
                            tabel->lisaLask[4][j]->setText(tabel->lisaLask[4][j]->text().insert(tabel->lisaLask[4][j]->text().length()-1, ','));
                            logiValja << "#" << tabel->nimi[j]->text() << " lisa lask " << 5 << " = " << tabel->lisaLask[4][j]->text() << "\n";
                        }else if(lasuNr == 18 && tabel->lisaLask[6][j]->isVisible() && tabel->lisaLask[6][j]->text() == "00"){
                            tabel->lisaLask[6][j]->setText(read[10]);
                            tabel->lisaLask[6][j]->setText(tabel->lisaLask[6][j]->text().insert(tabel->lisaLask[6][j]->text().length()-1, ','));
                            logiValja << "#" << tabel->nimi[j]->text() << " lisa lask " << 7 << " = " << tabel->lisaLask[6][j]->text() << "\n";
                        }else if(lasuNr == 20 && tabel->lisaLask[8][j]->isVisible() && tabel->lisaLask[8][j]->text() == "00"){
                            tabel->lisaLask[8][j]->setText(read[10]);
                            tabel->lisaLask[8][j]->setText(tabel->lisaLask[8][j]->text().insert(tabel->lisaLask[8][j]->text().length()-1, ','));
                            logiValja << "#" << tabel->nimi[j]->text() << " lisa lask " << 9 << " = " << tabel->lisaLask[8][j]->text() << "\n";
                        }else if(lasuNr == 22 && tabel->lisaLask[10][j]->isVisible() && tabel->lisaLask[10][j]->text() == "00"){
                            tabel->lisaLask[10][j]->setText(read[10]);
                            tabel->lisaLask[10][j]->setText(tabel->lisaLask[10][j]->text().insert(tabel->lisaLask[10][j]->text().length()-1, ','));
                            logiValja << "#" << tabel->nimi[j]->text() << " lisa lask " << 11 << " = " << tabel->lisaLask[10][j]->text() << "\n";
                        }else if(lasuNr == 24 && tabel->lisaLask[12][j]->isVisible() && tabel->lisaLask[12][j]->text() == "00"){
                            tabel->lisaLask[12][j]->setText(read[10]);
                            tabel->lisaLask[12][j]->setText(tabel->lisaLask[12][j]->text().insert(tabel->lisaLask[12][j]->text().length()-1, ','));
                            logiValja << "#" << tabel->nimi[j]->text() << " lisa lask " << 13 << " = " << tabel->lisaLask[12][j]->text() << "\n";
                        }else{
                            tabel->lask[lasuNr][j]->setText(lask.getSLask());
//                            tabel->lask[lasuNr][j]->setText(tabel->lask[lasuNr][j]->text().insert(tabel->lask[lasuNr][j]->text().length()-1, ','));
                            logiValja << "#" << tabel->nimi[j]->text() << " lask " << lasuNr+1 << " = " << tabel->lask[lasuNr][j]->text() << "\n";
                        }
                    }else if((lasuNr) > 24){
                        QMessageBox::critical(this, "Viga", "Laskude arv suurem, kui võimalik! Lask ei läinud kirja!", QMessageBox::Ok);
                        logiValja << "#Laskude arv suurem, kui võimalik! Lask ei läinud kirja!\n\n";
                    }
                    break;  //Ei ole mõtet ülejäänud laskureid enam kontrollida
                }
            }
        }
//        }
//        QMessageBox::information(this, "SiusData", "Saabus info:\n" + info, QMessageBox::Ok);
//        QMessageBox::information(this, "SiusData", "Saabus info:\n" + rida, QMessageBox::Ok);
        logi->close();
        }
#ifdef PROOV
        qDebug() << "loeSiusDatast(), summ(i)";
#endif
        for(int i = 0; i < 8; i++){ //Pärast on vaja kõik summeerida, et tulemused ekraanile ilmuks - see teeb funktsiooni jube aeglaseks, aga võib summeerida peale kõigi laskude lugemist
            summ(i);
            tulemus->marklehed[i]->setTulemus(tabel->seeria[i]->text());
            tulemus->marklehed[i]->zoom();
        }
        if(siusDataSocket->bytesAvailable() > 5){   //Kui puhver ei ole tühi, aga millegipärast tuli loopist välja, siis käivitada funktsioon uuesti mõne aja pärast
            //logiValja << "#buffer.length(): " << siusiBuffer.length() << ", uuele ringile minek" << "\n";
#ifdef PROOV
            QMessageBox::information(this, "Finaal", "Uuele ringile minek", QMessageBox::Ok);
#endif
            QTimer::singleShot(100, this, SLOT(loeSiusDatast()));
        }
    }
}

void QFinaalMain::loeVorgust()
{
	QDataStream in(socket);
	in.setVersion(QDataStream::Qt_4_7);
	
	if (blockSize == 0) {
		if (socket->bytesAvailable() < (int)sizeof(quint16))
		return;
		
		in >> blockSize;
	}
	
	if (socket->bytesAvailable() < blockSize)
	return;
	
	QString lask;
	in >> lask;
	
	if(lask == "Tere"){
        QMessageBox::information(this, "Finaal", tr("Ühendus loodud"), QMessageBox::Ok);
		return;
	}
	
	for(int i = 0; i < 8; i++)
    for(int j = 0; j < 24; j++)
	if(tabel->lask[j][i]->hasFocus()){
		tabel->lask[j][i]->setText(lask);
		int lisa = i;
		int lisa2 = i + 1;
		if(lisa2 >= 8) lisa2 = 0;
		if(!tabel->kohal[lisa2]->isChecked()){
			do{
				lisa++;
				vajutaTab();
				if(lisa >= 8) lisa = 0;
			}while(!tabel->kohal[lisa]->isChecked() && lisa != i);
			}else{
			vajutaTab();
			return;
		}
		}else if(tabel->lisaLask[j][i]->hasFocus()){
		tabel->lisaLask[j][i]->setText(lask);
		vajutaTab();
		return;
	}
}

void QFinaalMain::loeVorku()
{
	loeSiiaAct->setChecked(false);
	loeVorkuAct->setChecked(true);
}

//void QFinaalMain::portSuletud()
//{
//	timer->stop();
//}

void QFinaalMain::saadaLask(QString lask)
{
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	
	out << (quint16)0;
	out << lask;
	out.device()->seek(0);
	out << (quint16)(block.size() - sizeof(quint16));
	
	socket = server->nextPendingConnection();
	socket->write(block);
}

void QFinaalMain::seadistaLehelugemismasin()
{
    if(lugeja != 0){
        if(server != 0 && loeVorkuAct->isChecked())
            lugeja->seadistaVorgule();
        else lugeja->seadista();
    }
#ifdef PROOV
    qDebug() << "QFinaalMain::Seadistamine";
#endif
}

void QFinaalMain::salvesta()
{
#ifdef PROOV
        qDebug() << "salvesta();";
#endif
    if(valik->ui.voistluseNimi->text().isEmpty() || valik->ui.alaNimi->text().isEmpty()){
        QString uusNimi = QInputDialog::getText(this, "Sisestage võistluse nimi", "Võistluse nimi:",
        QLineEdit::Normal, valik->ui.voistluseNimi->text());
        QString uusHarjutus = QInputDialog::getText(this, "Sisestage harjutuse nimi", "Harjutus:",
        QLineEdit::Normal, valik->ui.alaNimi->text());
        if(uusNimi.isEmpty() || uusHarjutus.isEmpty())
        return;
        valik->ui.voistluseNimi->setText(uusNimi);
        valik->ui.alaNimi->setText(uusHarjutus);
    }

    /*QDir di(dir);
        di.cdUp();
        if(seaded->ui.lineEdit->text().isEmpty()){
        if(!di.cd("Tulemused"))
        di.mkdir("Tulemused");
        di.cd("Tulemused");
        }else{
        if(!di.cd(seaded->ui.lineEdit->text()))
        di.mkdir(seaded->ui.lineEdit->text());
        di.cd(seaded->ui.lineEdit->text());
        }
        di.mkdir(valik->ui.voistluseNimi->text());
    di.cd(valik->ui.voistluseNimi->text());*/
    QFile fail(valik->ui.failiNimi->text());
    if(fail.open(QIODevice::WriteOnly)){
        QDataStream valja(&fail);

        valja << (quint32)0x00FA3058;	//Kontrollarv
        valja << (qint32)15;			//Millise programmi versiooni failiga on tegu
        valja.setVersion(QDataStream::Qt_4_3);

        valja << valik->ui.voistluseNimi->text();
        valja << valik->ui.alaNimi->text();

        for(int i = 0; i < 8; i++){
            valja << tabel->id[i];
            valja << (QString)tabel->nimi[i]->text();
            valja << (QString)tabel->tulemus[i]->text();
            for(int j = 0; j < 24; j++)
            valja << (QString)tabel->lask[j][i]->text();
            for(int j = 0; j < 24; j++)
            valja << (QString)tabel->lisaLask[j][i]->text();
        }
    }
}

void QFinaalMain::salvestaNimega()
{
    //if(valik->ui.voistluseNimi->text().isEmpty() || valik->ui.alaNimi->text().isEmpty()) return;
    QString failiNimi = QFileDialog::getSaveFileName(this, tr("Salvesta nimega"), "" +
    valik->ui.failiNimi->text(),	tr("Finaali fail (*.fnl)"));
    if(failiNimi.isEmpty() || failiNimi == ".fnl") return;
    if(!failiNimi.endsWith(".fnl")) failiNimi.append(".fnl");
    valik->ui.failiNimi->setText(failiNimi);
    salvesta();
    /*QFile fail(failiNimi);
        fail.open(QIODevice::WriteOnly);
        QDataStream valja(&fail);

        valja << (quint32)0x00FA3058;	//Kontrollarv
        valja << (qint32)12;			//Millise programmi versiooni failiga on tegu
        valja.setVersion(QDataStream::Qt_4_3);

        for(int i = 0; i < 8; i++){
        valja << (QString)tabel->nimi[i]->text();
        valja << (QString)tabel->tulemus[i]->text();
        for(int j = 0; j < 10; j++)
        valja << (QString)tabel->lask[j][i]->text();
        for(int j = 0; j < 10; j++)
        valja << (QString)tabel->lisaLask[j][i]->text();
    }*/
}

void QFinaalMain::uhenduServeriga()
{
	if(socket == 0){
		socket = new QTcpSocket(this);
		connect(socket, SIGNAL(readyRead()), this, SLOT(loeVorgust()));
        connect(socket, SIGNAL(connected()), this, SLOT(uhendusSiusigaLoodud()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(uhendusSiusigaKatkes()));
	}
	if(uhendumiseAken == 0)
	uhendumiseAken = new UhendumiseAken(this);
	if(uhendumiseAken->exec() == QDialog::Accepted)
	socket->connectToHost(uhendumiseAken->ui.aadressEdit->text(), uhendumiseAken->ui.portEdit->text().toInt());
}

void QFinaalMain::uhenduSiusDataga()
{
	if(siusDataSocket == 0){
		siusDataSocket = new QTcpSocket(this);
		connect(siusDataSocket, SIGNAL(readyRead()), this, SLOT(loeSiusDatast()));
	}
	if(uhendumiseAken == 0){
		uhendumiseAken = new UhendumiseAken(this);
        uhendumiseAken->ui.aadressEdit->setText("127.0.0.1");
		uhendumiseAken->ui.portEdit->setText("4000");
	}
	if(uhendumiseAken->exec() == QDialog::Accepted)
            siusDataSocket->connectToHost(uhendumiseAken->ui.aadressEdit->text(), uhendumiseAken->ui.portEdit->text().toInt());

    voistlus = false;  //Kasutatakse SiusDatast info vastuvõtmisel, võiks tõsta Siusiga ühendumise fn alla

    //Logi, andmed, mis tulevad Siusist
    siusLogi = new QFile(QDir::toNativeSeparators(valik->ui.failiNimi->text().left(QDir::fromNativeSeparators(valik->ui.failiNimi->text()).lastIndexOf('/')) + QString("/Finaal sisse logi %1.log").arg(QDate::currentDate().toString(Qt::ISODate))));
}

void QFinaalMain::uhendusSiusigaKatkes()
{
    statusBar()->showMessage("Ühendus SiusDataga katkes!");
}

void QFinaalMain::uhendusSiusigaLoodud()
{
    statusBar()->showMessage("Ühendus SiusDataga loodud", 5000);
}

void QFinaalMain::uusUhendus()
{
    QMessageBox::information(this, "Finaal", tr("Serverisse on loodud uus ühendus."), QMessageBox::Ok);
	saadaLask("Tere");
}

void QFinaalMain::xls()
{
	summ0();
	summ1();
	summ2();
	summ3();
	summ4();
	summ5();
	summ6();
	summ7();
	
	xlslib_core::workbook wb;
	xlslib_core::worksheet *sheet = wb.sheet("Finaal");
	
	xlslib_core::font_t *tiitelFont = wb.font("Times New Roman");   //font, millega kirjutatakse võistluse nimi
	tiitelFont->SetBoldStyle(xlslib_core::BOLDNESS_BOLD);
	tiitelFont->SetHeight(20*16); //teksti kõrgus 16
	
	xlslib_core::font_t *pealkiriFont = wb.font("Times New Roman"); //font, millega kirjutatakse "Finaal"
	pealkiriFont->SetBoldStyle(xlslib_core::BOLDNESS_BOLD);
	pealkiriFont->SetHeight(20*14); //teksti kõrgus 14
	
	xlslib_core::font_t *paksFont = wb.font("Times New Roman"); //font, millega kirjutatakse rasvane tekst
	paksFont->SetBoldStyle(xlslib_core::BOLDNESS_BOLD);
	paksFont->SetHeight(20*12); //teksti kõrgus 12
	
	xlslib_core::font_t *underlineFont = wb.font("Times New Roman"); //font, millega kirjutatakse koht ja nimi
	underlineFont->SetUnderlineStyle(xlslib_core::UNDERLINE_SINGLE);
	underlineFont->SetHeight(20*12); //teksti kõrgus 12
	
	xlslib_core::font_t *tekstiFont = wb.font("Times New Roman");    //tavalise teksti font
	tekstiFont->SetHeight(20*12);   //teksti kõrgus 12
	
	for(int i = 0; i < 100; i++)    //tabel kõik Times New Romaniga ja suurusega 10
	for(int j = 0; j < 50; j++)
	sheet->blank(i, j)->font(tekstiFont);
	
	sheet->defaultColwidth(8);  //millegipärast vajalik, et saaks muuta tulpade laiust
	sheet->colwidth(0, 32*33); //33 pikslit
	sheet->colwidth(1, 32*120); //120 pikslit
	sheet->colwidth(2, 32*96); //96 pikslit
	for(int i = 3; i <= 12; i++)
	sheet->colwidth(i, 32*40); //40 pikslit
	sheet->colwidth(13, 32*32); //32 pikslit
	
	//sheet->rowheight(2, 256); //12.75 (17pikslit)
	sheet->merge(0, 0, 0, 13);
	sheet->label(0, 0, valik->ui.voistluseNimi->text().toLocal8Bit().data())->font(tiitelFont);
	sheet->FindCell(0, 0)->halign(xlslib_core::HALIGN_CENTER);
	
	sheet->label(2, 1, "Finaal")->font(pealkiriFont);
	sheet->label(3, 1, valik->ui.alaNimi->text().toLocal8Bit().data())->font(paksFont);
	//sheet->label(2, 9, aegKoht.toStdString())->font(paksFont);
	
	sheet->label(5, 0, "Koht")->font(underlineFont);
	sheet->label(5, 1, "Nimi")->font(underlineFont);
	sheet->FindCell(5, 0)->fontitalic(true);
	sheet->FindCell(5, 1)->fontitalic(true);
	sheet->FindCell(5, 0)->halign(xlslib_core::HALIGN_CENTER);
	sheet->FindCell(5, 1)->halign(xlslib_core::HALIGN_CENTER);
	
	sheet->label(6, 0, "I")->font(paksFont);
	sheet->FindCell(6, 0)->halign(xlslib_core::HALIGN_CENTER);
	sheet->label(11, 0, "II")->font(paksFont);
	sheet->FindCell(11, 0)->halign(xlslib_core::HALIGN_CENTER);
	sheet->label(16, 0, "III")->font(paksFont);
	sheet->FindCell(16, 0)->halign(xlslib_core::HALIGN_CENTER);
	
	/*sheet->label(5,0,"Tere!")->font(tekstiFont);
		sheet->label(5,1,"Mis")->font(tekstiFont);
		sheet->label(5,2,"teed?")->font(tekstiFont);
		xlslib_core::cell_t *pCell = sheet->number(1, 0, 10);
	xlslib_core::cell_t *pCell2 = sheet->number(2, 0, 12);*/
	
	int seeria = 0;
	int iarv = 0;
	float ssum = 0;
	float arv = 0;
	
	QMap<int, int> map; //vajalik sorteerimiseks, QMap sorteerib võtmed automaatselt
	for(int i = 0; i < 8; i++)
	map.insertMulti(tabel->iSumma[i], i);
	
	QList<int> jarjekord = map.values();   //Sorteeritud laskurite jrk numbrid
	
	for(int i = 0; i < 8; i++){
		
		if(i <= 2)
                sheet->label(i * 7 + 6, 1, tabel->nimi[jarjekord.at(7 - i)]->text().toLocal8Bit().data())->font(paksFont);
		else{
                        sheet->label(i * 7 + 6, 1, tabel->nimi[jarjekord.at(7 - i)]->text().toLocal8Bit().data())->font(tekstiFont);
                        sheet->label(i * 7 + 6, 0, (est->toString(i + 1) + ".").toStdString())->font(tekstiFont);
                        sheet->FindCell(i * 7 + 6, 0)->halign(xlslib_core::HALIGN_CENTER);
		}
		
        for(int j = 0; j < 5; j++){
			if(tabel->lask[j][jarjekord.at(7 - i)]->text() != "00")
                        sheet->label(i * 7 + 7 + j, 3, tabel->lask[j][jarjekord.at(7 - i)]->text().toStdString())->font(tekstiFont);
                        sheet->FindCell(i * 7 + 7 + j, 3)->halign(xlslib_core::HALIGN_CENTER);
		}
        if(tabel->seeria1[jarjekord.at(7 - i)]->text() != "0,0")    //Esimese seeria summa kirjutamine
                sheet->label(i * 7 + 6, 3, tabel->seeria1[jarjekord.at(7 - i)]->text().toStdString())->font(paksFont);
                sheet->FindCell(i * 7 + 6, 3)->halign(xlslib_core::HALIGN_CENTER);
		
        for(int j = 0; j < 5; j++){
            if(tabel->lask[j + 5][jarjekord.at(7 - i)]->text() != "00")
                        sheet->label(i * 7 + 7 + j, 4, tabel->lask[j + 5][jarjekord.at(7 - i)]->text().toStdString())->font(tekstiFont);
                        sheet->FindCell(i * 7 + 7 + j, 4)->halign(xlslib_core::HALIGN_CENTER);
		}
		
		arv = est->toFloat(tabel->seeria1[jarjekord.at(7 - i)]->text());
		iarv = qRound(arv * 10);
		seeria = iarv;
		arv = est->toFloat(tabel->seeria2[jarjekord.at(7 - i)]->text());
		iarv = qRound(arv * 10);
		seeria += iarv;
        ssum = seeria;
        ssum = ssum / 10;

        if(tabel->seeria2[jarjekord.at(7 - i)]->text() != "0,0"){    //Summa kirjutamine peale teist seeriat
            if(seeria % 10 != 0)
                sheet->label(i * 7 + 6, 4, est->toString(ssum).toStdString())->font(paksFont);
            else sheet->label(i * 7 + 6, 4, (est->toString(ssum) + ",0").toStdString())->font(paksFont);
        }
        sheet->FindCell(i * 7 + 6, 4)->halign(xlslib_core::HALIGN_CENTER);

        for(int j = 10; j < 23; j += 2){
			arv = est->toFloat(tabel->lask[j][jarjekord.at(7 - i)]->text());
			iarv = qRound(arv * 10);
			seeria += iarv;
			arv = est->toFloat(tabel->lask[j + 1][jarjekord.at(7 - i)]->text());
			iarv = qRound(arv * 10);
			seeria += iarv;
			ssum = seeria;
			ssum = ssum / 10;
			
			if(tabel->lask[j][jarjekord.at(7 - i)]->text() != "00")
                        sheet->label(i * 7 + 7, 5 + (j - 10) / 2, tabel->lask[j][jarjekord.at(7 - i)]->text().toStdString())->font(tekstiFont);
                        sheet->FindCell(i * 7 + 7, 5 + (j - 10) / 2)->halign(xlslib_core::HALIGN_CENTER);
			
			if(tabel->lask[j + 1][jarjekord.at(7 - i)]->text() != "00")
                        sheet->label(i * 7 + 8, 5 + (j - 10) / 2, tabel->lask[j + 1][jarjekord.at(7 - i)]->text().toStdString())->font(tekstiFont);
                        sheet->FindCell(i * 7 + 8, 5 + (j - 10) / 2)->halign(xlslib_core::HALIGN_CENTER);
			
			if(seeria != 0 && seeria != qRound(est->toFloat(tabel->seeria[jarjekord.at(7 - i)]->text()) * 10)){
				if(seeria % 10 != 0)
                                sheet->label(i * 7 + 6, 5 + (j - 10) / 2, est->toString(ssum).toStdString())->font(paksFont);
                                else sheet->label(i * 7 + 6, 5 + (j - 10) / 2, (est->toString(ssum) + ",0").toStdString())->font(paksFont);
			}
                        sheet->FindCell(i * 7 + 6, 5 + (j - 10) / 2)->halign(xlslib_core::HALIGN_CENTER);
		}
		
		if(tabel->seeria[jarjekord.at(7 - i)]->text() != "0,0")
                sheet->label(i * 7 + 6, 12, tabel->seeria[jarjekord.at(7 - i)]->text().toStdString())->font(paksFont);
                sheet->FindCell(i * 7 + 6, 12)->halign(xlslib_core::HALIGN_CENTER);
	}
	/*for(int j = 0; j < 3; j++){
		arv = est->toFloat(tabel->lask[j][i]->text(), &onnestus);
		iarv = qRound(arv * 10);
		seeria1 += iarv;
		}
		for(int j = 3; j < 6; j++){
		arv = est->toFloat(tabel->lask[j][i]->text(), &onnestus);
		iarv = qRound(arv * 10);
		seeria2 += iarv;
		}
		for(int i = 0; i < 8; i++)
		for(int j = 0; j < 20; j++){
		
		}
		
		/*xlslib_core::expression_node_factory_t& maker = wb.GetFormulaFactory();   //Valemitega variant
		xlslib_core::expression_node_t *valem;
		
		for(int i = 6; i < 42; i += 5)
		{
		for(int j = 3; j < 5; j++)
		{
		valem = maker.f(xlslib_core::FUNC_SUM, maker.area(*sheet->FindCell(i + 1, j), *sheet->FindCell(i + 3, j), xlslib_core::CELL_RELATIVE_A1));
		sheet->formula(i, j, valem, true);
		sheet->FindCell(i, j)->font(paksFont);
		sheet->FindCell(i, j)->halign(xlslib_core::HALIGN_CENTER);
		sheet->FindCell(i, j)->format(xlslib_core::FMT_NUMBER3);
		
		//sheet->FindCell(1,0)->font(tekstiFont);
		//sheet->FindCell(2,0)->font(tekstiFont);
		}
		for(int j = 5; j < 11; j++)
		{
		valem = maker.f(xlslib_core::FUNC_SUM, maker.area(*sheet->FindCell(i + 1, j), *sheet->FindCell(i + 2, j), xlslib_core::CELL_RELATIVE_A1));
		sheet->formula(i, j, valem, true);
		sheet->FindCell(i, j)->font(paksFont);
		sheet->FindCell(i, j)->halign(xlslib_core::HALIGN_CENTER);
		sheet->FindCell(i, j)->format(xlslib_core::FMT_NUMBER3);
		}
	}*/
	QString fnimi = valik->ui.failiNimi->text();
	if(fnimi.endsWith(".fnl", Qt::CaseInsensitive))
	fnimi.replace(".fnl", ".xls");
	else fnimi.append(".xls");
	wb.Dump(fnimi.toLocal8Bit().data());
	QMessageBox::information(this, "Finaal", QString("Tulemused eksporditud .xls faili:\n%1").arg(fnimi), QMessageBox::Ok);
}

void QFinaalMain::xlsUus()  //Siia oli mõte teha funktsioon, mis loeks eksporditava tabeli formaadi sisse csv failist, näiteks v1l1;v1l2;v1s1
{
    summ0();
    summ1();
    summ2();
    summ3();
    summ4();
    summ5();
    summ6();
    summ7();

    xlslib_core::workbook wb;
    xlslib_core::worksheet *sheet = wb.sheet("Finaal");

    xlslib_core::font_t *tiitelFont = wb.font("Times New Roman");   //font, millega kirjutatakse võistluse nimi
    tiitelFont->SetBoldStyle(xlslib_core::BOLDNESS_BOLD);
    tiitelFont->SetHeight(20*16); //teksti kõrgus 16

    xlslib_core::font_t *pealkiriFont = wb.font("Times New Roman"); //font, millega kirjutatakse "Finaal"
    pealkiriFont->SetBoldStyle(xlslib_core::BOLDNESS_BOLD);
    pealkiriFont->SetHeight(20*14); //teksti kõrgus 14

    xlslib_core::font_t *paksFont = wb.font("Times New Roman"); //font, millega kirjutatakse rasvane tekst
    paksFont->SetBoldStyle(xlslib_core::BOLDNESS_BOLD);
    paksFont->SetHeight(20*12); //teksti kõrgus 12

    xlslib_core::font_t *underlineFont = wb.font("Times New Roman"); //font, millega kirjutatakse koht ja nimi
    underlineFont->SetUnderlineStyle(xlslib_core::UNDERLINE_SINGLE);
    underlineFont->SetHeight(20*12); //teksti kõrgus 12

    xlslib_core::font_t *tekstiFont = wb.font("Times New Roman");    //tavalise teksti font
    tekstiFont->SetHeight(20*12);   //teksti kõrgus 12

    for(int i = 0; i < 100; i++)    //tabel kõik Times New Romaniga ja suurusega 10
    for(int j = 0; j < 50; j++)
    sheet->blank(i, j)->font(tekstiFont);

    sheet->defaultColwidth(8);  //millegipärast vajalik, et saaks muuta tulpade laiust
    sheet->colwidth(0, 32*33); //33 pikslit
    sheet->colwidth(1, 32*120); //120 pikslit
    sheet->colwidth(2, 32*96); //96 pikslit
    for(int i = 3; i <= 12; i++)
    sheet->colwidth(i, 32*40); //40 pikslit
    sheet->colwidth(13, 32*32); //32 pikslit

    //sheet->rowheight(2, 256); //12.75 (17pikslit)
    sheet->merge(0, 0, 0, 13);
    sheet->label(0, 0, valik->ui.voistluseNimi->text().toLocal8Bit().data())->font(tiitelFont);
    sheet->FindCell(0, 0)->halign(xlslib_core::HALIGN_CENTER);

    sheet->label(2, 1, "Finaal")->font(pealkiriFont);
    sheet->label(3, 1, valik->ui.alaNimi->text().toLocal8Bit().data())->font(paksFont);
    //sheet->label(2, 9, aegKoht.toStdString())->font(paksFont);

    sheet->label(5, 0, "Koht")->font(underlineFont);
    sheet->label(5, 1, "Nimi")->font(underlineFont);
    sheet->FindCell(5, 0)->fontitalic(true);
    sheet->FindCell(5, 1)->fontitalic(true);
    sheet->FindCell(5, 0)->halign(xlslib_core::HALIGN_CENTER);
    sheet->FindCell(5, 1)->halign(xlslib_core::HALIGN_CENTER);

    sheet->label(6, 0, "I")->font(paksFont);
    sheet->FindCell(6, 0)->halign(xlslib_core::HALIGN_CENTER);
    sheet->label(11, 0, "II")->font(paksFont);
    sheet->FindCell(11, 0)->halign(xlslib_core::HALIGN_CENTER);
    sheet->label(16, 0, "III")->font(paksFont);
    sheet->FindCell(16, 0)->halign(xlslib_core::HALIGN_CENTER);

    /*sheet->label(5,0,"Tere!")->font(tekstiFont);
            sheet->label(5,1,"Mis")->font(tekstiFont);
            sheet->label(5,2,"teed?")->font(tekstiFont);
            xlslib_core::cell_t *pCell = sheet->number(1, 0, 10);
    xlslib_core::cell_t *pCell2 = sheet->number(2, 0, 12);*/

    int seeria = 0;
    int iarv = 0;
    float ssum = 0;
    float arv = 0;

    QMap<int, int> map; //vajalik sorteerimiseks, QMap sorteerib võtmed automaatselt
    for(int i = 0; i < 8; i++)
    map.insertMulti(tabel->iSumma[i], i);

    QList<int> jarjekord = map.values();   //Sorteeritud laskurite jrk numbrid
}
