#include "parseinfo.h"
#include "ui_parseinfo.h"
#include "DownParser.h"
#include "SynTaxTree.h"
#include "LL1Parser.h"

#include <QDebug>

static int tab_count = 0;

ParseInfo::ParseInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParseInfo)
{
    ui->setupUi(this);
}

ParseInfo::~ParseInfo()
{
    delete ui;
}

void ParseInfo::StartLL1() {
    ui->textBrowser->clear();
    Node *node = LL1Parser();
    if (node == nullptr) {
        ui->textBrowser->append("Error at:");
        ui->textBrowser->append(QString("Line: %1").arg(QString::number(LL1ErrorLine)));
        ui->textBrowser->append(QString("Message: %1").arg(LL1ErrorMessage));
    }
    else {
        ShowTree(node);
        delete node;
    }
}

void ParseInfo::StartDown() {
    ui->textBrowser->clear();
    Node *node = DownParser();
    if (node == nullptr) {
        ui->textBrowser->append("Error at:");
        ui->textBrowser->append(QString("Line: %1").arg(QString::number(ParserErrorLine)));
        ui->textBrowser->append(QString("Left: %1").arg(ParserErrorLeft));
        ui->textBrowser->append(QString("Message: %1").arg(ParserErrorMessage));
    }
    else {
        ShowTree(node);
        delete node;
    }
}

