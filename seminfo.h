#ifndef SEMINFO_H
#define SEMINFO_H

#include <QWidget>
#include "SemAnalyze.h"

namespace Ui {
class SemInfo;
}

class SemInfo : public QWidget
{
    Q_OBJECT

public:
    explicit SemInfo(QWidget *parent = nullptr);
    ~SemInfo();
    void DownSem();
    void LL1Sem();

private:
    Ui::SemInfo *ui;
    void printError();
    QString printErrorInfo(int id);
    QString printIDKind(IDKind i);
    QString printTypeKind(Typekind i);
    void PrintSymbTable();
};

#endif // SEMINFO_H
