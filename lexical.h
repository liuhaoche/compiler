#ifndef LEXICAL_H
#define LEXICAL_H

#include <string>
#include <vector>
#include <QString>

struct Token {
    int rownumber;//行号
    int lex;     //语法信息 若在map中找到则为保留字或分隔符即SymboltoCode[word] 否则为标识符1或常数2
    std::string sem;//语义信息 即为word本身
    Token() = default;
    Token(int _rownumber, int _lex, std::string _sem);
};

extern std::vector<Token> tokens;
extern std::vector<int> lexical_error_line;
extern std::vector<QString> lexical_error_message;

int lexical(QString file_path);

#endif // LEXICAL_H
