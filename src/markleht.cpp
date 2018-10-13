#include "markleht.h"

const QStringList Markleht::lehetuubid = QStringList() << QString::fromLatin1("’hup¸ss") << QString::fromLatin1("’hup¸stol") << QString::fromLatin1("Sportp¸ss");

Markleht::Markleht(QWidget *parent) :
    QLabel(parent)
{
    pilt = 0;
    painter = 0;
    aktiivne = true;
//    lehetuubid << QString::fromLatin1("’hup¸ss") << QString::fromLatin1("’hup¸stol") << QString::fromLatin1("Sportp¸ss");
}

Markleht::Markleht(int relv, QString n, QString r, QWidget *parent) :
    QLabel(parent)
{
    setNimi(n);
    setRada(r);
    init(relv);
}

QStringList Markleht::getLehetuubid()
{
    return lehetuubid;
}

QString Markleht::getNimi()
{
    return nimi;
}

QString Markleht::getRada()
{
    return rada;
}

int Markleht::getRelv()
{
    return relv;
}

void Markleht::init(int valik)
{
//    eelmineLask.setX(10);   //Testimiseks
//    eelmineLask.setY(15);
//    tulemus = "1099";
    relv = valik;   //0 - ıhup¸ss; 1 - ıhup¸stol; 2 - sportp¸ss
    aktiivne = true;

    if(pilt == 0)
        pilt = new QImage(1250, 1250, QImage::Format_ARGB32);
    if(painter == 0){
        painter = new QPainter(pilt);
        painter->translate(pilt->width() / 2, pilt->height() / 2);  //0-punkt pildi keskele, tuleb ainult algul ¸he korra teha
    }
//    painter->scale(1, -1);  //Y-koordinaat suunaga ¸les

    this->setBackgroundRole(QPalette::Base);
    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
//    this->setScaledContents(true);
    reset();
}

void Markleht::joonistaLask(Lask &l)
{
    painter->setBrush(Qt::red);
    int d = kordaja * kaliiber;
    if(!eelmineLask.isEmpty()){
        painter->drawEllipse(QPoint(kordaja * 2 * eelmineLask.X(), kordaja * -2 * eelmineLask.Y()), d, d);   //Vana lasu joonistamine teise v‰rviga
#ifdef PROOV
    qDebug() << "JoonistaLask(): eelminelask: " << kordaja * 2 * eelmineLask.X() << ", "  << kordaja * -2 * eelmineLask.Y();
#endif
    }
    painter->setBrush(Qt::green);
    painter->drawEllipse(QPoint(kordaja * 2 * l.X(), kordaja * -2 * l.Y()), d, d);   //Uue lasu joonistamine
    int x = abs(l.X() * 100);   //Et oleks ikka positiivsed v‰‰rtused
    int y = abs(l.Y() * 100);
#ifdef PROOV
    qDebug() << "QPoint: " << QPoint(kordaja * 2 * l.X(), kordaja * -2 * l.Y()) << "/tx: " << x << ", y: " << y;
#endif
    eelmineLask.set(&l);
    kaugeimX *= 100;    //Vırdlemine k‰ib t‰isarvudega
    if(kaugeimX == markleheRaadius * 100){ //Kui on esimene lask, on leht v‰lja zoom'itud, vaja sisse zoom'ida
       kaugeimX = 0;    //Algul tuleb see 0'ida, muidu ei zoomigi sisse
    }
    if(x > y){  //Oleneb, kumb koordinaat on suurem, vaja selle j‰rgi zoomida
        if(kordaja * 2 * x + d * 200 > kaugeimX)
            kaugeimX = kordaja * 2 * x + d * 200;  //Et kıige kaugem lask ikka tervenisti n‰ha oleks;
    }else{
        if(kordaja * 2 * y + d * 200 > kaugeimX)
            kaugeimX = kordaja * 2 * y + d * 200;  //Et kıige kaugem lask ikka tervenisti n‰ha oleks
    }
    kaugeimX /= 100;    //Tagasi ıigesse suurusesse
    zoom();
}

