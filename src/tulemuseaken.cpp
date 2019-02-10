#include <QtGui>
#include "tulemuseaken.h"

TulemuseAken::TulemuseAken(QWidget *parent) : QWidget(parent)
{
	if(this->objectName().isEmpty()){
		this->setObjectName("TulemuseAken");
	}
	est = new QLocale(QLocale::Estonian, QLocale::Estonia);
	//QFont font;
	mitmesLask = 0;
    kiri.setPointSize(40);
	kiri.setBold(true);
	setFont(kiri);
	kiriKriipsutatud = kiri;
	kiriKriipsutatud.setStrikeOut(true);
	setBackgroundRole(QPalette::Base);
	setAutoFillBackground(true);
	pealKiri = new QString("Pealkiri");
	nim = new QString("Nimi");
	rad = new QString("Rada");
	las = new QString("0. lask");
	see = new QString("Finaal");
	kok = new QString("Kokku");
	vah = new QString("Vahe");
    ruudustik = new QGridLayout();
    vRuudustik = new QVBoxLayout();
	for(int i = 0; i < 8; i++){
		kohal[i] = true;
		//summa[i] = new QString("0,0");
		seeria[i] = new QString("0,0");
		nimi[i] = new QString("Nimi");
		//rada[i] = new QString(est->toString(i + 1) + ".");
		rada[i] = new QString(QChar(i + 65));
		lask[i] = new QString("...");
		vahe[i] = new QString("0,0");
		koht[i] = new QString(est->toString(i + 1) + ".");

        marklehed[i] = new Markleht(this);
        if(i >= 4)
            ruudustik->addWidget(marklehed[i], 1, i - 4);
        else
            ruudustik->addWidget(marklehed[i], 0, i);
        marklehed[i]->hide();
	}
	
	fontAct = new QAction(tr("Muuda kirja..."), this);
	fontAct->setStatusTip(tr("Muuda selle aknda kirja"));
	connect(fontAct, SIGNAL(triggered()), this, SLOT(muudaFont()));
	fullScreenAct = new QAction(tr("Täisekraan vaade"), this);
	fullScreenAct->setShortcut(tr("F"));
	fullScreenAct->setStatusTip(tr("Näita seda akent täisekraan vaates"));
	connect(fullScreenAct, SIGNAL(triggered()), this, SLOT(fullScreen()));
	popup = new QMenu(this);
	popup->addAction(fontAct);
	popup->addAction(fullScreenAct);
	(void) new QShortcut(Qt::Key_F, this, SLOT(fullScreen()));
    resultsImage = new QImage(1600, 1200, QImage::Format_ARGB32);
    halfHeightResultsImage = new QImage(1600, 600, QImage::Format_ARGB32);
    resultsPainter = new QPainter(resultsImage);
    halfHeightResultsPainter = new QPainter(halfHeightResultsImage);
	silt = new QLabel(this);
	silt->setBackgroundRole(QPalette::Base);
	silt->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	silt->setScaledContents(true);
    silt->setGeometry(0, 0, this->width(), this->height());
    resultsPainter->setFont(kiri);
    halfHeightResultsPainter->setFont(kiri);
    silt->setPixmap(QPixmap::fromImage(*resultsImage));
//    silt->hide();

//#ifdef PROOV
    vRuudustik->addWidget(silt);
    vRuudustik->addLayout(ruudustik);
    this->setLayout(vRuudustik);
//#endif
//#ifdef PROOV
//    markleht = new Markleht(0, "Erm", "A",this);
//    markleht->setGeometry(0, 0, 400, 400);
//    markleht2 = new Markleht(1, "Rumjantseva", "H", this);
//    markleht2->setGeometry(400, 0, 400, 400);
//#endif
}

TulemuseAken::~TulemuseAken()
{
}

void TulemuseAken::naitaLehed()
{
    for(int i = 0; i < 8; i++){
//        marklehed[i]->reset();
        marklehed[i]->show();
    }
    silt->setPixmap(QPixmap::fromImage(*halfHeightResultsImage));
}

void TulemuseAken::naitaNimed()
{
    silt->show();
    joonista();
}

void TulemuseAken::peidaLehed()
{
    for(int i = 0; i < 8; i++){
        marklehed[i]->hide();
    }
    silt->setPixmap(QPixmap::fromImage(*resultsImage));
}

void TulemuseAken::peidaNimed()
{
    silt->hide();
}

void TulemuseAken::puhastaLehed()
{
    for(int i = 0; i < 8; i++){
        marklehed[i]->reset();
    }
}

void TulemuseAken::resizeEvent(QResizeEvent *event)
{
	silt->setGeometry(0, 0, this->width(), this->height());
}

void TulemuseAken::contextMenuEvent(QContextMenuEvent *event)
{
	popup->exec(event->globalPos());
}

void TulemuseAken::mouseDoubleClickEvent(QMouseEvent *event)
{
	fullScreen();
}

void TulemuseAken::fullScreen()
{
	if(this->windowState() == Qt::WindowFullScreen)
	this->setWindowState(Qt::WindowNoState);
	else this->setWindowState(Qt::WindowFullScreen);
}

void TulemuseAken::muudaFont()
{
	setFont(QFontDialog::getFont(0, this->font()));
	kiri = font();
	kiriKriipsutatud = kiri;
	kiriKriipsutatud.setStrikeOut(true);
    resultsPainter->setFont(kiri);
	joonista();
}

