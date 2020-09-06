#ifndef QFINAALMAIN_H_
	#define QFINAALMAIN_H_
	
	#include <QMainWindow>
	#include <QtNetwork>
	
	#if QT_VERSION >= 0x050000
		#include <QtWidgets>
		#include <QtPrintSupport/QPrinter>
		#include <QtPrintSupport/QPrintDialog>
		#else
		#include <QShortcut>
		#include <QMenuBar>
	#endif
	
#include "qfinaal.h"
#include "tulemuseaken.h"
#include "lehelugeja.h"
#include "lugemismasin.h"
#include "valikkast.h"
#include "seaded.h"
#include "uhendumiseaken.h"
#include "radadeaken.h"
//	#include "qextserialport.h"
	//#include <qextserialbase.h>
	//#include <qextserialport.h>
#include "xlslib.h"
#include "lask.h"
#include "markleht.h"
	
	
	class QAction;
	class QMenu;
	class QextSerialPort;
	
	class QFinaalMain : public QMainWindow
	{
		
        Q_OBJECT
		
protected:
        void closeEvent(QCloseEvent *event);

public:
//        QList<bool> voistlus;   //Kasutatakse SiusDatast info vastuv�tmisel, n�itab, kas on proovilasud v�i v�istluslasud
                                   //Finaalis on ainult �hte bool'i vaja, protokollitajas aga igale laskurile �ht
        bool voistlus;   //Kasutatakse SiusDatast info vastuv�tmisel, n�itab, kas finaali v�istluslasud on alanud
        bool koikLasknud;   //Kasutatakse m�rklehtede peitmise juures, kui k�ik, kes kohal, on lasknud
        int mitmesLask; //Mitmes lask parasjagu on
        int mituPuudu;  //Mitu inimest ei ole kohal
        int mituLasknud;    //Mitu inimest on lasknud
        int mituKohal;
        int relv; //Vaikimisi on �hup�ssi leht, 0 - �hup�ss; 1 - �hup�stol; 2 - sportp�ss
        quint16 blockSize;
        QFinaal *tabel;
        QMenu *failMenu;
        QMenu *editMenu;
        QMenu *helpMenu;
        QAction *uusAct;
        QAction *avaAct;
        QAction *saveAct;
        QAction *nsaveAct;
        QAction *peidaNaitaLehedAct;
        QAction *peidaNaitaNimedAct;
        QAction *printAct;
        QAction *valjuAct;
        QAction *eksportAct;
        QAction *importAct;
        QAction *impordiSiusStartListAct;
        QAction *naitaTul;
        QAction *infoAct;
        QAction *abiWAct;
        QAction *seadedAct;
        QAction *seadistaAct;
        QAction *vahemalluAct;
        QAction *kaivitaServerAct;
        QAction *kustutaLasudAct;   //Kustutab k�ik lasud, kiirema arenduse eesm�rgil
        QAction *lehelugejaAct;
        QAction *loeSiiaAct;
        QAction *loeVorkuAct;
        QAction *uhenduServerigaAct;
        QAction *uhenduSiusDatagaAct;
        QAction *xlsAct;
        QFile *logi;    //Siusist ja lehelugemismasinast tulnud info p�hjal tehtud muudatuste logi
        QFile *siusLogi;    //Siusist saabunud v�rguliikluse logi
        TulemuseAken *tulemus;
        ValikKast *valik;
        Seaded *seaded;
        Lehelugeja *lugeja;
//        QextSerialPort *serial;
        LugemisMasin *lugemisMasin;
        UhendumiseAken *uhendumiseAken;
        RadadeAken *radadeAken;
        QLocale *est;
        QTimer *aeg;
        QTimer *aeg2;
        QTimer *timer;
        QTimer *seadistaja; //Viivis peale lehtede lugemist, enne kui uuesti seadistatakse
        QTimer *lehtedePeitja;  //Peidab lehed peale viimast lasku
        QString ipAadress;
        QString siusDatast;
        QStringList eelmineRida;
        QKeyEvent *tab;
        QToolBar *toolBar;
        QTcpServer *server;
        QTcpSocket *socket;
        QTcpSocket *siusDataSocket; //SiusDataga suhtlemiseks
        QFinaalMain();
        virtual ~QFinaalMain();
        //void resizeEvent(QResizeEvent *event);

public slots:
        void uus();
        void loeFail();
        void ava();
        //void ava2();
        //void ava3(QString);
        void salvesta();
        void salvestaNimega();
        void eksport();
        void impordiSiusStartList();    //SiusData formaadis nimekirja importimine
        void initTulemuseaken();    //Tulemuseakna esimene n�itamine
        void jarjesta();
        void summ(int);
        void summ0();
        void summ1();
        void summ2();
        void summ3();
        void summ4();
        void summ5();
        void summ6();
        void summ7();
        void naitaValik();
        void naitaTulemused();
        void vajutaTab();
		void peidaLisaLasud();
        void peidaNaitaLehed();
        void peidaNaitaNimed();
        void print();
        void puhastaTulemuseaken();    //Tulemuseakna puhastamine eelmisest infost
        void import();
        void naitaInfo();
        void naitaWAbi();
        void naitaSeaded();
        void salvestaSeaded();
        void vahemallu();
        void kaivitaServer();
        void uhenduServeriga();
        void uhenduSiusDataga();
        void uhendusSiusigaKatkes();    //Kui �hendus katkes, siis n�itab sellekohast teadet
        void uhendusSiusigaLoodud();    //Kui �hendus on loodud, siis n�itab sellekohast teadet
        void kasKohal();
        void kustutaLasud();    //Kustutab k�ik lasud, lihtsama arendamise jaoks
        void lehelugeja();
//        void lisaLogisse(QString);
        void loeLask(Lask); //Uus lugemismasinast lugemise fn
//        void loeLehti();
        void loeSiia();
        void loeSiusDatast();   //V�tab vastu lasud SiusDatast
        void loeVorgust();
        void loeVorku();
        void maaraRelv();   //Teeb kindlaks v�i k�sib kasutajalt, mis relvaga on tegu ja milliseid m�rklehti n�idata
        void peidaLehed();  //Peidab m�rklehed
//        void portSuletud();
        void saadaLask(QString lask);
        void seadistaLehelugemismasin();
        void uuendaLasuNrit();  //Uuendab, mitmes lask k�imas on
        void uuendaSeaded();    //Esialgu kasutusel ainult m�rklehtede valiku jaoks, teised seaded v�etakse otse
        void uusUhendus();
        void xls();
        void xlsUus();
	};
	
#endif /*QFINAALMAIN_H_*/
