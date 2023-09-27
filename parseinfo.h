#ifndef PARSEINFO_H
#define PARSEINFO_H

#include <QWidget>

namespace Ui {
class ParseInfo;
}

class Node;

class ParseInfo : public QWidget
{
    Q_OBJECT

public:
    explicit ParseInfo(QWidget *parent = nullptr);
    ~ParseInfo();
    void StartDown();
    void StartLL1();

private:
    Ui::ParseInfo *ui;
    void ShowTree(Node *node, bool is_param = false);
};

#endif // PARSEINFO_H
