#ifndef LL1PARSER_H
#define LL1PARSER_H

#include <QString>

class Node;

extern QString LL1ErrorMessage;
extern int LL1ErrorLine;

Node* LL1Parser();

#endif // LL1PARSER_H
