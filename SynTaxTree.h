#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H

#include <iostream>
#include <vector>

typedef enum { ProK = 1, PheadK, TypeK, VarK, ProcK, ProcDeck, StmLK, Deck, StmtK, ExpK } Node_kind;
typedef enum { ArrayK = 1, CharK, IntegerK, RecordK, Idk } Dec_kind;//声明类型
typedef enum { IfK = 1, WhileK, AssignK, ReadK, WriteK, CallK, ReturnK } Stmt_kind;//语句类型
typedef enum { OpK = 1, ConstK, Exp_IdK } Exp_kind; //表达式类型
typedef enum { valparamtype = 1, varparamtype } Para_kind;
typedef enum { LT = 1, EQ, PLUS, MINUS, TIMES, OVER } OP_kind;
typedef enum { IdV = 1, ArrayMembV, FieldMembV } Varkind_kind;//EXP中变量的类型
typedef enum { Void = 1, Integer, Boolean } ExpType;//表达式返回值类型

struct Node {
    Node* child[3];//指向子语法树的结点指针
    Node* sibling;//指向兄弟语法树的指针
    int lineno;//行号
    Node_kind nodekind;//节点类型
    struct {
        Dec_kind dec;
        Stmt_kind stmt;
        Exp_kind exp;
    } kind;
    std::vector<std::string> name;// 标识符名字
    std::vector<int> table; // 指针数组，数组成员是节点中的各个标识符在符号表的入口
    std::string type_name; //记录类型名 类型声明语句用
    union {
        struct {
            int low;
            int up;
            Dec_kind childType;//数组成员类型 (只是int char 还是有别的)??
        } ArrayAttr;
        struct {
            Para_kind paramt;
        } ProcAttr;
        struct {
            OP_kind op;
            int val;
            Varkind_kind varkind;
            ExpType type; //记录返回类型;
        } ExpAttr;
    } attr;
    Node();
    ~Node();
};

#endif // SYNTAXTREE_H
