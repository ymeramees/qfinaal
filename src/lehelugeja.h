#ifndef LEHELUGEJA_H
#define LEHELUGEJA_H

#include <QtGlobal>
#if QT_VERSION >= 0x050000
#include <QDialog>
#include <QMessageBox>
#else
#include <QtGui/QDialog>
#include <QtGui/QMessageBox>
#endif

#include <QDebug>
//#define _TTY_WIN_
//#include <qextserialbase.h>
//#include <qextserialport.h>
#include "lugemismasin.h"
//#include "qextserialport.h"
#include "qextserialenumerator.h"

namespace Ui {
    class Lehelugeja;
}

class Lehelugeja : public QDialog {
    Q_OBJECT
public:
    //QTimer timer;
//    QextSerialPort *serial;
    LugemisMasin *lugemisMasin;
    QList<QextPortInfo> pordid;
    QString pordiNimi;
//    Lehelugeja(QextSerialPort *s, QString harjutus,QWidget *parent = 0);
    Lehelugeja(LugemisMasin *s, QString harjutus,QWidget *parent = 0);
    ~Lehelugeja();

public slots:
    void pordiNimiMuutus(QString s);
    void avaUhendus();
    void lisaLogisse(QString s);
    void seadista();
    void seadistaVorgule();
    void lopetaUhendus();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::Lehelugeja *m_ui;

signals:
    void portSuletud();
};

#endif // LEHELUGEJA_H
