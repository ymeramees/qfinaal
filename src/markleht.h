#ifndef MARKLEHT_H
#define MARKLEHT_H

#include <QLabel>
#include <QPainter>
#ifdef PROOV
#include <QDebug>
#endif

#include "lask.h"

class Markleht : public QLabel
{
    Q_OBJECT

private:
    int kaliiber;   //Lasu ringi l�bim��t mm'tes
    int kaugeimX;
    int kaugeimY;
    int kordaja;    //Suhe mm ja px'ide vahel, m��tkava: 1px = kordaja * mm
    int markleheRaadius;
    int relv;
    int zoomiAste;
    bool aktiivne;  //N�itab, kas laskur on v�lja langenud v�i mitte
    QImage *pilt;
    Lask eelmineLask;
    QPainter *painter;
    QString nimi;
    QString rada;
    QString tulemus;

private slots:
//    void mouseReleaseEvent(QMouseEvent * event);
    void resizeEvent(QResizeEvent * e);
public:
    static const QStringList lehetuubid;// = QStringList() << QString::fromLatin1("�hup�ss") << QString::fromLatin1("�hup�stol") << QString::fromLatin1("Sportp�ss");
    explicit Markleht(QWidget *parent = 0);
    explicit Markleht(int relv, QString n, QString r, QWidget *parent = 0);

signals:

public slots:
    QString getNimi();
    QString getRada();
    QStringList getLehetuubid();
    int getRelv();
    void init(int valik);
    void joonistaLask(Lask & l);    //Koordinaadid millimeetrites
    void joonistaLeht();
    void reset();
    void setAktiivne(bool a);
    void setNimi(QString n);
    void setRada(QString r);
    void setRelv(int r);
    void setTulemus(QString tul);
    void zoom();

};

#endif // MARKLEHT_H