void Markleht::joonistaLeht()
{
    //Mııtkava: 1px = kordaja * mm
    if(relv == 0){   //’hup¸ss
        kordaja = 8;
        kaliiber = 4.5;
        kaugeimX = kaugeimY = markleheRaadius = 364;
        QFont font;
        font.setPointSize(24);
        painter->setFont(font);
        pilt->fill(Qt::white);
        QPen pliiats;
        if(aktiivne)
            pliiats.setColor(Qt::black);
        else
            pliiats.setColor(Qt::gray);
        pliiats.setWidth(4);
        painter->setBrush(Qt::white);
        painter->setPen(pliiats);

        painter->drawEllipse(QPoint(0, 0), 364, 364);
        painter->drawEllipse(QPoint(0, 0), 324, 324);
        painter->drawEllipse(QPoint(0, 0), 284, 284);
        if(aktiivne)
            painter->setBrush(Qt::black);
        else
            painter->setBrush(Qt::gray);
        painter->drawEllipse(QPoint(0, 0), 244, 244);
        pliiats.setColor(Qt::white);
        painter->setPen(pliiats);
        painter->drawEllipse(QPoint(0, 0), 204, 204);
        painter->drawEllipse(QPoint(0, 0), 164, 164);
        painter->drawEllipse(QPoint(0, 0), 124, 124);
        painter->drawEllipse(QPoint(0, 0), 84, 84);
        painter->drawEllipse(QPoint(0, 0), 44, 44);
        painter->drawEllipse(QPoint(0, 0), 4, 4);
        painter->drawText(64-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "8");
        painter->drawText(-64-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "8");
        painter->drawText(104-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "7");
        painter->drawText(-104-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "7");
        painter->drawText(144-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "6");
        painter->drawText(-144-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "6");
        painter->drawText(184-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "5");
        painter->drawText(-184-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "5");
        painter->drawText(224-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "4");
        painter->drawText(-224-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "4");

        if(aktiivne)
            painter->setPen(Qt::black);
        else
            painter->setPen(Qt::gray);
        painter->drawText(264-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "3");
        painter->drawText(-264-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "3");
        painter->drawText(304-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "2");
        painter->drawText(-304-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "2");
        painter->drawText(344-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "1");
        painter->drawText(-344-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "1");

    }else if(relv == 2){   //50m rifle target
        float fBlackRings[] = {122.4, 138.4, 154.4}; //black rings on white background, in mm's
        int blackRings[(sizeof(fBlackRings)/sizeof(*fBlackRings))];

        float fWhiteRings[] = {10.4, 26.4, 42.4, 58.4, 74.4, 90.4, 106.4}; //white rings on black background, in mm's
        int whiteRings[(sizeof(fWhiteRings)/sizeof(*fWhiteRings))];

        kordaja = 4;

        int blackArea = qRound(112.4 * kordaja);
        int innerTen = 5 * kordaja;
        for(int i = 0; i < (sizeof(fBlackRings)/sizeof(*fBlackRings)); i++){
            blackRings[i] = qRound(fBlackRings[i] * kordaja);
        }
        for(int i = 0; i < (sizeof(fWhiteRings)/sizeof(*fWhiteRings)); i++){
            whiteRings[i] = qRound(fWhiteRings[i] * kordaja);
        }

        kaliiber = 5.6;
        kaugeimX = kaugeimY = markleheRaadius = blackRings[(sizeof(blackRings)/sizeof(*blackRings))-1];

        QFont font;
        font.setPointSize(30);
        painter->setFont(font);
        pilt->fill(Qt::white);
        QPen pliiats;
        if(aktiivne)
            pliiats.setColor(Qt::black);
        else
            pliiats.setColor(Qt::gray);
        pliiats.setWidth(4);
        painter->setBrush(Qt::white);
        painter->setPen(pliiats);

        painter->drawEllipse(QPoint(0, 0), blackRings[2], blackRings[2]);   //1
        painter->drawEllipse(QPoint(0, 0), blackRings[1], blackRings[1]);
        painter->drawEllipse(QPoint(0, 0), blackRings[0], blackRings[0]);
        if(aktiivne)
            painter->setBrush(Qt::black);
        else
            painter->setBrush(Qt::gray);
        painter->drawEllipse(QPoint(0, 0), blackArea, blackArea);
        pliiats.setColor(Qt::white);
        painter->setPen(pliiats);
        painter->drawEllipse(QPoint(0, 0), whiteRings[6], whiteRings[6]);
        painter->drawEllipse(QPoint(0, 0), whiteRings[5], whiteRings[5]);   //5
        painter->drawEllipse(QPoint(0, 0), whiteRings[4], whiteRings[4]);
        painter->drawEllipse(QPoint(0, 0), whiteRings[3], whiteRings[3]);
        painter->drawEllipse(QPoint(0, 0), whiteRings[2], whiteRings[2]);
        painter->drawEllipse(QPoint(0, 0), whiteRings[1], whiteRings[1]);
        painter->drawEllipse(QPoint(0, 0), whiteRings[0], whiteRings[0]); //10
        painter->drawEllipse(QPoint(0, 0), innerTen, innerTen);

        painter->drawText((whiteRings[1]+whiteRings[2])/2-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "8");
        painter->drawText((whiteRings[1]+whiteRings[2])/-2-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "8");
        painter->drawText((whiteRings[2]+whiteRings[3])/2-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "7");
        painter->drawText((whiteRings[2]+whiteRings[3])/-2-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "7");
        painter->drawText((whiteRings[3]+whiteRings[4])/2-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "6");
        painter->drawText((whiteRings[3]+whiteRings[4])/-2-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "6");
        painter->drawText((whiteRings[4]+whiteRings[5])/2-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "5");
        painter->drawText((whiteRings[4]+whiteRings[5])/-2-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "5");
        painter->drawText((whiteRings[5]+whiteRings[6])/2-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "4");
        painter->drawText((whiteRings[5]+whiteRings[6])/-2-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "4");

        if(aktiivne)
            painter->setPen(Qt::black);
        else
            painter->setPen(Qt::gray);
        painter->drawText((blackArea+blackRings[0])/2-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "3");
        painter->drawText((blackArea+blackRings[0])/-2-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "3");
        painter->drawText((blackRings[0]+blackRings[1])/2-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "2");
        painter->drawText((blackRings[0]+blackRings[1])/-2-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "2");
        painter->drawText((blackRings[1]+blackRings[2])/2-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "1");
        painter->drawText((blackRings[1]+blackRings[2])/-2-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "1");

    }else{  //’hup¸stol
        kordaja = 4;
        kaliiber = 4.5;
        kaugeimX = kaugeimY = markleheRaadius = 622;
        QFont font;
        font.setPointSize(30);
        painter->setFont(font);
        pilt->fill(Qt::white);
        QPen pliiats;
        if(aktiivne)
            pliiats.setColor(Qt::black);
        else
            pliiats.setColor(Qt::gray);
        pliiats.setWidth(5);
        painter->setBrush(Qt::white);
        painter->setPen(pliiats);
        //painter->rotate(180);
        painter->drawEllipse(QPoint(0, 0), 622, 622);
        painter->drawEllipse(QPoint(0, 0), 558, 558);
        painter->drawEllipse(QPoint(0, 0), 494, 494);
        painter->drawEllipse(QPoint(0, 0), 430, 430);
        painter->drawEllipse(QPoint(0, 0), 366, 366);
        painter->drawEllipse(QPoint(0, 0), 302, 302);
        if(aktiivne)
            painter->setBrush(Qt::black);
        else
            painter->setBrush(Qt::gray);
        painter->drawEllipse(QPoint(0, 0), 238, 238);
        pliiats.setColor(Qt::white);
        painter->setPen(pliiats);
        painter->drawEllipse(QPoint(0, 0), 174, 174);
        painter->drawEllipse(QPoint(0, 0), 110, 110);
        painter->drawEllipse(QPoint(0, 0), 46, 46);
        painter->drawEllipse(QPoint(0, 0), 20, 20);
//        painter->drawText(-146, 5, "8");
        painter->drawText(142-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "8");
        painter->drawText(-142-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "8");
//        painter->drawText(136, 5, "8");
        painter->drawText(206-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "7");
        painter->drawText(-206-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "7");
//        painter->drawText(-210, 5, "7");
//        painter->drawText(200, 5, "7");
        if(aktiivne)
            painter->setPen(Qt::black);
        else
            painter->setPen(Qt::gray);
        painter->drawText(270-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "6");
        painter->drawText(-270-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "6");
//        painter->drawText(-274, 5, "6");
//        painter->drawText(264, 5, "6");
        painter->drawText(334-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "5");
        painter->drawText(-334-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "5");
//        painter->drawText(-338, 5, "5");
//        painter->drawText(328, 5, "5");
        painter->drawText(398-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "4");
        painter->drawText(-398-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "4");
//        painter->drawText(-402, 5, "4");
//        painter->drawText(392, 5, "4");
        painter->drawText(462-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "3");
        painter->drawText(-462-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "3");
//        painter->drawText(-466, 5, "3");
//        painter->drawText(456, 5, "3");
        painter->drawText(526-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "2");
        painter->drawText(-526-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "2");
//        painter->drawText(-530, 5, "2");
//        painter->drawText(520, 5, "2");
        painter->drawText(590-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "1");
        painter->drawText(-590-20, -20, 40, 40, Qt::AlignCenter | Qt::AlignVCenter, "1");
//        painter->drawText(-594, 5, "1");
//        painter->drawText(584, 5, "1");
    }
    zoom();
//    this->setPixmap(QPixmap::fromImage(*pilt));
}

