#include "PreTask.h"

#include <QString>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QFile>
#include <iostream>

std::vector<Production> Productions;
std::unordered_set<std::string> NonTSymbol, TSymbol;
std::unordered_map<std::string, int> SymboltoCode;
std::unordered_map<int, std::string> CodetoSymbol;
std::unordered_set<int> NonTSymbolCode, TSymbolCode;
std::vector<ProductionInCode> ProductionsInCode;
std::string StartSymbol = "Program";

// 读取 json 文件，获得所有产生式，并确定文法开始符 (读入的第一条产生式的左部非终极符)
static void getGrammer() {
    std::string Path = "D:/QT Project/grammer-analysis/grammer.json";
    QFile file(QString::fromStdString(Path));
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "file open error";
        return;
    }
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    QString str = stream.readAll();
    file.close();
    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8(), &jsonError);
    if (jsonError.error != QJsonParseError::NoError && !doc.isNull()) {
        qDebug() << "Json format error";
        return;
    }
    // QJsonObject 是大括号括起来的，其是键值对的数组，QJsonArray 是方括号括起来的，是值的数组
    QJsonObject rootObj = doc.object();
    for (auto it1 = rootObj.begin(); it1 != rootObj.end(); ++it1) {
        std::string keyValue = it1.key().toStdString();
        QJsonObject subObj = it1.value().toObject();
        for (auto it2 = subObj.begin(); it2 != subObj.end(); ++it2) {
            Productions.emplace_back(Production());
            Productions.back().setLeftPart(keyValue);
            if (it2.value().isNull()) {
                Productions.back().addRightPart("null");
                continue;
            }
            QJsonArray rootArry = it2.value().toArray();
            for (auto it3 = rootArry.begin(); it3 != rootArry.end(); ++it3)
                Productions.back().addRightPart(it3->toString().toStdString());
        }
    }
}

// 根据产生式推断出终极符、非终极符符 (只在右部出现过的是终极符，其他是非终极符)
static void getSymbol() {
    for (auto &production : Productions) {
        if (TSymbol.count(production.getLeftPart()) != 0)
            TSymbol.erase(production.getLeftPart());
        NonTSymbol.emplace(production.getLeftPart());
        for (auto &symbol : production.getRightPart()) {
            if (NonTSymbol.count(symbol) == 0)
                TSymbol.emplace(symbol);
        }
    }
}

// 对终极符和非终极符编码，并填充所有使用符号的对象
static void getCode() {
    int symbolCount = 0;
    for (auto &symbol : TSymbol) {
        SymboltoCode[symbol] = ++symbolCount;
        TSymbolCode.emplace(symbolCount);
    }
    for (auto &symbol : NonTSymbol) {
        SymboltoCode[symbol] = ++symbolCount;
        NonTSymbolCode.emplace(symbolCount);
    }
    for (auto &p : SymboltoCode)
        CodetoSymbol[p.second] = p.first;
    size_t n = Productions.size();
    ProductionsInCode.resize(n);
    for (size_t i = 0; i < n; ++i) {
        ProductionsInCode[i].setLeftPart(SymboltoCode[Productions[i].getLeftPart()]);
        for (auto &symbol : Productions[i].getRightPart())
            ProductionsInCode[i].addRightPart(SymboltoCode[symbol]);
    }
}

void preTask() {
    getGrammer();
    getSymbol();
    getCode();
    TSymbol.emplace("#");
    SymboltoCode["#"] = 0;
    TSymbolCode.emplace(0);
    CodetoSymbol[0] = "#";
}
