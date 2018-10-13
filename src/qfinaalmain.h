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
#include "../Protokollitaja/lask.h"
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
//        QList<bool> voistlus;   //Kasutatakse SiusDatast info vastuvõtmisel, näitab, kas on proovilasud või võistluslasud
                                   //Finaalis on ainult ühte bool'i vaja, protokollitajas aga igale laskurile üht
        bool voistlus;   //Kasutatakse SiusDatast info vastuvõtmisel, näitab, kas finaali võistluslasud on alanud
        bool koikLasknud;   //Kasutatakse märklehtede peitmise juures, kui kõik, kes kohal, on lasknud
        int mitmesLask; //Mitmes lask parasjagu on
        int mituPuudu;  //Mitu inimest ei ole kohal
        int mituLasknud;    //Mitu inimest on lasknud
        int mituKohal;
        int relv; //Vaikimisi on õhupüssi leht, 0 - õhupüss; 1 - õhupüstol; 2 - sportpüss
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
        QAction *kustutaLasudAct;   //Kustutab kõik lasud, kiirema arenduse eesmärgil
        QAction *lehelugejaAct;
        QAction *loeSiiaAct;
        QAction *loeVorkuAct;
        QAction *uhenduServerigaAct;
        QAction *uhenduSiusDatagaAct;
        QAction *xlsAct;
        QFile *logi;    //Siusist ja lehelugemismasinast tulnud info põhjal tehtud muudatuste logi
        QFile *siusLogi;    //Siusist saabunud võrguliikluse logi
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
        void initTulemuseaken();    //Tulemuseakna esimene näitamine
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
        void uhendusSiusigaKatkes();    //Kui ühendus katkes, siis näitab sellekohast teadet
        void uhendusSiusigaLoodud();    //Kui ühendus on loodud, siis näitab sellekohast teadet
        void kasKohal();
        void kustutaLasud();    //Kustutab kõik lasud, lihtsama arendamise jaoks
        void lehelugeja();
//        void lisaLogisse(QString);
        void loeLask(Lask); //Uus lugemismasinast lugemise fn
//        void loeLehti();
        void loeSiia();
        void loeSiusDatast();   //Võtab vastu lasud SiusDatast
        void loeVorgust();
        void loeVorku();
        void maaraRelv();   //Teeb kindlaks või küsib kasutajalt, mis relvaga on tegu ja milliseid märklehti näidata
        void peidaLehed();  //Peidab märklehed
//        void portSuletud();
        void saadaLask(QString lask);
        void seadistaLehelugemismasin();
        void uuendaLasuNrit();  //Uuendab, mitmes lask käimas on
        void uuendaSeaded();    //Esialgu kasutusel ainult märklehtede valiku jaoks, teised seaded võetakse otse
        void uusUhendus();
        void xls();
        void xlsUus();
	};
	
#endif /*QFINAALMAIN_H_*/
