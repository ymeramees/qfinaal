#ifndef TULEMUSEAKEN_H_
	#define TULEMUSEAKEN_H_
	
	#include <QtCore/QVariant>
	
	#if QT_VERSION >= 0x050000
		#include <QtWidgets>
		#else
		#include <QtGui/QApplication>
		#include <QtGui/QWidget>
		#include <QtGui/QLabel>
		#include <QtGui/QVBoxLayout>
		#include <QtGui/QHBoxLayout>
		#include <QtGui/QSpacerItem>
		#include <QtGui/QMenu>
	#endif
	
	#include <QApplication>
	#include <QWidget>
	#include <QLabel>
	#include <QVBoxLayout>
	#include <QHBoxLayout>
	#include <QSpacerItem>
	#include <QMenu>

#include "markleht.h"
	
	class TulemuseAken : public QWidget
	{
		
        Q_OBJECT
		
		public:
        Markleht *marklehed[8];
//        Markleht *markleht2;
		int mitmesLask;
		QFont kiri;
		QFont kiriKriipsutatud;
        QGridLayout *ruudustik;
        QVBoxLayout *vRuudustik;
        QImage *pilt;
        QPainter *painter;
        QLabel *silt;
        QString *pealKiri;
        QString *nim;
        QString *rad;
        QString *las;
        QString *see;
        QString *kok;
        QString *vah;
        bool kohal[8];
        //QString *summa[8];
        QString *seeria[8];
        QString *nimi[8];
        QString *rada[8];
        QString *lask[8];
        QString *vahe[8];
        QString *koht[8];
        QLocale *est;
        QMenu *popup;
        QAction *fontAct;
        QAction *fullScreenAct;
        TulemuseAken(QWidget *parent = 0);
        virtual ~TulemuseAken();
        void resizeEvent(QResizeEvent *event);
        void contextMenuEvent(QContextMenuEvent *event);

    public slots:
        void muudaFont();
        void fullScreen();
        void joonista();
        void mouseDoubleClickEvent(QMouseEvent *event);
        void naitaLehed();  //Toob märklehed nähtavale
        void naitaNimed();  //Toob nimed nähtavale
        void peidaLehed();  //Peidab märklehed
        void peidaNimed();  //Peidab nimede tabeli
        void puhastaLehed();    //Puhastab lehed eelmistest laskudest
	};
	
#endif /*TULEMUSEAKEN_H_*/
