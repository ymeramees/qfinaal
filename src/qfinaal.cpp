#include <QtGui>
#include "qfinaal.h"

QFinaal::QFinaal(QWidget *parent) : QWidget(parent)
{
    if(this->objectName().isEmpty()){
        this->setObjectName("QFinaalClass");
    }
    est = new QLocale(QLocale::Estonian, QLocale::Estonia);
    QFont font;
    font.setPointSize(10);
    this->setFont(font);
    hKast = new QHBoxLayout(this);
    area = new QScrollArea();
    area->setWidgetResizable(true);
    sisu = new QWidget();
    vasakSisu = new QWidget();
    kast = new QGridLayout(sisu);
    kastVasak = new QGridLayout(vasakSisu);
    kastParem = new QVBoxLayout();
//    hKast->addLayout(kastVasak);
    hKast->addWidget(vasakSisu);
    area->setWidget(sisu);
    hKast->addWidget(area);
    hKast->addLayout(kastParem);
//    area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    kast->setColumnMinimumWidth(2, 20);
    kastVasak->setColumnMinimumWidth(2, 20);
    kast->setHorizontalSpacing(10);
    kastVasak->setHorizontalSpacing(10);
    kast->setVerticalSpacing(20);
    kastVasak->setVerticalSpacing(20);
    kastParem->setSpacing(20);
    kast->setContentsMargins(11, 11, 11, 11);
    kastVasak->setContentsMargins(11, 11, 11, 11);
    kastParem->setContentsMargins(11, 11, 11, 11);
    kast->setObjectName("kast");
    kastVasak->setObjectName("kastVasak");
    kastParem->setObjectName("kastParem");
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(naitaHMenuud(QPoint)));

    naitaIDAct = new QAction("ID...", this);
    naitaIDAct->setStatusTip("Vaata/muuda laskuri ID'd");
    connect(naitaIDAct, SIGNAL(triggered()), this, SLOT(naitaIdAken()));

    hupikMenuu = new QMenu(this);
    hupikMenuu->addAction(naitaIDAct);

    /*sildiRida = new QHBoxLayout();
        sildiRida->setSpacing(6);
    sildiRida->setObjectName("sildiRida");*/
    //sildiVahe = new QSpacerItem(40, 20, QSizePolicy::Expanding);
    for(int i = 0; i < 30; i++){
        silt[i] = new QLabel(this);
        silt[i]->setObjectName("silt" + i);
        silt[i]->setText(QString("%1. Lask").arg(i - 4));
    }

    silt[0]->setText("Kohal");
    silt[1]->setText("Nimi");
    //silt[0]->setMinimumWidth(nimi[0]->width());
    silt[2]->setText("Tulemus");
    //silt[1]->setMinimumWidth(tulemus[0]->width());
    silt[3]->setText("1. Lask");
    silt[4]->setText("2. Lask");
    silt[5]->setText("3. Lask");
    silt[6]->setText("4. Lask");
    silt[7]->setText("5. Lask");
    silt[8]->setText("summa");
    silt[9]->setText("6. Lask");
    silt[10]->setText("7. Lask");
    silt[11]->setText("8. Lask");
    silt[12]->setText("9. Lask");
    silt[13]->setText("10. Lask");
    silt[14]->setText("summa");
    silt[29]->setText("Finaal");
    for(int i = 0; i < 29; i++){
        if(i < 3) kastVasak->addWidget(silt[i], 0, i);
        else kast->addWidget(silt[i], 0, i - 3);
    }
    kastParem->addWidget(silt[29]);
    //vKast->addLayout(sildiRida);
    for(int i = 0; i < 8; i++){
        /*rida[i] = new QHBoxLayout();
            rida[i]->setSpacing(6);
        rida[i]->setObjectName(QString("rida" + i));*/
        id << QString("%1").arg(i + 1);
        kohal[i] = new QCheckBox(this);
        kohal[i]->setChecked(true);
        kohal[i]->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(kohal[i], SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(naitaHMenuud(QPoint)));
        kastVasak->addWidget(kohal[i], (i+1)*2-1, 0);
        nimi[i] = new QLineEdit(this);
        nimi[i]->setObjectName(QString("nimi" + i));
        //nimi[i]->setFont(font);
        nimi[i]->setMinimumWidth(80);
//        nimi[i]->setFixedWidth(100);
        nimi[i]->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(nimi[i], SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(naitaHMenuud(QPoint)));
        kastVasak->addWidget(nimi[i], (i+1)*2-1, 1);
        tulemus[i] = new QLineEdit(this);
        tulemus[i]->setObjectName(QString("tulemus" + i));
        //tulemus[i]->setFont(font);
        tulemus[i]->setMaximumWidth(56);
//        tulemus[i]->setFixedWidth(56);
        tulemus[i]->setText("0");
        kastVasak->addWidget(tulemus[i], (i+1)*2-1, 2);
        /*vahe[i] = new QSpacerItem(40, 20, QSizePolicy::Expanding);
            kast->addItem(vahe[i]);
            lisarida[i] = new QHBoxLayout();
            lisarida[i]->setSpacing(6);
        lisarida[i]->setObjectName(QString("lisarida" + i));*/
        lisaLabel[i] = new QLabel(this);
        lisaLabel[i]->setObjectName(QString("lisaLabel" + i));
        lisaLabel[i]->setText("Lisalasud:");
        kastVasak->addWidget(lisaLabel[i], (i+1)*2, 1);
        lisaLabel[i]->hide();

        seeria1[i] = new QLabel(this);
        seeria1[i]->setObjectName(QString("seeria1" + i));
        seeria1[i]->setText("0,0");
        //kast->addWidget(seeria1[i], (i+1)*2-1, 14);
        seeria2[i] = new QLabel(this);
        seeria2[i]->setObjectName(QString("seeria2" + i));
        seeria2[i]->setText("0,0");
        //kast->addWidget(seeria2[i], (i+1)*2-1, 14);
        for(int j = 0; j < 24; j++){
            lask[j][i] = new QLineEdit(this);
            lask[j][i]->setObjectName(QString("lask" + j + i));
            //lask[j][i]->setFont(font);
//            lask[j][i]->setMaximumWidth(56);
//            lask[j][i]->setMinimumWidth(15);
            lask[j][i]->setFixedWidth(40);
            lask[j][i]->setText("00");
            int tulp;
            if(j < 5){
                tulp = j;
            }else if(j == 5){
                tulp = j+1;
                kast->addWidget(seeria1[i], (i+1)* 2-1, j);
            }else if(j > 5 && j < 10){
                tulp = j + 1;
            }else if(j == 10){
                tulp = j + 2;
                kast->addWidget(seeria2[i], (i+1)* 2-1, j + 1);
            }else{
                tulp = j + 2;
            }
            kast->addWidget(lask[j][i], (i+1)* 2-1, tulp);
            lisaLask[j][i] = new QLineEdit(this);
            lisaLask[j][i]->setObjectName(QString("lisaLask" + j + i));
            lisaLask[j][i]->setMaximumWidth(56);
            lisaLask[j][i]->setMinimumWidth(15);
            lisaLask[j][i]->setText("00");
            if(j < 14)
            kast->addWidget(lisaLask[j][i], (i+1)* 2, j+12);
            lisaLask[j][i]->hide();
        }
        seeria[i] = new QLabel(this);
        seeria[i]->setObjectName(QString("seeria" + i));
        seeria[i]->setText("0,0");
        seeria[i]->setMinimumHeight(lask[0][0]->sizeHint().height());
        lisaLabel[i]->setMinimumHeight(lisaLask[0][0]->sizeHint().height());
//        seeria[i]->setFixedWidth(15);
//        kast->addWidget(seeria[i], (i+1)* 2-1, 26);
        kastParem->addWidget(seeria[i]);
        //summa[i] = new QLabel(this);
        //summa[i]->setObjectName(QString("summa" + i));
        //summa[i]->setText("0,0");
        //kast->addWidget(summa[i], (i+1)*2-1, 15);*/
    }
    QSpacerItem *tuhjusVasak = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QSpacerItem *tuhjus = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QSpacerItem *tuhjusParem = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    kastVasak->addItem(tuhjusVasak, 17, 0);
    kast->addItem(tuhjus, 17, 0);
    kastParem->addItem(tuhjusParem);

    for(int i = 0; i < 8; i++){
        this->setTabOrder(nimi[i], tulemus[i]);
        if(i < 7)
            this->setTabOrder(tulemus[i], nimi[i+1]);
    }
    for(int j = 0; j < 24; j++){
        for(int i = 0; i < 8; i++){
            if(i < 7)
                this->setTabOrder(lask[j][i], lask[j][i+1]);
            else if(j < 23) this->setTabOrder(lask[j][i], lask[j+1][0]);
        }
    }
    for(int j = 0; j < 24; j++){
        for(int i = 0; i < 8; i++){
            if(i < 7)
                this->setTabOrder(lisaLask[j][i], lisaLask[j][i+1]);
            else if(j < 23) this->setTabOrder(lisaLask[j][i], lisaLask[j+1][0]);
        }
    }
