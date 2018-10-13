#include "valikkast.h"

ValikKast::ValikKast(QWidget *parent)
: QDialog(parent)
{
	ui.setupUi(this);
	ui.failiNimi->setText("");
	connect(ui.avaNupp, SIGNAL(clicked()), this, SLOT(avamine()));
	connect(ui.voistluseNimi, SIGNAL(textEdited(QString)), this, SLOT(muutus(QString)));
	connect(ui.uusNupp, SIGNAL(clicked()), this, SLOT(uus()));
	connect(ui.edasiNupp, SIGNAL(clicked()), this, SLOT(edasi()));
}

void ValikKast::avamine()
{
	QString uusNimi = QFileDialog::getOpenFileName(this, tr("Ava fail"), ui.failiNimi->text(),
	tr("Finaali fail (*.fnl)"));
	if(!uusNimi.isEmpty()){
		ui.failiNimi->setText(uusNimi);
		//failiNimi.chop(4);
		QFile fail(ui.failiNimi->text());
		if(fail.open(QIODevice::ReadOnly)){
			QDataStream sisse(&fail);
			quint32 kontroll, versioon;
			sisse >> kontroll >> versioon;
			if(kontroll != 0x00FA3058){
				QMessageBox::critical(this, tr("Protokollitaja"), tr("Vigane või vale fail!"),
				QMessageBox::Ok);
				return;
			}
            if(versioon >= 12 && versioon <= 15){
				QString rida;
				sisse >> rida;
				ui.voistluseNimi->setText(rida);
				rida.clear();
				sisse >> rida;
				ui.alaNimi->setText(rida);
            }else if(versioon == 11){
				QString nimi = fail.fileName().section("/", -1, -1);
				if(nimi.endsWith(".fnl"))
				nimi.chop(QString(".fnl").length());
				ui.alaNimi->setText(nimi);
				nimi = fail.fileName().section("/", -2, -2);
				ui.voistluseNimi->setText(nimi);
			}else QMessageBox::critical(this, tr("Protokollitaja"), tr("Vale versiooni fail!"),
			QMessageBox::Ok);
			fail.close();
		}
		//this->accept();
	}
}

void ValikKast::edasi()
{
	if(ui.voistluseNimi->text().isEmpty() || ui.failiNimi->text().isEmpty() || ui.alaNimi->text().isEmpty()){
		QMessageBox::critical(this, "Protokollitaja", "Kõik lahtrid peavad olema täidetud!",
		QMessageBox::Ok);
		return;
	}
	QFile fail(ui.failiNimi->text());
	if(!fail.open(QIODevice::ReadOnly)){
		if(QMessageBox::question(this, "Küsimus", "Sellise nimega faili ei ole. Kas soovite selle luua?",
		QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok){
			fail.close();
			if(fail.open(QIODevice::WriteOnly)){
				QDataStream valja(&fail);
				valja << (quint32)0x00FA3058;	//Kontrollarv
                valja << (qint32)14;			//Millise programmi versiooni failiga on tegu
				valja.setVersion(QDataStream::Qt_4_3);
				valja << ui.voistluseNimi->text();
				valja << ui.alaNimi->text();
			}else QMessageBox::critical(this, "Protokollitaja", "Ei õnnestu faili luua. Kontrollige kas "
			"teil on sinna kausta kirjutamis õigus.", QMessageBox::Ok);
		}else return;
	}
	this->accept();
}

void ValikKast::muutus(QString)
{
	ui.failiNimi->setText("");
}

void ValikKast::uus()
{
	QString uusNimi = QInputDialog::getText(this, "Sisestage uue võistluse nimi", "Võistluse nimi:",
	QLineEdit::Normal, ui.voistluseNimi->text());
	QString uusHarjutus = QInputDialog::getText(this, "Sisestage uue harjutuse nimi", "Harjutus:",
	QLineEdit::Normal, ui.alaNimi->text());
	QString failiNimi = QFileDialog::getSaveFileName(this, tr("Salvesta"), uusHarjutus, tr("Finaali fail (*.fnl)"));
	if(!failiNimi.endsWith(".fnl"))
	failiNimi.append(".fnl");
	if(!uusNimi.isEmpty() && !failiNimi.isEmpty() && failiNimi != ".fnl" && !uusHarjutus.isEmpty()){
		ui.voistluseNimi->setText(uusNimi);
		ui.alaNimi->setText(uusHarjutus);
		ui.failiNimi->setText(failiNimi);
	}
}

ValikKast::~ValikKast()
{
	
}
