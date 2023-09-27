#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_set>

#include "lexical.h"
#include "PreTask.h"

#define ID SymboltoCode["ID"]
#define DIGIT SymboltoCode["INTC"]

std::vector<Token> tokens;
std::vector<int> lexical_error_line;
std::vector<QString> lexical_error_message;

static std::unordered_set<char> separator_symbol {'.', '/', ':', '=', ']', '[', ',', '*', '+', '-', '(', ')', '<', '=', ';', '{', '}'};

static inline bool Map_in(std::string &str) {           // 判断word是否在TSymbol中
    return TSymbol.count(str);
}

static inline bool is_separator(char c) {
    if (isspace(c))
        return true;
    if (c == '\0')
        return true;
    return separator_symbol.count(c);
}

Token::Token(int _rownumber, int _lex, std::string _sem) :
    rownumber(_rownumber),
    lex(_lex),
    sem(_sem) {}

int lexical(QString file_path) {
    lexical_error_line.clear();
    tokens.clear();
    std::string word;                                   // string变量识别单词
    std::string str;                                    // string变量进行字符识别
    std::ifstream infile(file_path.toStdString());      // 文件输入流
    std::ostringstream out;
    out << infile.rdbuf();
    str = out.str();                                    // 将得到的字符储存到string类型变量中
    int csize = str.length();
    int row_num = 1;
    infile.close();
    for (int i = 0; i < csize;) {                   // 对整个字符串进行遍历
        while (isspace(str[i])) {
            if (str[i] == '\n')
                ++row_num;
            ++i;
        }
        if (i >= csize)
            break;
        if (isalpha(str[i])) {                          // 对标识符和基本字进行识别,调用库函数isalpha()
            word.clear();
            while (isalpha(str[i]) || isdigit(str[i])) {
                word += str[i++];
            }
            if (is_separator(str[i])) {
                if (Map_in(word)) {                         // 判断是不是基本字
                    tokens.emplace_back(row_num, SymboltoCode[word], word);
                }
                else {                                      // 否则为标识符
                    tokens.emplace_back(row_num, ID, word);
                }
            }
            else {
                lexical_error_line.emplace_back(row_num);
                while(!is_separator(str[i]))
                    word += str[i++];
                lexical_error_message.emplace_back("Error ID " + QString::fromStdString(word));
            }
        }
        else if (isdigit(str[i])) {                     // 判断是不是常数，调用库函数isdigit()
            word.clear();
            while (isdigit(str[i])) {
                word += str[i++];
            }
            if (!is_separator(str[i])) {
                lexical_error_line.emplace_back(row_num);
                while(!is_separator(str[i]))
                    word += str[i++];
                lexical_error_message.emplace_back("Error DIGIT " + QString::fromStdString(word));
            }
            else {
                tokens.emplace_back(row_num, DIGIT, word);
            }
        }
        else if (str[i] == ':') {//对:=进行判断
            if (str[++i] == '=') {
                tokens.emplace_back(row_num, SymboltoCode[":="], ":=");
                ++i;
            }
            else {
                lexical_error_line.emplace_back(row_num);
                lexical_error_message.emplace_back("Error SYMBOL :" + QString(str[i - 1]));
            }
        }
        else if (str[i] == '.') {
            if (str[++i] == '.') {
                tokens.emplace_back(row_num, SymboltoCode[".."], "..");
                ++i;
            }
            else {
                tokens.emplace_back(row_num, SymboltoCode["."], ".");
            }
        }
        else if (str[i] == '{') {
            while (i < csize && str[i] != '}') {
                if (str[i] == '\n')
                    ++row_num;
                ++i;
            }
            if (i == csize) {
                lexical_error_line.emplace_back(row_num);
                lexical_error_message.emplace_back("Missing }");
            }
            else
                ++i;
        }
        else if (str[i] == '}') {
            lexical_error_line.emplace_back(row_num);
            lexical_error_message.emplace_back("Missing {");
            ++i;
        }
        else {//对其他的基本字依次进行判断
            word = str[i++];
            if (Map_in(word)) {
                tokens.emplace_back(row_num, SymboltoCode[word], word);
            }
            else {
                lexical_error_line.emplace_back(row_num);
                lexical_error_message.emplace_back(QString("Unknown symbol %1").arg(QString::fromStdString(word)));
            }
        }
    }
    return 0;
}
