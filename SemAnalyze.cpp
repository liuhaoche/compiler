#define _CRT_SRCURE_NO_WARNINGS
#include<SynTaxTree.h>
#include "SemAnalyze.h"

//1） 标识符的重复定义；
//（2） 无声明的标识符；
//（3） 标识符为非期望的标识符类别（类型标识符，变量标识符，过程名标识符）；
//（4） 数组类型下标越界错误；
//（5） 数组成员变量和域变量的引用不合法；
//（6） 赋值语句的左右两边类型不相容；
//（7） 赋值语句左端不是变量标识符；
//（8） 过程调用中 ，形实参类型不匹配；
//（9） 过程调用中， 形实参个数不相同；
//（10）过程调用语句中，标识符不是过程标识符；
//（11）if 和while 语句的条件部分不是bool类型；
//（12）表达式中运算符的分量的类型不相容

std::vector<Error> ErrorInformation;
scope Scope[256];
std::vector<std::pair<int, scope>> DeleteScope;
typeIR IntegerInnerExp, CharInnerExp, BoolInnerExp;
//vector<scope> DeleteScope;//保存已被删除的符号表
int Level = -1;//全局变量表示当前层数
int off = initOff;
//1.创建一个符号表
void CreatTable() {
    Level++;
    off = initOff;
    return;
}
//2.撤销一个符号表
void DestroyTable() {
    std::pair<int, scope> p;
    p.first = Level;
    p.second = Scope[Level];
    DeleteScope.push_back(p);
    Scope[Level].next = nullptr;
    Level--;
    return;
}
//3.符号表中查找标识符
bool searchoneTable(std::string Id, symbtable** Entry, int currentLevel) {
    symbtable* point = Scope[currentLevel].next;
    while (point != nullptr) {
        if (point->idname == Id) {
            (*Entry) = point;
            return true;
        }
        point = point->next;
    }
    return false;
    //没找到Entry为空
}
//符号表中查找标识符 ？？？
bool FindEntry(std::string Id, bool flag, symbtable** Entry) {
    if (flag == false) {//表示向层数减一的方向寻找
        if (searchoneTable(Id, Entry, Level)) return true;
        return false;
    }
    else {
        for (int i = Level; i >= 0; i--) {
            if (searchoneTable(Id, Entry, i)) return true;
        }
        return false;
    }
}
//4.登记标识符和属性到符号表
bool Enter(std::string Id, AttributeIR* AttribP, symbtable** Entry, Node* t) {
    bool present = FindEntry(Id, false, Entry); //最外层查找标识符
    //cout << "present" << present << endl;
    if (present == true) {
        Error e;
        e.line = t->lineno;
        e.id = 1;
        e.idname = "标识符" + Id + "标识符重复声明";
        ErrorInformation.push_back(e);
    }
    else { //插入符号表
        symbtable* newsymbtable = new symbtable;
        newsymbtable->idname = Id;
        newsymbtable->attrIR = *AttribP;
        if (Scope[Level].next == nullptr)
            Scope[Level].next = newsymbtable;
        else {
            symbtable* st = Scope[Level].next;
            while (st->next != nullptr) {
                st = st->next;
            }
            st->next = newsymbtable;
        }
        (*Entry) = newsymbtable;
    }
    return present;

}
//5.在域表中查找域名
bool FindField(std::string Id, fieldChain* head, fieldChain** Entry) {
    fieldChain* r = head;
    while (r != nullptr) {
        if (Id == r->idname) {
            (*Entry) = r;
            return true;
        }
        r = r->next;
    }
    //没找到Entry为空
    return false;
}

