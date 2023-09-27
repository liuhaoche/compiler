#ifndef PRETASK_H
#define PRETASK_H

#include "globals.h"

#include <unordered_map>
#include <unordered_set>

// 空用字符串 "null" 表示

// 接受一个字符串，标志着一个json文件，从中获取语法信息
void preTask();

// 产生式集合，左部非终极符做键，值是所有可能的产生式右部
extern std::vector<Production> Productions;
// 非终极符和终极符集合
extern std::unordered_set<std::string> NonTSymbol, TSymbol;
// 符号到编码的映射
extern std::unordered_map<std::string, int> SymboltoCode;
// 编码到符号的映射
extern std::unordered_map<int, std::string> CodetoSymbol;
// 非终极符和终极符编码的集合
extern std::unordered_set<int> NonTSymbolCode, TSymbolCode;
// 以编码形式的产生式的集合，结构与上述产生式集合相同
extern std::vector<ProductionInCode> ProductionsInCode;
// 文法的开始符
extern std::string StartSymbol;

#endif // PRETASK_H