#ifdef PROOV
    qDebug() << "Kõrgus: " << vasakSisu->height() << ", Sisu kõrgus: " << sisu->height() << ", MinimumSize: " << sisu->height() + area->horizontalScrollBar()->sizeHint().height();
#endif
    vasakSisu->setFixedWidth(240);
    area->setMinimumHeight(395/*sisu->height() + area->horizontalScrollBar()->sizeHint().height()*/);
//    area->resize(1160, 500);
}

void QFinaal::naitaHMenuud(QPoint p)
{
    QPoint menuuPos = dynamic_cast<QWidget*>(sender())->mapToGlobal(p);
    idIndeks = -1;
    for(int i = 0; i < id.count(); i++) //Leidmine, millisel real klikk toimus
        if(nimi[i]->mapToGlobal(QPoint(0,0)).y() - 5 < menuuPos.y() && nimi[i]->mapToGlobal(QPoint(0,0)).y() + nimi[i]->height() + 8 > menuuPos.y())
            idIndeks = i;

    if(idIndeks == -1 || idIndeks >= this->id.count())  //Kontroll, et id massiiv lõhki ei läheks
        return;

    hupikMenuu->exec(QCursor::pos());
}

void QFinaal::naitaIdAken()
{
    if(idIndeks == -1 || idIndeks >= this->id.count())  //Kontroll, et id massiiv lõhki ei läheks
        return;

    bool ok = false;
    int uusId = QInputDialog::getInt(this, "Sisestage uus laskuri ID", "ID:", this->id[idIndeks].toInt(), 0, 99999, 1, &ok);
    if(ok)
        this->id[idIndeks] = QString("%1").arg(uusId);
}

QFinaal::~QFinaal()
{
}