void ParseInfo::ShowTree(Node *node, bool is_param) {
    std::string message;
    for (int i = 0; i < tab_count; ++i)
        message += '\t';
    if (node == nullptr) {
        ui->textBrowser->append("");
        return;
    }
    if (node->nodekind == ProK) {
        message += "ProK";
        ui->textBrowser->append(QString::fromStdString(message));
        ++tab_count;
        ShowTree(node->child[0]);
        ShowTree(node->child[1]);
        ShowTree(node->child[2]);
        --tab_count;
    }
    else if (node->nodekind == PheadK) {
        message += "PheadK ";
        message += node->name[0];
        ui->textBrowser->append(QString::fromStdString(message));
    }
    else if (node->nodekind == TypeK) {
        message += "TypeK";
        ui->textBrowser->append(QString::fromStdString(message));
        ++tab_count;
        ShowTree(node->child[0]);
        --tab_count;
        ShowTree(node->sibling);
    }
    else if (node->nodekind == VarK) {
        message += "VarK";
        ui->textBrowser->append(QString::fromStdString(message));
        ++tab_count;
        ShowTree(node->child[0]);
        --tab_count;
        ShowTree(node->sibling);
    }
    else if (node->nodekind == ProcK) {
        message += "ProcK";
        ui->textBrowser->append(QString::fromStdString(message));
        ++tab_count;
        ShowTree(node->child[0]);
        --tab_count;
    }
    else if (node->nodekind == ProcDeck) {
        message += "ProcDecK ";
        message += node->name[0];
        ui->textBrowser->append(QString::fromStdString(message));
        ++tab_count;
        ShowTree(node->child[0], true);
        ShowTree(node->child[1]);
        ShowTree(node->child[2]);
        --tab_count;
        if (node->sibling)
            ShowTree(node->sibling);
    }
    else if (node->nodekind == StmLK) {
        message += "StmLK";
        ui->textBrowser->append(QString::fromStdString(message));
        ++tab_count;
        ShowTree(node->child[0]);
        --tab_count;
    }
    else if (node->nodekind == Deck) {
        message += "DecK ";
        if (is_param) {
            if (node->attr.ProcAttr.paramt == valparamtype)
                message += "value ";
            else
                message += "var ";
            message += "param: ";
        }
        if (node->kind.dec == CharK) {
            message += "CharK ";
            for (auto &str : node->name) {
                message += str;
                message += " ";
            }
            ui->textBrowser->append(QString::fromStdString(message));
        }
        else if (node->kind.dec == IntegerK) {
            message += "IntegerK ";
            for (auto &str : node->name) {
                message += str;
                message += " ";
            }
            ui->textBrowser->append(QString::fromStdString(message));
        }
        else if (node->kind.dec == Idk) {
            message += "IdK ";
            message += node->type_name;
            message += " ";
            for (auto &str : node->name) {
                message += str;
                message += " ";
            }
            ui->textBrowser->append(QString::fromStdString(message));
        }
        else if (node->kind.dec == ArrayK) {
            message += "ArrayK ";
            message += "Low ";
            message += std::to_string(node->attr.ArrayAttr.low);
            message += " ";
            message += "Up ";
            message += std::to_string(node->attr.ArrayAttr.up);
            message += " ";
            message += "BaseType ";
            if (node->attr.ArrayAttr.childType == IntegerK)
                message += "IntegerK ";
            else
                message += "CharK ";
            for (auto &str : node->name) {
                message += str;
                message += " ";
            }
            ui->textBrowser->append(QString::fromStdString(message));
        }
        else {
            message += "RecordK ";
            for (auto &str : node->name) {
                message += str;
                message += " ";
            }
            ui->textBrowser->append(QString::fromStdString(message));
            ++tab_count;
            ShowTree(node->child[0]);
            --tab_count;
        }
        if (node->sibling)
            ShowTree(node->sibling);
    }
    else if (node->nodekind == StmtK) {
        message += "StmtK ";
        if (node->kind.stmt == IfK) {
            message += "IfK ";
            ui->textBrowser->append(QString::fromStdString(message));
            ++tab_count;
            ShowTree(node->child[0]);
            ShowTree(node->child[1]);
            ShowTree(node->child[2]);
            --tab_count;
        }
        else if (node->kind.stmt == WhileK) {
            message += "WhileK ";
            ui->textBrowser->append(QString::fromStdString(message));
            ++tab_count;
            ShowTree(node->child[0]);
            ShowTree(node->child[1]);
            --tab_count;
        }
        else if (node->kind.stmt == AssignK) {
            message += "AssignK ";
            ui->textBrowser->append(QString::fromStdString(message));
            ++tab_count;
            ShowTree(node->child[0]);
            ShowTree(node->child[1]);
            --tab_count;
        }
        else if (node->kind.stmt == ReadK) {
            message += "ReadK ";
            message += node->name[0];
            ui->textBrowser->append(QString::fromStdString(message));
        }
        else if (node->kind.stmt == WriteK) {
            message += "AssignK ";
            ui->textBrowser->append(QString::fromStdString(message));
            ++tab_count;
            ShowTree(node->child[0]);
            --tab_count;
        }
        else if (node->kind.stmt == CallK) {
            message += "CallK ";
            message += node->name[0];
            ui->textBrowser->append(QString::fromStdString(message));
            ++tab_count;
            ShowTree(node->child[0]);
            --tab_count;
        }
        else if (node->kind.stmt == ReturnK){
            message += "ReturnK ";
            ui->textBrowser->append(QString::fromStdString(message));
        }
        else {
            qDebug() << node->kind.stmt;
        }
        if (node->sibling)
            ShowTree(node->sibling);
    }
    else if (node->nodekind == ExpK) {
        message += "ExpK ";
        if (node->kind.exp == OpK) {
            message += "OpK ";
            if (node->attr.ExpAttr.op == LT)
                message += "LT";
            else if (node->attr.ExpAttr.op == EQ)
                message += "EQ";
            else if (node->attr.ExpAttr.op == PLUS)
                message += "PLUS";
            else if (node->attr.ExpAttr.op == MINUS)
                message += "MINUS";
            else if (node->attr.ExpAttr.op == TIMES)
                message += "TIMES";
            else {
                message += "OVER";
            }
            ui->textBrowser->append(QString::fromStdString(message));
            ++tab_count;
            ShowTree(node->child[0]);
            ShowTree(node->child[1]);
            --tab_count;
        }
        else if (node->kind.exp == ConstK) {
            message += "ConstK ";
            message += std::to_string(node->attr.ExpAttr.val);
            ui->textBrowser->append(QString::fromStdString(message));
        }
        else {
            if (node->attr.ExpAttr.varkind == IdV) {
                message += "IdV ";
                message += node->name[0];
                ui->textBrowser->append(QString::fromStdString(message));
            }
            else if (node->attr.ExpAttr.varkind == ArrayMembV) {
                message += "ArrayMembV ";
                message += node->name[0];
                ui->textBrowser->append(QString::fromStdString(message));
            }
            else if (node->attr.ExpAttr.varkind == FieldMembV) {
                message += "FieldMembV ";
                message += node->name[0];
                ui->textBrowser->append(QString::fromStdString(message));
                ++tab_count;
                ShowTree(node->child[0]);
                --tab_count;
            }
        }
        if (node->sibling != nullptr)
            ShowTree(node->sibling);
    }
}
