#include "mainwindow.h"
#include "PreTask.h"
#include "ParserPreTask.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    preTask();
    getAll();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