//void Markleht::mouseReleaseEvent(QMouseEvent *event)
//{
//    kaugeimX /= 1.5;
////    Lask l;
////    l.set10Lask(84);
////    l.setXY(QPoint(4, 8));
////    joonistaLask(l);
//}

void Markleht::reset()
{
    kaugeimX = kaugeimY = markleheRaadius = kordaja = 0;
    zoomiAste = 0;
    eelmineLask.clear();

//    this->setGeometry(0, 0, this->width(), this->height());
    joonistaLeht();
}

void Markleht::resizeEvent(QResizeEvent * e)
{
    zoom();
}

void Markleht::setAktiivne(bool a)
{
    aktiivne = a;
}

void Markleht::setNimi(QString n)
{
    nimi = n;
}

void Markleht::setRada(QString r)
{
    rada = r;
}

void Markleht::setRelv(int r)
{
    relv = r;
}

void Markleht::setTulemus(QString tul)
{
    tulemus = tul;
}

void Markleht::zoom()
{
    if(kaugeimX < (markleheRaadius / 5))
        kaugeimX = markleheRaadius / 5; //Et sisek¸mnete puhul v‰ga sisse ei zoom'iks
    int h = (kaugeimX * 2) + (kordaja * kaliiber *4);  //Zoom'itava lehe ala, kaugeima koordinaadi j‰rgi + lasu l‰bimııt, et see ikka n‰ha oleks (lasu l‰bimııtu korrutatud senise 2 asemel 4'ga, kuna muidu j‰i ikka lask nime taha
    int w = (kaugeimX * 2) + (kordaja * kaliiber * 4);
    int x = pilt->width() / 2 - w / 2;
    int y = pilt->height() / 2 - h / 2;

    QImage koopia = pilt->copy(x, y, w, h);
    QPainter painter2(&koopia);
    painter2.setBrush(Qt::white);
    QFont font;
    font.setPointSize(h / 15);
    painter2.setFont(font);
    QRect lasuKast(w - w / 4, h - h / 8 - 1, w / 4, h / 8);
    QRect nimeKast(0, 0, font.pointSize() * nimi.length(), h / 8);
    QRect tulKast(w - font.pointSize() * (tulemus.length() - 1) - 1, 0, font.pointSize() * (tulemus.length() - 1), h / 8);
    QRect rajaKast(0, h - h / 8 - 1, w / 5, h / 8);

    painter2.drawRect(nimeKast);
    nimeKast.setLeft(5);
    painter2.drawText(nimeKast, Qt::AlignVCenter, nimi);
    if(eelmineLask.get10Lask() != -999){
        painter2.drawRect(lasuKast);
//        lasuKast.setLeft(lasuKast.left() + 5);
//        lasuKast.setRight(lasuKast.right() - 5);
        QString lask = QString("%1").arg(eelmineLask.getFLask()).replace('.', ',');
        if(!lask.contains(',')) //Kui on komakohtadega laskmine, aga lask on t‰isarv
            lask.append(",0");
        painter2.drawText(lasuKast, Qt::AlignVCenter | Qt::AlignHCenter, lask);
    }
    if(tulemus != "0" && tulemus != "0,0" && tulemus != "0.0"){
        painter2.drawRect(tulKast);
        painter2.drawText(tulKast, Qt::AlignVCenter | Qt::AlignHCenter, tulemus);
    }else{  //Kui tulemus on 0, siis tıen‰oliselt, kuigi ei pruugi, on tegu proovilasuga
        QRectF rect = QRectF(w - w / 4, 0, w / 4, w / 4);

        QPainterPath path;
        path.moveTo(rect.right(), rect.top());
        path.lineTo(rect.topLeft());
        path.lineTo(rect.bottomRight());
        path.lineTo(rect.right(), rect.top());

        //painter2.fillPath(path, QBrush(QColor ("black")));
        painter2.setBrush(Qt::black);   //Kolmnurk mustalt, valge servaga
        QPen vanaPliiats = painter2.pen();
        QPen pliiats;
        pliiats.setColor(Qt::white);
        pliiats.setWidth(3);
        painter2.setPen(pliiats);
//        painter2.setPen(Qt::white);
        painter2.drawPath(path);

        painter2.setBrush(Qt::white);   //Tagasi algseks seadistamine
        painter2.setPen(vanaPliiats);
    }
    painter2.drawRect(rajaKast);
    painter2.drawText(rajaKast, Qt::AlignVCenter | Qt::AlignHCenter, rada);
    this->setPixmap(QPixmap::fromImage(koopia.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation)/**pilt).copy(x, y, w, h*/));
}
