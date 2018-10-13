#ifndef QFINAAL_H_
#define QFINAAL_H_

#include <QtCore/QVariant>

#if QT_VERSION >= 0x050000
    #include <QAction>
    #include <QApplication>
    #include <QButtonGroup>
    #include <QHBoxLayout>
    #include <QLabel>
    #include <QLineEdit>
    #include <QSpacerItem>
    #include <QVBoxLayout>
    #include <QGridLayout>
    #include <QInputDialog>
    #include <QScrollArea>
    #include <QScrollBar>
    #include <QMenu>
    #include <QToolBar>
    #include <QWidget>
#else
    #include <QtGui/QAction>
    #include <QtGui/QApplication>
    #include <QtGui/QButtonGroup>
    #include <QtGui/QHBoxLayout>
    #include <QtGui/QLabel>
    #include <QtGui/QLineEdit>
    #include <QtGui/QSpacerItem>
    #include <QtGui/QVBoxLayout>
    #include <QtGui/QGridLayout>
    #include <QtGui/QWidget>
#endif

#include <QCheckBox>
	
class QFinaal : public QWidget
{
    Q_OBJECT

public:
    QAction *naitaIDAct;
    QStringList id;
    QGridLayout *kast;  //Lasud, seeriad ja summa
    QGridLayout *kastVasak; //Nimed ja tulemused
    QHBoxLayout *hKast;
    QVBoxLayout *kastParem; //Finaali summa
    QScrollArea *area;  //Seeriate kerimiseks
    QWidget *sisu;  //Pind, kuhu paigutada seeriad
    QWidget *vasakSisu; //Pind, kuhu paigutada nimed ja tulemused
    QCheckBox *kohal[8];
    //QHBoxLayout *rida[8], *lisarida[8], *sildiRida;
    QLineEdit *lask[24][8];
    QLineEdit *nimi[8];
    QLineEdit *tulemus[8];
    QLineEdit *lisaLask[24][8];
    QLabel *seeria1[8], *seeria2[8], *seeria[8];
    QLabel /**summa[8],*/ *lisaLabel[8], *silt[30];
    int iSumma[8];
    int idIndeks;
    QMenu *hupikMenuu;
//    QPoint menuuPos;
    QSpacerItem *vahe[8], *sildiVahe;
    QLocale *est;
    QFinaal(QWidget *parent = 0);
    virtual ~QFinaal();

public slots:
    void naitaHMenuud(QPoint);
    void naitaIdAken();
};

#endif /*QFINAAL_H_*/
