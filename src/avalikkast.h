#ifndef VALIKKAST_H_
#define VALIKKAST_H_

#include <QtGui/QDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include "ui_ValikKast.h"
#include "seaded.h"

class ValikKast : public QDialog
{

        Q_OBJECT

public:
        bool avamine;
        ValikKast(Seaded* ,QWidget *parent = 0);
        virtual ~ValikKast();
        Ui::Dialog ui;
        Seaded* seaded;

public slots:
        void closeEvent(QCloseEvent *event);
        void on_avaNupp_clicked();
};

#endif /*VALIKKAST_H_*/