void TulemuseAken::joonista()
{
    QPen pen(resultsPainter->pen());
    resultsImage->fill(0);
    halfHeightResultsImage->fill(0);

    resultsPainter->drawText(60, 100, *pealKiri);
    resultsPainter->drawText(130, 200, *nim);
    resultsPainter->drawText(650, 200, *rad);
    resultsPainter->drawText(860, 200, *las);
    resultsPainter->drawText(1150, 200, *see);
    resultsPainter->drawText(1400, 200, *vah);
    resultsPainter->drawLine(20, 210, 1580, 210);

    halfHeightResultsPainter->drawText(60, 50, *pealKiri);
    halfHeightResultsPainter->drawText(130, 100, *nim);
    halfHeightResultsPainter->drawText(650, 100, *rad);
    halfHeightResultsPainter->drawText(860, 100, *las);
    halfHeightResultsPainter->drawText(1150, 100, *see);
    halfHeightResultsPainter->drawText(1400, 100, *vah);
    halfHeightResultsPainter->drawLine(20, 110, 1580, 110);

    for(int i = 0; i < 8; i++){
        int y = 270 + 120 * i;

        //Kui on väljalangemise lask ja võrdsed tulemused, siis värvida need punaseks
        if((*las == "12. lask" && i == 6 && *seeria[i] == *seeria[i+1] && *seeria[i] != "0,0")
        || (*las == "12. lask" && i == 7 && *seeria[i] == *seeria[i-1] && *seeria[i] != "0,0")
        || (*las == "14. lask" && i == 5 && *seeria[i] == *seeria[i+1] && *seeria[i] != "0,0")
        || (*las == "14. lask" && i == 6 && *seeria[i] == *seeria[i-1] && *seeria[i] != "0,0")
        || (*las == "16. lask" && i == 4 && *seeria[i] == *seeria[i+1] && *seeria[i] != "0,0")
        || (*las == "16. lask" && i == 5 && *seeria[i] == *seeria[i-1] && *seeria[i] != "0,0")
        || (*las == "18. lask" && i == 3 && *seeria[i] == *seeria[i+1] && *seeria[i] != "0,0")
        || (*las == "18. lask" && i == 4 && *seeria[i] == *seeria[i-1] && *seeria[i] != "0,0")
        || (*las == "20. lask" && i == 2 && *seeria[i] == *seeria[i+1] && *seeria[i] != "0,0")
        || (*las == "20. lask" && i == 3 && *seeria[i] == *seeria[i-1] && *seeria[i] != "0,0")
        || (*las == "22. lask" && i == 1 && *seeria[i] == *seeria[i+1] && *seeria[i] != "0,0")
        || (*las == "22. lask" && i == 2 && *seeria[i] == *seeria[i-1] && *seeria[i] != "0,0")
        || (*las == "24. lask" && i == 0 && *seeria[i] == *seeria[i+1] && *seeria[i] != "0,0")
        || (*las == "24. lask" && i == 1 && *seeria[i] == *seeria[i-1] && *seeria[i] != "0,0")){
            resultsPainter->setPen(QColor(Qt::red));
            halfHeightResultsPainter->setPen(QColor(Qt::red));
        }

        if(!kohal[i]){  //Keda ei ole kohal ja kes välja langes, näidatakse läbi kriipsutatuna
            resultsPainter->setFont(kiriKriipsutatud);
            halfHeightResultsPainter->setFont(kiriKriipsutatud);
        }

        //Kes eelmises ringis välja langesid, näidatakse halliga
        if(i > 8 - ((mitmesLask - 9) / 2)){
            resultsPainter->setPen(QColor(Qt::gray));
            halfHeightResultsPainter->setPen(QColor(Qt::gray));
        }

        resultsPainter->drawText(20, y, 60, 60, Qt::AlignCenter, *koht[i]);
        resultsPainter->drawText(120, y, 500, 60, Qt::AlignLeft | Qt::AlignVCenter, *nimi[i]);
        resultsPainter->drawText(650, y, 150, 60, Qt::AlignCenter, *rada[i]);
        resultsPainter->drawText(875, y, 200, 60, Qt::AlignCenter, *lask[i]);
        resultsPainter->drawText(1150, y, 200, 60, Qt::AlignCenter, *seeria[i]);
        resultsPainter->drawText(1380, y, 200, 60, Qt::AlignCenter, *vahe[i]);

        int halfY = y / 2 - 10;
        halfHeightResultsPainter->drawText(20, halfY, 60, 50, Qt::AlignCenter, *koht[i]);
        halfHeightResultsPainter->drawText(120, halfY, 500, 50, Qt::AlignLeft | Qt::AlignVCenter, *nimi[i]);
        halfHeightResultsPainter->drawText(650, halfY, 150, 50, Qt::AlignCenter, *rada[i]);
        halfHeightResultsPainter->drawText(875, halfY, 200, 50, Qt::AlignCenter, *lask[i]);
        halfHeightResultsPainter->drawText(1150, halfY, 200, 50, Qt::AlignCenter, *seeria[i]);
        halfHeightResultsPainter->drawText(1380, halfY, 200, 50, Qt::AlignCenter, *vahe[i]);

        resultsPainter->setPen(pen);
        halfHeightResultsPainter->setPen(pen);
        resultsPainter->setFont(kiri);
        halfHeightResultsPainter->setFont(kiri);
    }
    //resultsPainter->setFont(kiri);
    if(marklehed[0]->isVisible())
        silt->setPixmap(QPixmap::fromImage(*halfHeightResultsImage));
    else
        silt->setPixmap(QPixmap::fromImage(*resultsImage));
}
