#ifndef YUYIANALYZE_H
#define YUYIANALYZE_H

#include <string>
#include <vector>
class Node;

#define initOff 7
typedef enum { typeKind, varKind, procKind } IDKind;
typedef enum { dir, indir } AccessKind;
typedef enum { intTy, charTy, arrayTy, recordTy, boolTy } Typekind;
struct AttributeIR;
struct typeIR;
struct symbtable;
struct ParamTable;
struct AttributeIR { //标识符的信息项
    struct typeIR* idtype = nullptr;//指向内部表示
    IDKind kind;
    union {
        struct {
            AccessKind access;
            int level;
            int off;
        } VarAttr;//变量标识符的属性
        struct {
            int level;
            ParamTable* param;
            int code;
            int size;
        } ProcAttr;
    }More;
};
struct ParamTable {
    symbtable* param = nullptr;
    ParamTable* next = nullptr;
};
struct fieldChain {
    std::string idname;
    typeIR* kind;//域中成员的类型 TypeKind
    int offset;
    fieldChain* next = nullptr;
};
struct typeIR {
    int size;
    Typekind kind;
    union {
        struct {
            int low;
            int up;
            struct typeIR* indexTy;//指向数组下标的内部表示
            struct typeIR* elemTy;//指向数组元素的内部表示
        }ArrayAttr;
        fieldChain* body;
    }More;
};
struct symbtable {
    std::string idname;
    AttributeIR attrIR;
    struct symbtable* next = nullptr;
};
//保存错误的信息
struct Error {
    int line = 0;
    int id = 0;
    std::string idname;
};
extern std::vector<Error> ErrorInformation;
struct scope {
    symbtable* next = nullptr;
};
extern scope Scope[256];
extern std::vector<std::pair<int, scope>> DeleteScope;
extern int Level;//全局变量表示当前层数
extern int off;
extern typeIR IntegerInnerExp, CharInnerExp, BoolInnerExp;

void Analyze(Node* currentP);
void DestroyTable();

#endif // YUYIANALYZE_H