bool flagtree;///////
void innitialize();//2. 初始化函数
typeIR* TypeProcess(Node* t);//3. 类型分析处理函数
typeIR* nameType(Node* t);//4. 自定义类型内部分析函数
typeIR* arrayType(Node* t);//5. 数组类型内部处理函数
typeIR* recordType(Node* t);//6. 处理记录类型的内部表示...语法怎么表示的
void TypeDecPart(Node* t);//7. 类型声明部分分析处理函数
void VarDecList(Node* t);//8. 变量声明部分分析处理函数
void ProcDecpart(Node* t);//9. 过程声明部分分析处理函数
symbtable* HeadProcess(Node* t);//10. 过程声明头分析函数
ParamTable* ParaDecList(Node* t);//11. 形参分析处理函数
void Body(Node* t);//12.执行体部分分析处理函数
void statement(Node* t);//13. 语句序列分析处理函数
typeIR* Expr(Node* t, AccessKind* Ekind);//14. 表达式分析处理函数
typeIR* arrayVar(Node* t);//15. 数组变量的处理分析函数
typeIR* recordVar(Node* t);//16. 记录变量中域变量的分析处理函数
void assignstatment(Node* t);//17. 赋值语句分析函数
void callstatment(Node* t);//18. 过程调用语句分析处理函数
void ifstatment(Node* t);//19. 条件语句的分析处理函数
void whilestatment(Node* t);//20. 循环语句分析处理函数
void readstatment(Node* t);//21. 读语句分析处理函数
void returnstatment(Node* t);//22. 返回语句分析处理函数
void writestatment(Node* t);//23. 写语句分析处理函数
//1. 语义分析主程序
void Analyze(Node* currentP) {
    //if(currentP == nullptr) return;
    CreatTable();
    innitialize();
    symbtable** Entry = new symbtable*;
    AttributeIR* atr = new AttributeIR;
    atr->kind = procKind;
    Enter(currentP->child[0]->name[0],atr , Entry, currentP->child[0]);
    Node* decNode = currentP->child[1];
    //t->child[1]->sibling->child[0]
    if (decNode != nullptr && decNode->nodekind == TypeK) {
        TypeDecPart(decNode->child[0]);
        //cout << "类型声明节点非空" << endl;
        decNode = decNode->sibling;
    }
    if (decNode != nullptr && decNode->nodekind == VarK) {
        if(currentP->child[1]->sibling->child[0] == decNode->child[0] && decNode->child[0] != nullptr)
        //cout << "变量声明节点非空" << endl;
        if(decNode->child[0] != nullptr)
        VarDecList(decNode->child[0]);
        decNode = decNode->sibling;
    }
    if (decNode != nullptr && decNode->nodekind == ProcK) {
        ProcDecpart(decNode->child[0]);
    }
    Body(currentP->child[2]);
    delete currentP;
    return;
}
//2. 初始化函数
void innitialize() {
    IntegerInnerExp.kind = intTy;
    IntegerInnerExp.size = 1;
    CharInnerExp.kind = charTy;
    CharInnerExp.size = 1;
    BoolInnerExp.kind = boolTy;
    BoolInnerExp.size = 1;
}
//3. 类型分析处理函数
typeIR* TypeProcess(Node* t) {
    typeIR* ptr = nullptr;
    if (t == nullptr) return nullptr;
    Dec_kind deckind = t->kind.dec;
    switch (deckind)
    {
    case Idk: {
        ptr = nameType(t);
        break;
    }
    case ArrayK: {
        ptr = arrayType(t);
        break;
    }
    case RecordK: {
        ptr = recordType(t);
        break;
    }
    case IntegerK: {
        ptr = &IntegerInnerExp;
        //cout << "类型为int" << endl;
        break;
    }
    case CharK: {
        ptr = &CharInnerExp;
        break;
    }
    default:
        break;
    }
    return ptr;
}
//4. 自定义类型内部分析函数
typeIR* nameType(Node* t) {
    symbtable** Entry = new symbtable*;
    int i = 0;
    //if (flagtree) i++;////////
    //会不会出现一个节点存多个的情况？？,如果有是什么情况
    bool present = FindEntry(t->type_name, false, Entry);//检查类型是否声明
    if (present == false) {
        Error e;
        e.id = 2;
        e.idname = "自定义类型" + t->type_name + "未声明";
        ErrorInformation.push_back(e);
    }
    else {
        if ((*Entry)->attrIR.kind != typeKind) {
            Error e;
            e.id = 3;
            e.idname = "类型不匹配：" + t->name[i] + "不是类型标识符";
            e.line = t->lineno;
            ErrorInformation.push_back(e);
        }
        else return (*Entry)->attrIR.idtype;
    }
    return nullptr;
}
//5. 数组类型内部处理函数
typeIR* arrayType(Node* t) {
    if (t->attr.ArrayAttr.low > t->attr.ArrayAttr.up) {
        Error e;
        e.id = 4;
        e.line = t->lineno;
        e.idname = "数组下标越界";
        ErrorInformation.push_back(e);
        return nullptr;
    }
    else {
        typeIR* p = new typeIR;
        p->kind = arrayTy;
        p->size = t->attr.ArrayAttr.up - t->attr.ArrayAttr.low + 1;
        p->More.ArrayAttr.indexTy = &IntegerInnerExp;
        if (t->attr.ArrayAttr.childType == IntegerK) {
            p->More.ArrayAttr.elemTy = &IntegerInnerExp;
        }
        else p->More.ArrayAttr.elemTy = &CharInnerExp;
        return p;
    }
}
//6. 处理记录类型的内部表示...语法怎么表示的
typeIR* recordType(Node* t) {
    typeIR* ptr = new typeIR;
    ptr->kind = recordTy;
    fieldChain* head = nullptr;//头结点
    fieldChain* tail = nullptr;//当前的结点
    Node* np = t->child[0];
    int offset = 0;
    while (np != nullptr) {
        for (size_t i = 0; i < np->name.size(); i++) {//处理多id的情况，判断是否重命名
            if (head != nullptr) {
                fieldChain** Entry = new fieldChain*;
                bool present = FindField(np->name[i], head, Entry);
                if (present == true) {
                    Error e;
                    e.id = 1;
                    e.line = t->lineno;//t 还是 np
                    e.idname = "记录中标识符重命名";
                    ErrorInformation.push_back(e);
                }
            }
            fieldChain* newF = new fieldChain;
            newF->kind = TypeProcess(np);
            newF->idname = np->name[i];
            newF->offset = offset + newF->kind->size;
            //newF->next = nullptr;
            if (head == nullptr) head = newF;
            else tail->next = newF;
            tail = newF;
        }
        np = np->sibling;
    }
    ptr->More.body = head;
    return ptr;
}
//7. 类型声明部分分析处理函数
void TypeDecPart(Node* t) {
    //初始化present为0 属性类型记为typeKind
    if (t == nullptr) return;
    symbtable** Entry = new symbtable*;
    size_t i = 0;
    //if (t->kind.dec == Idk) i++; // 跳过标识符
    for (; i < t->name.size(); i++) {
        bool flag = FindEntry(t->name[i], false, Entry);
        if (flag == true) {
            Error e;
            e.id = 1;
            e.line = t->lineno;
            e.idname = "类型标识符" + t->name[i] + "重复声明";
            ErrorInformation.push_back(e);
        }
        else {
            AttributeIR* present = new AttributeIR;
            present->kind = typeKind;
            flagtree = true;
            present->idtype = TypeProcess(t);
            flagtree = false;
            Enter(t->name[i], present, Entry, t);
        }
    }
    TypeDecPart(t->sibling);
    return;
}
//8. 变量声明部分分析处理函数
void VarDecList(Node* t) {
    if (t == nullptr)  return;
    symbtable** Entry = new symbtable*;
    size_t i = 0;
    //if (t->kind.dec = Idk) i++;
    for (; i < t->name.size(); i++) {
        //cout << t->name.size() << endl;
        bool flag = FindEntry(t->name[i], false, Entry);
        if (flag == true) {
            Error e;
            e.id = 1;
            e.idname = "变量标识符" + t->name[i] + "重复声明";
            e.line = t->lineno;
            ErrorInformation.push_back(e);
        }
        else {
            AttributeIR* present = new AttributeIR;
            present->kind = varKind;
            present->idtype= TypeProcess(t);
            present->More.VarAttr.access = dir;//判断是直接变量
            present->More.VarAttr.level = Level;
            if (present->idtype) {//
                present->More.VarAttr.off = off;
                off += present->idtype->size;//下一层off
            }
            Enter(t->name[i], present, Entry, t);
        }
    }
    VarDecList(t->sibling);
    return;
}
//9. 过程声明部分分析处理函数
void ProcDecpart(Node* t) {
    if (t == nullptr) return;
    while (t != nullptr) {
        symbtable* p = HeadProcess(t);//是t还是t->child[0]
        ParamTable* pa = ParaDecList(t->child[0]);//是不是child[0]
        p->attrIR.More.ProcAttr.param = pa;
        Node* decNode = t->child[1];
        if (decNode != nullptr && decNode->nodekind == TypeK) {
            TypeDecPart(decNode->child[0]);
            decNode = decNode->sibling;
        }
        if (decNode != nullptr && decNode->nodekind == VarK) {
            VarDecList(decNode->child[0]);
            decNode = decNode->sibling;
        }
        if (decNode != nullptr && decNode->nodekind == ProcK) {
            ProcDecpart(decNode->child[0]);
        }
        Body(t->child[2]);
        DestroyTable();
        //cout<<"destory table 1";
        t = t->sibling;
    }
}
//10. 过程声明头分析函数
symbtable* HeadProcess(Node* t) {
    if (t == nullptr) return nullptr;
    symbtable** Entry = new symbtable*;
    int i = 0;
    //if (t->kind.dec = Idk) i++;//
    bool flag = FindEntry(t->name[i], false, Entry);
    if (flag == true) {
        Error e;
        e.id = 1;
        e.line = t->lineno;
        e.idname = "过程表示符" + t->name[i] + "重复声明";
        ErrorInformation.push_back(e);
    }
    else {
        AttributeIR* present = new AttributeIR;
        present->kind = procKind;
        present->More.ProcAttr.level = Level;
        Enter(t->name[i], present, Entry, t);
        CreatTable();
        return (*Entry);
    }
    return nullptr;
}
//11. 形参分析处理函数
ParamTable* ParaDecList(Node* t) {
    if (t == nullptr) return nullptr;
    ParamTable* head = nullptr;
    ParamTable* tail = nullptr;
    ParamTable* curptr = new ParamTable;
    for (size_t i = 0; i < t->name.size(); i++) {
        symbtable** Entry = new symbtable*;
        bool flag;
        flag = FindEntry(t->name[i], false, Entry);
        if (flag == true) {
            Error e;
            e.id = 1;
            e.idname = "形参标识符" + t->name[i] + "重复声明";
            e.line = t->lineno;
            ErrorInformation.push_back(e);
        }
        else {
            AttributeIR* present = new AttributeIR;
            present->kind = varKind;
            present->idtype = TypeProcess(t);
            present->More.VarAttr.access = dir;//判断是否为dir
            present->More.VarAttr.level = Level;
            present->More.VarAttr.off = off;
            off += present->idtype->size;
            Enter(t->name[i], present, Entry, t);
            curptr->param = (*Entry);
            if (head == nullptr) head = curptr;
            else tail->next = curptr;
            tail = curptr;
        }
    }
    if (head == nullptr) head = ParaDecList(t->sibling);
    else
        tail->next = ParaDecList(t->sibling);//或curptr
    return head;
}
//12. 执行体部分分析处理函数
void Body(Node* t) {
    if (t == nullptr) return;
    Node* t1 = t->child[0];
    while (t1 != nullptr) {
        statement(t1);
        t1 = t1->sibling;
    }
}
//13. 语句序列处理函数
void statement(Node* t) {
    Stmt_kind sk = t->kind.stmt;
    switch (sk) {
        case IfK: {
            ifstatment(t);
            break;
        }
        case WhileK: {
            whilestatment(t);
            break;
        }
        case AssignK: {
            assignstatment(t);
            break;
        }
        case ReadK: {
            readstatment(t);
            break;
        }
        case WriteK: {
            writestatment(t);
            break;
        }
        case CallK: {
            callstatment(t);
            break;
        }
        case ReturnK: {
            returnstatment(t);
            break;
        }
    }
}
//14. 表达式分析处理函数
typeIR* Expr(Node* t, AccessKind* Ekind) {
    if (t == nullptr) return nullptr;
    if (t->kind.exp == Exp_IdK) {
        symbtable** Entry = new symbtable*;
        bool flag = FindEntry(t->name[0], true, Entry);//使用时为true 声明时为false

        if (flag == false) {
            Error e;
            e.id = 2;
            e.idname = "表达式中" + t->name[0] + "未声明";
            e.line = t->lineno;
            ErrorInformation.push_back(e);
            return nullptr;
        }
        else {
            if (t->child[0] != nullptr) {
                if ((*Entry)->attrIR.idtype->kind == arrayTy) {
                    Expr(t->child[0], Ekind);
                    if (Expr(t->child[0], Ekind) != nullptr) {
                        if (Expr(t->child[0], Ekind)->kind == arrayTy)
                            return Expr(t->child[0], Ekind);
                    }
                    return (*Entry)->attrIR.idtype->More.ArrayAttr.elemTy;
                }
                else if ((*Entry)->attrIR.idtype->kind == recordTy) {
                    //处理Eptr = recordVar(t)
                    fieldChain* f = (*Entry)->attrIR.idtype->More.body;
                    while (f != nullptr) {
                        if (f->idname == t->child[0]->name[0]) return f->kind;//返回记录中成员的内部表示
                        f = f->next;
                    }
                    Error e;
                    e.id = 2;
                    e.idname = "记录中未声明变量" + t->child[0]->name[0];
                    e.line = t->lineno;
                    ErrorInformation.push_back(e);
                    return nullptr;
                }
            }
            else return (*Entry)->attrIR.idtype;
        }
    }
    else if (t->kind.exp == ConstK) {
        (*Ekind) = dir;
        return &IntegerInnerExp;
    }
    else {
        AccessKind* Ekind = new AccessKind;
        typeIR* left = Expr(t->child[0], Ekind);
        typeIR* right = Expr(t->child[1], Ekind);
        if (left == right) {
            *Ekind = dir;
            return left;
        }
        else {
            Error e;
            e.id = 12;
            e.idname = "表达式中运算符的分量的类型不相容";
            e.line = t->lineno;
            ErrorInformation.push_back(e);
            return nullptr;
        }
    }
    return nullptr;
}
//15. 数组变量分析函数
typeIR* arrayVar(Node* t) {//传入的是数组节点
    symbtable** Entry = new symbtable*;

    bool flag = FindEntry(t->name[0], true, Entry);
    if (flag == false) {
        Error e;
        e.id = 2;
        e.idname = "数组变量" + t->name[0] + "未声明";
        e.line = t->lineno;
        ErrorInformation.push_back(e);
        return nullptr;
    }
    else {
        if ((*Entry)->attrIR.kind != varKind) {
            Error e;
            e.id = 7;
            e.idname = t->name[0] + "不是变量";
            e.line = t->lineno;
            ErrorInformation.push_back(e);
            return nullptr;
        }
        else {
            if ((*Entry)->attrIR.idtype->kind != arrayTy) {
                Error e;
                e.id = 3;
                e.idname = t->name[0] + "不是数组类型变量";
                ErrorInformation.push_back(e);
                return nullptr;
            }
            else {
                AccessKind* Ekind = new AccessKind;
                *Ekind = dir;
                typeIR* ir = Expr(t->child[0], Ekind);
                if (ir != &IntegerInnerExp) {
                    Error e;
                    e.id = 5;
                    e.idname = "数组引用非法";
                    e.line = t->lineno;
                    ErrorInformation.push_back(e);
                    return nullptr;
                }
                else return (*Entry)->attrIR.idtype->More.ArrayAttr.elemTy;
            }
        }
    }
    return nullptr;
}
//16. 记录变量中域变量分析处理函数
typeIR* recordVar(Node* t) {
    symbtable** Entry = new symbtable*;
    bool flag = FindEntry(t->name[0], true, Entry);
    if (flag == false) {
        Error e;
        e.id = 2;
        e.idname = "记录变量" + t->name[0] + "未声明";
        e.line = t->lineno;
        ErrorInformation.push_back(e);
        return nullptr;
    }
    if ((*Entry)->attrIR.kind != varKind) {
        Error e;
        e.id = 7;
        e.idname = t->name[0] + "不是变量";
        e.line = t->lineno;
        ErrorInformation.push_back(e);
        return nullptr;
    }
    if ((*Entry)->attrIR.idtype->kind != recordTy) {
        Error e;
        e.id = 3;
        e.idname = t->name[0] + "不是记录类型变量";
        ErrorInformation.push_back(e);
        return nullptr;
    }
    fieldChain* f = (*Entry)->attrIR.idtype->More.body;
    while (f != nullptr) {
        if (f->idname == t->child[0]->name[0]) {
            return f->kind;
        }
        f = f->next;
    }
    Error e;
    e.id = 2;
    e.idname = "记录中未声明变量" + t->child[0]->name[0];
    e.line = t->lineno;
    ErrorInformation.push_back(e);
    return nullptr;

}
//17. 赋值语句分析函数
void assignstatment(Node* t) {
    typeIR* l_IR = nullptr;//左侧的内部表示类型
    if (t->child[0]->kind.exp != Exp_IdK) {
        Error e;
        e.id = 7;
        e.idname = "赋值语句左端不是变量标识符";
        e.line = t->child[0]->lineno;
        ErrorInformation.push_back(e);
    }
    else {
        symbtable** Entry = new symbtable*;
        bool flag = FindEntry(t->child[0]->name[0], true, Entry);//true
        if (flag == false) {//没找到标识符
            Error e;
            e.id = 2;
            e.idname = "标识符" + t->child[0]->name[0] + "未声明";
            e.line = t->child[0]->lineno;
            ErrorInformation.push_back(e);
        }
        else {//找到后判断类型
            Node* left = t->child[0];//左侧表示符，判断是变量数组还是记录
            if (left->child[0] != nullptr) {
                if ((*Entry)->attrIR.idtype->kind == arrayTy) {
                    l_IR = arrayVar(left);
                    if (l_IR == nullptr) return;
                }
                else if ((*Entry)->attrIR.idtype->kind == recordTy) {
                    l_IR = recordVar(left);
                    if (l_IR == nullptr) return;
                }
            }
            else l_IR = (*Entry)->attrIR.idtype;
        }
    }
    AccessKind* Ekind = new AccessKind;
    typeIR* r_IR = Expr(t->child[1], Ekind);
    if (r_IR == nullptr) return;//错误在前面已经找到
    if (r_IR != l_IR) {
        Error e;
        e.id = 6;
        e.idname = "赋值语句左右类型不兼容";
        e.line = t->child[0]->lineno;
        ErrorInformation.push_back(e);
    }
    return;
}
//18. 过程调用语句分析处理函数
void callstatment(Node* t) {
    symbtable** Entry = new symbtable*;
    //cout<<"callname::"<<t->name[0]<<endl;
//    if(t->name[0] == "read"){
//        readstatment(t);
//        return;
//    }
//    if(t->name[0] == "write") {
//        writestatment(t);
//        return;
//    }
    bool flag = FindEntry(t->name[0], true, Entry);
    if (flag == false) {
        Error e;
        e.id = 2;
        e.idname = "记录变量" + t->name[0] + "未声明";
        e.line = t->lineno;
        //cout<<"111"<<endl;
        ErrorInformation.push_back(e);
        return;
    }
    if ((*Entry)->attrIR.kind != procKind) {
        Error e;
        e.id = 10;
        e.idname = "标识符" + t->name[0] + "不是过程名";
        e.line = t->lineno;
        ErrorInformation.push_back(e);
        return;
    }
    ParamTable* f_p = (*Entry)->attrIR.More.ProcAttr.param;
    Node* r_p = t->child[0];

    while (r_p != nullptr && f_p != nullptr) {
        AccessKind* Ekind = new AccessKind;
        typeIR* ir = Expr(r_p, Ekind);
        if (ir != f_p->param->attrIR.idtype) {
            Error e;
            e.id = 8;
            e.idname = "形参和实参类型不匹配";
            e.line = t->lineno;
            ErrorInformation.push_back(e);
            return;
        }
        r_p = r_p->sibling;
        f_p = f_p->next;
    }
    if (r_p != nullptr || f_p != nullptr) {
        Error e;
        e.id = 9;
        e.idname = "形参和实参个数不同";
        e.line = t->lineno;
        ErrorInformation.push_back(e);
        return;
    }
    return;
}
//19. if语句分析处理函数
void ifstatment(Node* t) {
    Node *condition = t->child[0];
    AccessKind *Ekind = new AccessKind;
    Expr(condition, Ekind);
    symbtable** Entry = new symbtable*;
    if (t->child[0]->name.size() != 0) {
        for (size_t i = 0; i < t->child[0]->name.size(); i++) {
            bool flag = FindEntry(t->child[0]->name[i], true, Entry);
            if (flag == false) {
                Error e;
                e.id = 2;
                e.idname = "变量" + t->child[0]->name[0] + "未声明";
                e.line = t->lineno;
                ErrorInformation.push_back(e);
            }
            else {
                if ((*Entry)->attrIR.idtype->kind != intTy && (*Entry)->attrIR.idtype->kind != boolTy) {
                    Error e;
                    e.id = 11;
                    e.idname = "if语句中条件部分不是bool类型";
                    e.line = t->lineno;
                    ErrorInformation.push_back(e);
                }
            }
        }
    }
    else {
        if (t->child[0]->nodekind != ExpK) {
            Error e;
            e.id = 11;
            e.idname = "if语句中条件部分不是bool类型";
            e.line = t->lineno;
            ErrorInformation.push_back(e);
        }
    }
    Node* elsesentence = t->child[1]; //else语句序列存在child[1]里面
    while (elsesentence != nullptr) {
        statement(elsesentence);
        elsesentence = elsesentence->sibling;
    }
    Node* thensentence = t->child[2];//then语句存在child[2]里面
    while (thensentence != nullptr) {
        statement(thensentence);
        thensentence = thensentence->sibling;
    }
    return;
}
//20. 循环语句分析处理函数
void whilestatment(Node* t) {
    symbtable** Entry = new symbtable*;
    Node *condition = t->child[0];
    AccessKind *Ekind = new AccessKind;
    Expr(condition, Ekind);
    if (t->child[0]->name.size() != 0) {
        for (size_t i = 0; i < t->child[0]->name.size(); i++) {
            bool flag = FindEntry(t->child[0]->name[i], true, Entry);
            if (flag == false) {
                Error e;
                e.id = 2;
                e.idname = "变量" + t->child[0]->name[0] + "未声明";
                e.line = t->lineno;
                ErrorInformation.push_back(e);
            }
            else {
                if ((*Entry)->attrIR.idtype->kind != intTy && (*Entry)->attrIR.idtype->kind != boolTy) {
                    Error e;
                    e.id = 11;
                    e.idname = "while语句中条件部分不是bool类型";
                    e.line = t->lineno;
                    ErrorInformation.push_back(e);
                }
            }
        }
    }
    else {
        if (t->child[0]->nodekind != ExpK) {
            Error e;
            e.id = 11;
            e.idname = "while语句中条件部分不是bool类型";
            e.line = t->lineno;
            ErrorInformation.push_back(e);
        }
    }
    Node* whilesentence = t->child[1];
    while (whilesentence != nullptr) {
        statement(whilesentence);
        whilesentence = whilesentence->sibling;
    }
    return;
}
//21. 读语句分析处理函数
void readstatment(Node* t) {
    symbtable** Entry = new symbtable*;
    //cout << "read::"<<t->name[0]<<endl;;
    //Node* t1 = t->child[0];
    bool flag = FindEntry(t->name[0], true, Entry);
    if (flag == false) {
        Error e;
        e.id = 2;
        e.idname = "未找到变量" + t->name[0];
        e.line = t->lineno;
        ErrorInformation.push_back(e);
    }
    else {
        if ((*Entry)->attrIR.kind != varKind && t->kind.exp != Exp_IdK) {
            Error e;
            e.id = 3;
            e.idname = "变量" + t->name[0] + "不是变量标识符";
            e.line = t->lineno;
            ErrorInformation.push_back(e);
        }
    }
    return;
}
//22. return语句分析处理函数
void returnstatment(Node* t) {

}
//23. 写语句分析处理函数
void writestatment(Node* t) {
    AccessKind* Ekind = new AccessKind;
    Expr(t->child[0], Ekind);
//    bool flag = FindEntry(t->child[0]->name[0], true, Entry);
//    if (flag == false) {
//        Error e;
//        e.id = 2;
//        e.idname = "未找到变量" + t->child[0]->name[0];
//        e.line = t->lineno;
//        ErrorInformation.push_back(e);
//    }
//    else {
//        if ((*Entry)->attrIR.kind != varKind) {
//            Error e;
//            e.id = 3;
//            e.idname = "变量" + t->child[0]->name[0] + "不是变量标识符";
//            e.line = t->lineno;
//            ErrorInformation.push_back(e);
//        }
//    }
    return;
}
