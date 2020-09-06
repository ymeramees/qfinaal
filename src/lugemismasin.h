#ifndef LUGEMISMASIN_H
#define LUGEMISMASIN_H

#include <QObject>
#include <QLineF>
#include <QTimer>
#include <QDebug>

#include "lask.h"
#include "qextserialport.h"
#include "qextserialenumerator.h"

class LugemisMasin : public QObject
{
    Q_OBJECT
public:
    explicit LugemisMasin(QObject *parent = 0);

private:
    bool lugemismasinaValimine; //Näitab, kas alles määratakse kindlaks, milline lugemismasin ühendatud on
    bool oliLiigneLask;
    bool vahemuses;
    bool oliKahtlane;
    bool uhendatud; //Vajalik, et aru saada, kas lugemismasin on ühendatud või mitte, et uus nupuvajutus midagi ära ei rikuks.
    bool uusLugemismasin;   //Kas ühendatud on uus või vana lugemismasin
    bool kaabelLeitud;  //Kontrollimaks, kas käivitamisel leiti lugemismasina kaabel või ei
    int saatmiseEtapp;  //Näitab mitmendas etapis saatmine parasjagu on: 0 - algseis/ACK tulnud (lõpp), 1 - ENQ saadetud, 2 - STX saabunud, 3 - tekst saadetud, 4 - saabus tekst, vaja vastata ACK
    int laskudeArv; //Laskude arv lehes
    int relv;   //Märklehe tüüp, 0 - õhupüss, 1 - õhupüstol, 2 - sportpüss
    Lask loetudLask;    //Lask, kuhu loetud info talletatakse saatmiseks
    QByteArray saadetis;    //Lugemismasinasse saadetav tekst
    QextSerialPort *serial;
    QList<QextPortInfo> pordid;
    QString COMport;    //Pordi nimetus
    QTimer *lugeja; //Kontrollib, kas serial pordis on uut infot
//    QTimer *otsija; //Portide otsimiseks
    QTimer *saatja; //Viivis enne saatmist, et uus lugemismasin jõuaks reageerida
    QTimer *seadistaja; //Viivis peale lehtede lugemist, enne kui uuesti seadistatakse

private slots:
    int CRC(QByteArray *s);  //Arvutab lugemismasina ühenduse jaoks kontrollnumbri
    void loe();
    void loe2();    //Uuest lugemismasinast lehtede lugemine
//    void otsiPorti(); //Lisame hiljem
    void saada();
    void seadista2();   //Uue lugemismasina seadistamine lehtede lugemiseks
    void uhenda();
    void uhenda2(); //Uue lugemismasinaga ühendamine
signals:
    void teadeLogisse(QString); //Saadab teateated ja lugemismasinast tulnud tekst, mida võiks näidata lugemise logis
    void laskLoetud(const Lask) const;  //Saadab loetud lasu

public slots:
    QString getCOMport();   //Tagastab COM pordi nimetuse
    int getLaskudeArv();    //Tagastab laskude arvu lehes
    int getRelv();  //Tagastab märklehe tüübi
    void saada(QString);
    void seadista();
    void setCOMport(QString);   //Seab COM pordi nimetuse
    void setLaskudeArv(int);   //Seab laskude arvu lehes
    void setRelv(int); //Määrab märklehe tüübi
    void start();   //Käivitab tameri, mis hakkab kontrollima, kas on uut infot
    void stop();    //Peatab taimeri, mis kontrollib, kas on masinast uut infot
    void sulgeUhendus();
    void uuendaPorte();
};

#endif // LUGEMISMASIN_H
