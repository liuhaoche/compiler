#include "ParserPreTask.h"
#include "PreTask.h"

#include <iostream>

std::unordered_map<int, std::unordered_set<int>> First, Follow;
std::vector<std::unordered_set<int>> Predict;
std::unordered_map<int, std::unordered_map<int, int>> Table;

static inline bool isTerminal(const int &code) {
    return TSymbolCode.count(code);
}

// 生成 nonTSymbol 的 First 集
// 使用递归，要求文法不能是左递归的
static void __getFirst(int nonTSymbol) {
    size_t pos = 0;
    while (ProductionsInCode[pos].getLeftPart() != nonTSymbol)
        ++pos;
    while (pos < ProductionsInCode.size() && ProductionsInCode[pos].getLeftPart() == nonTSymbol) {
        bool can_erase_null = (First[nonTSymbol].count(SymboltoCode["null"]) == 0);
        for (auto &code : ProductionsInCode[pos].getRightPart()) {
            if (can_erase_null)
                First[nonTSymbol].erase(SymboltoCode["null"]);
            if (isTerminal(code)) {
                First[nonTSymbol].emplace(code);
                break;
            }
            if (First[code].empty())
                __getFirst(code);
            for (auto &code1 : First[code])
                First[nonTSymbol].emplace(code1);
            if (First[code].count(SymboltoCode["null"]) == 0)
                break;
        }
        ++pos;
    }
}

// 生成所有非终极符的 First 集
static void getFirst() {
    for (auto &nonTSymbol : NonTSymbolCode) {
        if (First[nonTSymbol].empty())
            __getFirst(nonTSymbol);
    }
}

// 生成所有非终极符的 Follow 集
void getFollow() {
    Follow[SymboltoCode[StartSymbol]].emplace(SymboltoCode["#"]);
    std::unordered_map<int, std::unordered_set<int>> dependOn;
    for (auto &production : ProductionsInCode) {
        const auto &rightPart = production.getRightPart();
        std::unordered_set<int> symbols;
        symbols.emplace(SymboltoCode["null"]);
        for (auto it = rightPart.rbegin(); it != rightPart.rend(); ++it) {
            if (isTerminal(*it)) {
                symbols.clear();
                symbols.emplace(*it);
            }
            else {
                if (symbols.count(SymboltoCode["null"]) == 1)
                    dependOn[*it].emplace(production.getLeftPart());
                for (auto &symbol : symbols) {
                    if (symbol != SymboltoCode["null"])
                        Follow[*it].emplace(symbol);
                }
                if (First[*it].count(SymboltoCode["null"]) == 0) {
                    symbols.clear();
                }
                symbols.insert(First[*it].begin(), First[*it].end());
            }
        }
    }
    bool loopFlag = true;
    while (loopFlag) {
        loopFlag = false;
        for (auto &p : dependOn) {
            int nonTSymbol = p.first;
            size_t originSize = Follow[nonTSymbol].size();
            for (auto &symbol : p.second) {
                Follow[nonTSymbol].insert(Follow[symbol].begin(), Follow[symbol].end());
            }
            if (originSize != Follow[nonTSymbol].size())
                loopFlag = true;
        }
    }
}

// 生成所有产生式的 Predict 集
static void getPredict() {
    size_t n = ProductionsInCode.size();
    Predict.resize(n);
    for (size_t i = 0; i < n; ++i) {
        bool needFollow = true;
        for (auto &code : ProductionsInCode[i].getRightPart()) {
            if (isTerminal(code)) {
                if (code != SymboltoCode["null"]) {
                    Predict[i].emplace(code);
                    needFollow = false;
                }
                break;
            }
            else {
                for (auto code1 : First[code]) {
                    if (code1 != SymboltoCode["null"])
                        Predict[i].emplace(code1);
                }
                if (First[code].count(SymboltoCode["null"]) == 0) {
                    needFollow = false;
                    break;
                }
            }
        }
        if (needFollow) {
            Predict[i].insert(Follow[ProductionsInCode[i].getLeftPart()].begin(), Follow[ProductionsInCode[i].getLeftPart()].end());
        }
    }
}

static void getTable() {
    size_t n = Predict.size();
    for (size_t i = 0; i < n; ++i) {
        int leftPart = ProductionsInCode[i].getLeftPart();
        for (auto &code : Predict[i]) {
            Table[leftPart][code] = i;
        }
    }
}

void getAll() {
    getFirst();
    getFollow();
    getPredict();
    getTable();
}
