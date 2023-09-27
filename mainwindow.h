#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class LexInfo;
class ParseInfo;
class SemInfo;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void GetFile();
    void DoLex();
    void DoDown();
    void DoLL1();
    void DoSem();

private:
    Ui::MainWindow *ui;
    LexInfo *lexinfo;
    ParseInfo *parseinfo;
    SemInfo *seminfo;
    int Symbol;
};
#endif // MAINWINDOW_H
