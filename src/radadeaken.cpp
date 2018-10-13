#include "radadeaken.h"
#include "ui_radadeaken.h"

RadadeAken::RadadeAken(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RadadeAken)
{
    ui->setupUi(this);
    rajad << ui->rada1Kast;
    rajad << ui->rada2Kast;
    rajad << ui->rada3Kast;
    rajad << ui->rada4Kast;
    rajad << ui->rada5Kast;
    rajad << ui->rada6Kast;
    rajad << ui->rada7Kast;
    rajad << ui->rada8Kast;

    nimed << ui->rada1Nimi;
    nimed << ui->rada2Nimi;
    nimed << ui->rada3Nimi;
    nimed << ui->rada4Nimi;
    nimed << ui->rada5Nimi;
    nimed << ui->rada6Nimi;
    nimed << ui->rada7Nimi;
    nimed << ui->rada8Nimi;

    QRegExp rx ("[A-H]");
    rx.setCaseSensitivity(Qt::CaseInsensitive);
    for(int i = 0; i < rajad.count(); i++){
        rajad.at(i)->setValidator (new QRegExpValidator (rx, this));
        connect(rajad.at(i), SIGNAL(editingFinished()), SLOT(suureks()));
        idd << "";
        tulemused << "";
    }
}

void RadadeAken::clear()
{
    for(int i = 0; i < rajad.count(); i++){
        rajad.at(i)->clear();
        nimed.at(i)->clear();
    }
}

//void RadadeAken::closeEvent(QCloseEvent *event){
//    if(this->isAccepted())
//        QMessageBox::warning(this, programmiNimi, QString::fromLatin1("Oled kindel, et soovid programmist väljuda?"), QMessageBox::Ok);
////        event->ignore();
//    else{
//        if(serial != 0)
//        serial->close();
//        if(server != 0)
//        server->close();
//        qApp->quit();
//    }
//}

QString RadadeAken::getId(int i)
{
    if(i < 0 || i >= idd.count())
        return "";
    else return idd.at(i);
}

QString RadadeAken::getNimi(int i)
{
    if(i < 0 || i >= nimed.count())
        return "";
    else return nimed.at(i)->text();
}

int RadadeAken::getRada(int i)
{
    if(i < 0 || i >= rajad.count())
        return -1;
    else if(rajad.at(i)->text() == "A"){
         return  1;
    }else if(rajad.at(i)->text() == "B"){
        return  2;
    }else if(rajad.at(i)->text() == "C"){
        return  3;
    }else if(rajad.at(i)->text() == "D"){
        return  4;
    }else if(rajad.at(i)->text() == "E"){
        return  5;
    }else if(rajad.at(i)->text() == "F"){
        return  6;
    }else if(rajad.at(i)->text() == "G"){
        return  7;
    }else if(rajad.at(i)->text() == "H"){
        return  8;
    }else return -1;
}

int RadadeAken::getRadadeArv()
{
    return rajad.count();
}

bool RadadeAken::setNimi(int i, QString nimi)
{
    if(i < 0 || i >= nimed.count())
        return false;
    else nimed.at(i)->setText(nimi);
    return true;
}

QString RadadeAken::getTulemus(int i)
{
    if(i < 0 || i >= tulemused.count())
        return "";
    else return tulemused.at(i);
}

bool RadadeAken::setRada(int i, int r)
{
    if(i < 0 || i >= rajad.count())
        return false;
    else switch(r){
    case 1 :
        rajad.at(i)->setText("A");
        break;
    case 2 :
        rajad.at(i)->setText("B");
        break;
    case 3 :
        rajad.at(i)->setText("C");
        break;
    case 4 :
        rajad.at(i)->setText("D");
        break;
    case 5 :
        rajad.at(i)->setText("E");
        break;
    case 6 :
        rajad.at(i)->setText("F");
        break;
    case 7 :
        rajad.at(i)->setText("G");
        break;
    case 8 :
        rajad.at(i)->setText("H");
        break;
    }
    return true;
}

bool RadadeAken::setRida(int i, int r, QString nimi, QString id, QString tul)
{
    if(!setNimi(i, nimi))
        return false;

    if(!setRada(i, r))
        return false;

    if(i < 0 || i >= idd.count())
        return false;
    else idd.replace(i, id);

    if(i < 0 || i >= tulemused.count())
        return false;
    else tulemused.replace(i, tul);
    return true;
}

void RadadeAken::suureks()
{
    QLineEdit *saatja = static_cast<QLineEdit*>(sender());
    saatja->setText(saatja->text().toUpper());
}

RadadeAken::~RadadeAken()
{
    delete ui;
}
