#include <QtGui>

#include "valikkast.h"

extern QString programmiNimi;
extern QString dir;

ValikKast::ValikKast(Seaded* s, QWidget *parent) : QDialog(parent)
{
        ui.setupUi(this);
        avamine = false;
        seaded = s;
        connect(ui.valjuNupp, SIGNAL(clicked()), qApp, SLOT(quit()));
}

ValikKast::~ValikKast()
{
}

void ValikKast::closeEvent(QCloseEvent *event)
{	//Vajalik, et ei saaks ristist kinni pannes sellest mööda hiilida
        if(avamine){
            QDir di(dir);
            di.cdUp();
            QFile fail(di.absolutePath() + "/Alg.ini");
                if(fail.open(QIODevice::WriteOnly | QIODevice::Text)){
                        QTextStream valja(&fail);
                        valja << programmiNimi + " seadete fail.\n";
                        valja << "Kui olete midagi siin ära rikkunud, siis lihtsalt kustutage see fail"
                                        " ja programm teeb uue.\n";
                        valja << ui.voistluseNimi->text() << "\n" << ui.alaNimi->text() << "\n"
                                << seaded->ui.lineEdit->text() << "\n" << seaded->ui.comboBox->currentIndex();
                }
                event->accept();
        }else{
                if(ui.voistluseNimi->text() != "" && ui.alaNimi->text() != ""){
                    QDir di(dir);
                    di.cdUp();
                    QFile fail(di.absolutePath() + "/Alg.ini");
                        if(fail.open(QIODevice::WriteOnly | QIODevice::Text)){
                                QTextStream valja(&fail);
                                valja << programmiNimi + " seadete fail.\n";
                                valja << "Kui olete midagi siin ära rikkunud, siis lihtsalt kustutage see fail"
                                                " ja programm teeb uue.\n";
                                valja << ui.voistluseNimi->text() << "\n" << ui.alaNimi->text() << "\n"
                                        << seaded->ui.lineEdit->text() << "\n" << seaded->ui.comboBox->currentIndex();
                        }
                        event->accept();
                }else{
                        QMessageBox::warning(this, programmiNimi, tr("Palun täitke kõik väljad!"),
                                        QMessageBox::Ok);
                        event->ignore();
                }
        }
}

void ValikKast::on_avaNupp_clicked()
{
        QFile fail(QFileDialog::getOpenFileName(this, tr("Ava"), "Tulemused",
                                tr("Finaali fail (*.fnl)")));
        QString nimi = fail.fileName().section("/", -1, -1);
        if(nimi.isEmpty()) return;
        if(nimi.endsWith(".fnl"))
                nimi.chop(QString(".fnl").length());
        ui.alaNimi->setText(nimi);
        QDir dir(fail.fileName());
        dir.cdUp();
        ui.voistluseNimi->setText(dir.dirName());
        //dir.cdUp();
        //seaded->ui.lineEdit->setText(dir.path());
        //avamine = true;
        close();
}
