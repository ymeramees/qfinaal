#ifndef SEADED_H
	#define SEADED_H
	
	#include <QtGlobal>
	#if QT_VERSION >= 0x050000
		#include <QDialog>
		#else
		#include <QtGui/QDialog>
	#endif
	#include <QFileDialog>
	#include <QMessageBox>
	#include <QTextStream>
	#include "ui_Seaded.h"
	
	class Seaded : public QDialog
	{
		Q_OBJECT
		
		public:
		//QString vanaDir;
        bool vanaLangemine;
		int vanaIndex;
        int vanaMarklehtedeAeg;
        int vanaRelv;
		Seaded(QWidget *parent = 0);
		~Seaded();
		Ui::SeadedClass ui;
		
		public slots:
        //void sirvi();
        void loobu();
	};
	
#endif // SEADED_H
