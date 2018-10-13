#include <QtGlobal>
#if QT_VERSION >= 0x050000
	#include <QApplication>
	#else
	#include <QtGui/QApplication>
#endif
#include "qfinaalmain.h"
#include "version.h"

QString programmiNimi = VER_INTERNALNAME_STR; //"Finaal 1.3"; // TODO uuendada nime
QString versioon = VER_PRODUCTVERSION_STR; //"1.3.x";
QString aasta = VER_AASTA;
QString argument;
//QDir asukoht;

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(finaal);
	
	QApplication a(argc, argv);
	//        asukoht = QDir(argv[0]);
	//        asukoht.cdUp();
	a.setApplicationName(programmiNimi);
    a.setOrganizationName("Ümeramees");
	QFinaalMain w;
	w.show();
	a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
	return a.exec();
}
