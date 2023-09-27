#include "seminfo.h"
#include "ui_seminfo.h"
#include "LL1Parser.h"
#include "DownParser.h"

SemInfo::SemInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SemInfo)
{
    ui->setupUi(this);
}

SemInfo::~SemInfo()
{
    delete ui;
}

QString SemInfo::printIDKind(IDKind i) {
    switch (i)
    {
    case varKind: {
        return "varKind";
    }
    case typeKind: {
        return "typeKind";
    }
    case procKind: {
        return "procKind";
    }
    default:
        return "";
    }
}
QString SemInfo::printTypeKind(Typekind i) {// intTy, charTy, arrayTy, recordTy, boolTy
    switch (i)
    {
    case intTy: {
        return "intTy";
    }
    case charTy: {
        return "charTy";
    }
    case arrayTy: {
        return "arrayTy";
    }
    case recordTy: {
        return "recordTy";
    }
    case boolTy: {
        return "boolTy";
    }
    default:
        return "";
    }
}

//6. 打印符号表:
void SemInfo::PrintSymbTable() {
    ui->textBrowser->append("idName\tType\tKind\t\tLevel\tOff");
    int i = 0;
    while (i <= Level) {
        symbtable* p = Scope[i].next;
        while (p != nullptr) {
            //还需要输出更多的东西！！！！！
            QString infromation;
            infromation = QString::fromStdString(p->idname) + "\t";
            if(p->attrIR.idtype != nullptr)
                infromation += printTypeKind(p->attrIR.idtype->kind) + "\t";
            else
                infromation += "\t";
            infromation += printIDKind(p->attrIR.kind) + "\t\t";
            infromation += QString::number(i);
            if(p->attrIR.kind == varKind) {
                infromation += "\t" + QString::number(p->attrIR.More.VarAttr.off) + "\t";
            }
            ui->textBrowser->append(infromation);
            p = p->next;
        }
        i++;
    }
    ui->textBrowser->append("Deleted SymbolTable:");
   for(size_t j = 0; j < DeleteScope.size(); j++) {
        ui->textBrowser->append("idName\tType\tKind\t\tLevel\tOff");
        symbtable* p = DeleteScope[j].second.next;
        int l = DeleteScope[j].first;
        while (p != nullptr) {
            //还需要输出更多的东西！！！！！
            QString infromation;
            infromation = QString::fromStdString(p->idname) + "\t";
            if(p->attrIR.idtype != nullptr)
                infromation += printTypeKind(p->attrIR.idtype->kind) + "\t";
            infromation += printIDKind(p->attrIR.kind) + "\t\t";
            infromation += QString::number(l);
            if(p->attrIR.kind == varKind)
                infromation += "\t" + QString::number(p->attrIR.More.VarAttr.off) + "\t";
            ui->textBrowser->append(infromation);
            p = p->next;
        }
    }
}
//7. 打印错误
QString SemInfo::printErrorInfo(int id) {
    switch(id){
        case 1: {return "The identifier has been defined  ";}
        case 2: {return "The identifier is not difined  ";}
        case 3: {return "The identifier is an undesired category of identifiers  ";}
        case 4: {return "The array subscript is out of bounds  ";}
        case 5: {return "References to array member variables and domain variables are illegal  ";}
        case 6: {return "The left and right types of the assignment statement are incompatible  ";}
        case 7: {return "The left of the assignment statement is not a variable identifier  ";break;}
        case 8: {return "The form argument type does not match in the procedure call   ";}
        case 9: {return "The number of parameters is not the same  ";}
        case 10: {return "The identifier is not the procedure identifier  ";}
        case 11: {return "The conditional part of the if and while statements is not of type bool  ";}
        case 12: {return "The types of the components of the operators in the expression are incompatible  ";}
    }
    return "";
}
void SemInfo::printError() {
    if (ErrorInformation.size() == 0) {
        ui->textBrowser->append("No semantic error!");
    }
    else {
        for (size_t i = 0; i < ErrorInformation.size(); i++) {
            //QString qsPara = QString::fromStdString(ErrorInformation[i].idname);
            ui->textBrowser->append(QString("At line: %1").arg(ErrorInformation[i].line));
            ui->textBrowser->append(printErrorInfo(ErrorInformation[i].id));
            ui->textBrowser->append(QString("Error ID: %1").arg(ErrorInformation[i].id));
        }
    }
}

void SemInfo::LL1Sem() {
    ui->textBrowser->clear();
    Node *node = DownParser();
    Analyze(node);
    PrintSymbTable();
    printError();
    DestroyTable();
    for(size_t i = 0; i < DeleteScope.size(); i++) {
        delete DeleteScope[i].second.next;
    }
    DeleteScope.clear();
    ErrorInformation.clear();
}

void SemInfo::DownSem() {
    ui->textBrowser->clear();
    Node *node = DownParser();
    Analyze(node);
    PrintSymbTable();
    printError();
    DestroyTable();
    for(size_t i = 0; i < DeleteScope.size(); i++) {
        delete DeleteScope[i].second.next;
    }
    DeleteScope.clear();
    ErrorInformation.clear();
}
