#include "seaded.h"

Seaded::Seaded(QWidget *parent)
: QDialog(parent)
{
	ui.setupUi(this);
	//connect(ui.sirviNupp, SIGNAL(clicked()), this, SLOT(sirvi()));
	connect(ui.loobuNupp, SIGNAL(clicked()), this, SLOT(loobu()));
}

Seaded::~Seaded()
{
	
}

/*void Seaded::sirvi()
	{
	QString dir = QFileDialog::getExistingDirectory(this, "Tulemuste kausta asukoht");
	if(!dir.isEmpty()) ui.lineEdit->setText(dir);
	//if(ui.lineEdit->text().contains(QDir::currentPath()))
	ui.lineEdit->setText(ui.lineEdit->text().remove(QDir::currentPath()+"/"));
}*/

void Seaded::loobu()
{
	//ui.lineEdit->setText(vanaDir);
	ui.comboBox->setCurrentIndex(vanaIndex);
    ui.valjalangemiseBox->setChecked(vanaLangemine);
    ui.relvaValik->setCurrentIndex(vanaRelv);
	close();
}
