#ifndef RADADEAKEN_H
#define RADADEAKEN_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#ifdef PROOV
#include <QDebug>
#endif

namespace Ui {
class RadadeAken;
}

class RadadeAken : public QDialog
{
    Q_OBJECT
    enum{
        A = 1,
        B = 2,
        C = 3,
        D = 4,
        E = 5,
        F = 6,
        G = 7,
        H = 8
    };
    
public:
    explicit RadadeAken(QWidget *parent = 0);
    ~RadadeAken();
    
private:
    Ui::RadadeAken *ui;
    QList<QLineEdit*> rajad;
    QList<QLabel*> nimed;
    QStringList idd;
    QStringList tulemused;
    bool setNimi(int i, QString nimi);
    bool setRada(int i, int r);
//    void closeEvent(QCloseEvent *event);

public slots:
    void clear();
    QString getNimi(int i);
    int getRada(int i);
    int getRadadeArv();
    QString getTulemus(int i);
    QString getId(int i);

//    QString getReasNimi(int i);
//    int getReasRada(int i);
//    QString getReasTulemus(int i);
//    QString getReasId(int i);
    bool setRida(int i, int r, QString nimi, QString id, QString tul);
    void suureks();
};

#endif // RADADEAKEN_H
