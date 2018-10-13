#ifndef VALIKKAST_H
	#define VALIKKAST_H
	
	#include <QtGlobal>
	#if QT_VERSION >= 0x050000
		#include <QDialog>
		#else
		#include <QtGui/QDialog>
	#endif
	#include <QInputDialog>
	#include <QFileDialog>
	#include <QTextStream>
	#include <QMessageBox>
	#include "ui_ValikKast.h"
	#include "seaded.h"
	
	class ValikKast : public QDialog
	{
		Q_OBJECT
		
		public:
		//QString failiNimi;
		ValikKast(QWidget *parent = 0);
		~ValikKast();
		Ui::ValikKastClass ui;
		//Seaded* seaded;
		
		public slots:
		void avamine();
        void edasi();
		void muutus(QString);
		void uus();
	};
	
#endif // VALIKKAST_H
