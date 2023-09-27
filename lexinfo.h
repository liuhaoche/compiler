#ifndef LEXINFO_H
#define LEXINFO_H

#include <QWidget>

namespace Ui {
class LexInfo;
}

class LexInfo : public QWidget
{
    Q_OBJECT

public:
    explicit LexInfo(QWidget *parent = nullptr);
    ~LexInfo();
    void StartLex(const QString &str);

private:
    Ui::LexInfo *ui;
};

#endif // LEXINFO_H
