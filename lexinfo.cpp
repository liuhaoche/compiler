#include "lexinfo.h"
#include "ui_lexinfo.h"
#include "lexical.h"

LexInfo::LexInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LexInfo)
{
    ui->setupUi(this);
}

void LexInfo::StartLex(const QString &str) {
    lexical(str);
    ui->textBrowser->clear();
    ui->tableWidget->setRowCount(tokens.size());
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "LINE NO" << "LEX" << "SEM");
    size_t n = tokens.size();
    for (size_t i = 0; i < n; ++i) {
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(tokens[i].lex)));
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(tokens[i].rownumber)));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(tokens[i].sem)));
    }
    if (lexical_error_line.size() != 0) {
        ui->textBrowser->append(QString::number(lexical_error_line.size()));
        for (size_t i = 0; i < lexical_error_message.size(); ++i) {
            ui->textBrowser->append(QString("Error at line %1").arg(QString::number(lexical_error_line[i])));
            ui->textBrowser->append(QString("Message: %1").arg(lexical_error_message[i]));
        }
    }
    else {
        ui->textBrowser->setText("Success!!!");
    }
}

LexInfo::~LexInfo()
{
    delete ui;
}
