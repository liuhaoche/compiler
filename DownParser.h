#ifndef DOWNPARSER_H
#define DOWNPARSER_H

#include <QString>

class Node;

extern QString ParserErrorMessage, ParserErrorLeft;
extern int ParserErrorLine;

Node* DownParser();

#endif // DOWNPARSER_H
