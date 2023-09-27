#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "parseinfo.h"
#include "lexinfo.h"
#include "seminfo.h"

#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , Symbol(0)
{
    ui->setupUi(this);
    ui->file_path->setPlaceholderText("file path");

    lexinfo = new LexInfo;
    parseinfo = new ParseInfo;
    seminfo = new SemInfo;

    connect(ui->get_file, &QPushButton::clicked, this, &MainWindow::GetFile);
    connect(ui->do_lex, &QPushButton::clicked, this, &MainWindow::DoLex);
    connect(ui->do_LL1, &QPushButton::clicked, this, &MainWindow::DoLL1);
    connect(ui->do_down, &QPushButton::clicked, this, &MainWindow::DoDown);
    connect(ui->do_sem, &QPushButton::clicked, this, &MainWindow::DoSem);
}

void MainWindow::GetFile() {
    QString str = ui->file_path->text();
    QFile file(str);
    file.open(QIODevice::ReadOnly);
    ui->file_content->clear();
    int line = 1;
    while (!file.atEnd()) {
        std::string str = file.readLine().toStdString();
        ui->file_content->append(QString::number(line++) + "   " + QString::fromStdString(str));
    }
    file.close();
}

void MainWindow::DoLL1() {
    parseinfo->show();
    parseinfo->StartLL1();
    Symbol = 1;
}

void MainWindow::DoDown() {
    parseinfo->show();
    parseinfo->StartDown();
    Symbol = 2;
}

void MainWindow::DoLex() {
    QString str = ui->file_path->text();
    lexinfo->show();
    lexinfo->StartLex(str);
}

void MainWindow::DoSem() {
    seminfo->show();
    if (Symbol == 1) {
        seminfo->LL1Sem();
    }
    else {
        seminfo->DownSem();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

