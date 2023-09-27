#include "DownParser.h"
#include "ParserPreTask.h"
#include "SynTaxTree.h"
#include "lexical.h"
#include "PreTask.h"

#include <QDebug>

#define ID SymboltoCode["ID"]
#define DIGIT SymboltoCode["INTC"]

static int pos, total;

QString ParserErrorMessage, ParserErrorLeft;
int ParserErrorLine;

static bool CheckPredict(int productnum) {
    --productnum;
    for (auto code : Predict[productnum]) {
        if (tokens[pos].lex == code)
            return true;
    }
    return false;
}

static void DownErrorFinish(const QString &leftpart) {
    ParserErrorMessage = "Program unexpected finish";
    ParserErrorLine = tokens[pos - 1].rownumber;
    ParserErrorLeft = leftpart;
}

static void DownErrorPredict(const std::vector<int> &productnum, const QString &leftpart) {
    ParserErrorMessage = "Suppose to get";
    for (auto num : productnum) {
        for (auto code : Predict[num]) {
            ParserErrorMessage += " '";
            ParserErrorMessage += QString::fromStdString(CodetoSymbol[code]);
            ParserErrorMessage += "'";
        }
    }
    ParserErrorMessage += QString(", but '%1' instead").arg(QString::fromStdString(tokens[pos].sem));
    ParserErrorLine = tokens[pos].rownumber;
    ParserErrorLeft = leftpart;
}

static void DownErrorSymbol(const QString &symbol, const QString &leftpart) {
    ParserErrorMessage = QString("Suppose to get '%1', but '%2' instead").arg(symbol).arg(QString::fromStdString(tokens[pos].sem));
    ParserErrorLine = tokens[pos].rownumber;
    ParserErrorLeft = leftpart;
}

static Node* down_program62();
static Node* down_program64();
static int down_program88(Node *node);
static int down_program11_12(Node *node);
static int down_program38(Node *node);
static int down_program81(Node *node);
static int down_program7(Node *node);
static int down_program33_34(Node *node);
static int down_program32(Node *node);
static int down_program26_27(Node *node);
static Node* down_program24_25();
static int down_program66(Node *node);
static int down_program78_79(Node *node);
static int down_program89_90_91(Node *node);
static int down_program85_86(Node *node);
static Node* down_program84();
static Node* down_program87();
static int down_program82_83(Node *node);
static int down_program99_100(Node *node);
static int down_program98(Node *node);
static int down_program95_96(Node *node);
static Node* down_program94();
static int down_program97(Node *node);
static int down_program92_93(Node *node);
static int down_program61(Node *node);
static int down_program22_23(Node *node);
static int down_program31(Node *node);
static int down_program47_48(Node *node);
static int down_program52_53(Node *node);
static Node* down_program49();
static int down_program50_51(Node *node);
static int down_program59(Node *node);
static int down_program54(Node *node);
static int down_program57_58(Node *node);
static Node* down_program60();
static int down_program55_56(Node *node);
static Node* down_program17();
static Node* down_program67();
static Node* down_program16();
static Node* down_program37();
static Node* down_program35();
static Node* down_program104();
static Node* down_program19_20_21();
static Node* down_program41_42(bool *flag);
static Node* down_program80();
static Node* down_program44_45(bool *flag);
static Node* down_program18();
static Node* down_program46();
static Node* down_program68();
static Node* down_program29_30(const QString &str);
static Node* down_program28();
static Node* down_program101_102_103(const QString &str);
static Node* down_program10(const QString &str);
static int down_program3_4(Node *node);
static int down_program1_2(Node *node);
static Node* down_program13(const QString &str);
static Node* down_program8_9(const QString &str);
static Node* down_program69_70_71_72_73_74(); // ?
static int down_program76_77(Node *node);
static Node* down_program75();
static Node* down_program63();
static int down_program65(Node *node);

// StmMore->null StmMore->; StmList
int down_program76_77(Node *node) {
    if (pos == total) {
        DownErrorFinish("StmMore");
        return -1;
    }
    if (CheckPredict(76)) {
        return 0;
    }
    else if (CheckPredict(77)) {
        ++pos;
        Node *node1 = down_program75();
        if (node1 == nullptr)
            return -1;
        node->sibling = node1;
        return 0;
    }
    DownErrorPredict({76, 77}, "StmMore");
    return -1;
}

// OtherTerm->null OtherTerm->AddOp Exp
Node* down_program44_45(bool *flag) {
    if (pos == total) {
        DownErrorFinish("OtherTerm");
        *flag = false;
        return nullptr;
    }
    if (CheckPredict(44)) {
        *flag = true;
        return nullptr;
    }
    else if (CheckPredict(45)) {
        Node *node = new Node;
        node->lineno = tokens[pos].rownumber;
        node->nodekind = ExpK;
        node->kind.exp = OpK;
        if (tokens[pos].sem == "+")
            node->attr.ExpAttr.op = PLUS;
        else
            node->attr.ExpAttr.op = MINUS;
        ++pos;
        Node *node1 = down_program18();
        if (node1 == nullptr) {
            delete node;
            *flag = false;
            return nullptr;
        }
        node->child[1] = node1;
        *flag = true;
        return node;
    }
    DownErrorPredict({44, 45}, "OtherTerm");
    *flag = false;
    return nullptr;
}

// OtherFactor->null OtherFactor->MultOp Term
Node* down_program41_42(bool *flag) {
    if (pos == total) {
        DownErrorFinish("OtherFactor");
        *flag = false;
        return nullptr;
    }
    if (CheckPredict(41)) {
        *flag = true;
        return nullptr;
    }
    else if (CheckPredict(42)) {
        Node *node = new Node;
        node->lineno = tokens[pos].rownumber;
        node->nodekind = ExpK;
        node->kind.exp = OpK;
        if (tokens[pos].sem == "*")
            node->attr.ExpAttr.op = TIMES;
        else
            node->attr.ExpAttr.op = OVER;
        ++pos;
        Node *node1 = down_program80();
        if (node1 == nullptr) {
            delete node;
            *flag = false;
            return nullptr;
        }
        node->child[1] = node1;
        *flag = true;
        return node;
    }
    DownErrorPredict({41, 42}, "OtherFactor");
    *flag = false;
    return nullptr;
}

// Variable->ID VariMore
Node* down_program104() {
    if (pos == total) {
        DownErrorFinish("Variable");
        return nullptr;
    }
    QString str = QString::fromStdString(tokens[pos++].sem);
    return down_program101_102_103(str);
}

// Factor->( Exp ) Factor->INTC Factor->Variable
Node* down_program19_20_21() {
    if (pos == total) {
        DownErrorFinish("Factor");
        return nullptr;
    }
    if (CheckPredict(19)) {
        ++pos;
        Node *node = down_program18();
        if (node == nullptr)
            return nullptr;
        if (pos == total) {
            DownErrorFinish("Factor");
            delete node;
            return nullptr;
        }
        if (tokens[pos].sem != ")") {
            DownErrorSymbol(")", "Factor");
            delete node;
            return nullptr;
        }
        ++pos;
        return node;
    }
    else if (CheckPredict(20)) {
        Node *node = new Node;
        node->lineno = tokens[pos].rownumber;
        node->nodekind = ExpK;
        node->kind.exp = ConstK;
        node->attr.ExpAttr.val = stoi(tokens[pos++].sem);
        return node;
    }
    else if (CheckPredict(21)) {
        return down_program104();
    }
    DownErrorPredict({19, 20, 21}, "Factor");
    return nullptr;
}

// Term->Factor OtherFactor
Node* down_program80() {
    Node *node1 = down_program19_20_21();
    if (node1 == nullptr)
        return nullptr;
    bool flag = false;
    Node *node2 = down_program41_42(&flag);
    if (!flag) {
        delete node1;
        return nullptr;
    }
    if (node2 != nullptr) {
        node2->child[0] = node1;
        return node2;
    }
    return node1;
}

// Exp->Term OtherTerm
Node* down_program18() {
    Node *node1 = down_program80();
    if (node1 == nullptr)
        return nullptr;
    bool flag = false;
    Node *node2 = down_program44_45(&flag);
    if (!flag) {
        delete node1;
        return nullptr;
    }
    if (node2 != nullptr) {
        node2->child[0] = node1;
        return node2;
    }
    return node1;
}

// ActParamMore->null ActParamMore->, ActParamList
int down_program3_4(Node *node) {
    if (pos == total) {
        DownErrorFinish("ActParamMore");
        return -1;
    }
    if (CheckPredict(3)) {
        return 0;
    }
    else if (CheckPredict(4)) {
        ++pos;
        return down_program1_2(node);
    }
    DownErrorPredict({3, 4}, "ActParamMore");
    return -1;
}

// ActParamList->null ActParamList->Exp ActParamMore
int down_program1_2(Node *node) {
    if (pos == total) {
        DownErrorFinish("ActParamList");
        return -1;
    }
    if (CheckPredict(1)) {
        return 0;
    }
    else if (CheckPredict(2)) {
        Node *node1 = down_program18();
        if (node1 == nullptr)
            return -1;
        if (node->child[0] == nullptr)
            node->child[0] = node1;
        else {
            Node *node2 = node->child[0];
            while (node2->sibling != nullptr)
                node2 = node2->sibling;
            node2->sibling = node1;
        }
        return down_program3_4(node);
    }
    DownErrorPredict({1, 2}, "ActParamList");
    return -1;
}

// CallStmRest->( ActParamList )
Node* down_program13(const QString &str) {
    if (pos == total) {
        DownErrorFinish("CallStmRest");
        return nullptr;
    }
    if (!CheckPredict(13)) {
        DownErrorPredict({13}, "CallStmRest");
        return nullptr;
    }
    ++pos;
    Node *node = new Node;
    node->lineno = tokens[pos - 1].rownumber;
    node->nodekind = StmtK;
    node->kind.stmt = CallK;
    node->name.emplace_back(str.toStdString());
    int ans = down_program1_2(node);
    if (ans != 0) {
        delete node;
        return nullptr;
    }
    if (pos == total) {
        DownErrorFinish("CallStmRest");
        delete node;
        return nullptr;
    }
    if (tokens[pos].sem != ")") {
        DownErrorSymbol(")", "CallStmRest");
        delete node;
        return nullptr;
    }
    ++pos;
    return node;
}

// RelExp->Exp OtherRelE OtherRelE->CmpOp Exp
Node* down_program67() {
    Node *node1 = down_program18();
    if (node1 == nullptr)
        return nullptr;
    if (pos == total) {
        DownErrorFinish("OtherRelE");
        delete node1;
        return nullptr;
    }
    if (!CheckPredict(43)) {
        DownErrorPredict({43}, "RelExp");
        delete node1;
        return nullptr;
    }
    Node *node = new Node;
    node->nodekind = ExpK;
    node->child[0] = node1;
    node->lineno = tokens[pos].rownumber;
    node->kind.exp = OpK;
    if (tokens[pos].sem == "<") {
        node->attr.ExpAttr.op = LT;
    }
    else {
        node->attr.ExpAttr.op = EQ;
    }
    ++pos;
    node1 = down_program18();
    if (node1 == nullptr) {
        delete node;
        return nullptr;
    }
    node->child[1] = node1;
    return node;
}

// FieldVarMore->null FieldVarMore->[ Exp ]
Node* down_program29_30(const QString &str) {
    if (pos == total) {
        DownErrorFinish("FieldVarMore");
        return nullptr;
    }
    if (CheckPredict(29)) {
        Node *node = new Node;
        node->lineno = tokens[pos - 1].rownumber;
        node->nodekind = ExpK;
        node->kind.exp = Exp_IdK;
        node->attr.ExpAttr.varkind = IdV;
        node->name.emplace_back(str.toStdString());
        return node;
    }
    else if (CheckPredict(30)) {
        ++pos;
        Node *node1 = down_program18();
        if (node1 == nullptr)
            return nullptr;
        Node *node = new Node;
        node->child[0] = node1;
        node->lineno = tokens[pos - 1].rownumber;
        node->nodekind = ExpK;
        node->name.emplace_back(str.toStdString());
        node->kind.exp = Exp_IdK;
        node->attr.ExpAttr.varkind = ArrayMembV;
        if (pos == total) {
            DownErrorFinish("VariMore");
            delete node;
            return nullptr;
        }
        if (tokens[pos].sem != "]") {
            DownErrorSymbol("]", "VariMore");
            delete node;
            return nullptr;
        }
        ++pos;
        return node;
    }
    DownErrorPredict({29, 30}, "FieldVarMore");
    return nullptr;
}

// FieldVar->ID FieldVarMore
Node* down_program28() {
    if (pos == total) {
        DownErrorFinish("FieldVar");
        return nullptr;
    }
    if (!CheckPredict(28)) {
        DownErrorPredict({28}, "FieldVar");
        return nullptr;
    }
    QString str = QString::fromStdString(tokens[pos++].sem);
    return down_program29_30(str);
}

// VariMore->null VariMore->[ Exp ] VariMore->. FieldVar
Node* down_program101_102_103(const QString &str) {
    if (pos == total) {
        DownErrorFinish("VariMore");
        return nullptr;
    }
    if (CheckPredict(101)) {
        Node *node = new Node;
        node->lineno = tokens[pos - 1].rownumber;
        node->nodekind = ExpK;
        node->kind.exp = Exp_IdK;
        node->attr.ExpAttr.varkind = IdV;
        node->name.emplace_back(str.toStdString());
        return node;
    }
    else if (CheckPredict(102)) {
        ++pos;
        Node *node1 = down_program18();
        if (node1 == nullptr)
            return nullptr;
        Node *node = new Node;
        node->child[0] = node1;
        node->lineno = tokens[pos - 1].rownumber;
        node->nodekind = ExpK;
        node->name.emplace_back(str.toStdString());
        node->kind.exp = Exp_IdK;
        node->attr.ExpAttr.varkind = ArrayMembV;
        if (pos == total) {
            DownErrorFinish("VariMore");
            delete node;
            return nullptr;
        }
        if (tokens[pos].sem != "]") {
            DownErrorSymbol("]", "VariMore");
            delete node;
            return nullptr;
        }
        ++pos;
        return node;
    }
    else if (CheckPredict(103)) {
        ++pos;
        Node *node1 = down_program28();
        if (node1 == nullptr)
            return nullptr;
        Node *node = new Node;
        node->child[0] = node1;
        node->lineno = tokens[pos - 1].rownumber;
        node->nodekind = ExpK;
        node->name.emplace_back(str.toStdString());
        node->kind.exp = Exp_IdK;
        node->attr.ExpAttr.varkind = FieldMembV;
        return node;
    }
    DownErrorPredict({101, 102, 103}, "VariMore");
    return nullptr;
}

// AssignmentRest->VariMore := Exp
Node* down_program10(const QString &str) {
    Node *node1 = down_program101_102_103(str);
    if (node1 == nullptr) {
        return nullptr;
    }
    Node *node = new Node;
    node->nodekind = StmtK;
    node->kind.stmt = AssignK;
    node->child[0] = node1;
    if (pos == total) {
        DownErrorFinish("AssignmentRest");
        delete node;
        return nullptr;
    }
    if (tokens[pos].sem != ":=") {
        DownErrorSymbol(":=", "AssignmentRest");
        delete node;
        return nullptr;
    }
    ++pos;
    node1 = down_program18();
    if (node1 == nullptr) {
        delete node;
        return nullptr;
    }
    node->child[1] = node1;
    return node;
}

// AssCall->AssignmentRest AssCall->CallStmRest
Node* down_program8_9(const QString &str) {
    if (pos == total) {
        DownErrorFinish("AssCall");
        return nullptr;
    }
    if (CheckPredict(8)) {
        return down_program10(str);
    }
    else if (CheckPredict(9)) {
        return down_program13(str);
    }
    DownErrorPredict({8, 9}, "AssCall");
    return nullptr;
}

// ReturnStm->RETURN
Node* down_program68() {
    if (pos == total) {
        DownErrorFinish("ReturnStm");
        return nullptr;
    }
    if (!CheckPredict(68)) {
        DownErrorPredict({68}, "ReturnStm");
        return nullptr;
    }
    Node *node = new Node;
    node->nodekind = StmtK;
    node->kind.stmt = ReturnK;
    node->lineno = tokens[pos++].rownumber;
    return node;
}

// OutputStm->WRITE ( Exp )
Node* down_program46() {
    if (pos == total) {
        DownErrorFinish("OutputStm");
        return nullptr;
    }
    if (!CheckPredict(46)) {
        DownErrorPredict({46}, "OutputStm");
        return nullptr;
    }
    ++pos;
    if (pos == total) {
        DownErrorFinish("OutputStm");
        return nullptr;
    }
    if (tokens[pos].sem != "(") {
        DownErrorSymbol("(", "OutputStm");
        return nullptr;
    }
    ++pos;
    Node *node1 = down_program18();
    if (node1 == nullptr)
        return nullptr;
    Node *node = new Node;
    node->nodekind = StmtK;
    node->kind.stmt = WriteK;
    node->child[0] = node1;
    if (pos == total) {
        DownErrorFinish("OutputStm");
        delete node;
        return nullptr;
    }
    if (tokens[pos].sem != ")") {
        DownErrorSymbol(")", "OutputStm");
        delete node;
        return nullptr;
    }
    ++pos;
    return node;
}

// InputStm->READ ( Invar )
Node* down_program35() {
    if (pos == total) {
        DownErrorFinish("InputStm");
        return nullptr;
    }
    if (!CheckPredict(35)) {
        DownErrorPredict({35}, "InputStm");
        return nullptr;
    }
    ++pos;
    if (pos == total) {
        DownErrorFinish("InputStm");
        return nullptr;
    }
    if (tokens[pos].sem != "(") {
        DownErrorSymbol("(", "InputStm");
        return nullptr;
    }
    ++pos;
    Node *node = new Node;
    node->nodekind = StmtK;
    node->kind.stmt = ReadK;
    if (pos == total) {
        DownErrorFinish("InputStm");
        delete node;
        return nullptr;
    }
    if (tokens[pos].lex != ID) {
        DownErrorSymbol("ID", "InputStm");
        delete node;
        return nullptr;
    }
    node->lineno = tokens[pos].rownumber;
    node->name.emplace_back(tokens[pos++].sem);
    if (pos == total) {
        DownErrorFinish("InputStm");
        delete node;
        return nullptr;
    }
    if (tokens[pos].sem != ")") {
        DownErrorSymbol(")", "InputStm");
        delete node;
        return nullptr;
    }
    ++pos;
    return node;
}

// LoopStm->WHILE RelExp DO StmList ENDWH
Node* down_program37() {
    if (pos == total) {
        DownErrorFinish("LoopStm");
        return nullptr;
    }
    if (!CheckPredict(37)) {
        DownErrorPredict({37}, "LoopStm");
        return nullptr;
    }
    ++pos;
    Node *node = new Node;
    node->nodekind = StmtK;
    node->kind.stmt = WhileK;
    Node *node1 = down_program67();
    if (node1 == nullptr) {
        delete node;
        return nullptr;
    }
    node->child[0] = node1;
    if (pos == total) {
        DownErrorFinish("LoopStm");
        delete node;
        return nullptr;
    }
    if (tokens[pos].sem != "DO") {
        DownErrorSymbol("DO", "LoopStm");
        delete node;
        return nullptr;
    }
    ++pos;
    node1 = down_program75();
    if (node1 == nullptr) {
        delete node;
        return nullptr;
    }
    node->child[1] = node1;
    if (pos == total) {
        DownErrorFinish("LoopStm");
        delete node;
        return nullptr;
    }
    if (tokens[pos].sem != "ENDWH") {
        DownErrorSymbol("ENDWH", "LoopStm");
        delete node;
        return nullptr;
    }
    ++pos;
    return node;
}

// ConditionalStm->IF RelExp THEN StmList ELSE StmList FI
Node* down_program16() {
    if (pos == total) {
        DownErrorFinish("ConditionalStm");
        return nullptr;
    }
    if (!CheckPredict(16)) {
        DownErrorPredict({16}, "ConditionalStm");
        return nullptr;
    }
    ++pos;
    Node *node = new Node;
    node->nodekind = StmtK;
    node->kind.stmt = IfK;
    Node *node1 = down_program67();
    if (node1 == nullptr) {
        delete node;
        return nullptr;
    }
    node->child[0] = node1;
    if (pos == total) {
        DownErrorFinish("ConditionalStm");
        delete node;
        return nullptr;
    }
    if (tokens[pos].sem != "THEN") {
        DownErrorSymbol("THEN", "ConditionalStm");
        delete node;
        return nullptr;
    }
    ++pos;
    node1 = down_program75();
    if (node1 == nullptr) {
        delete node;
        return nullptr;
    }
    node->child[1] = node1;
    if (pos == total) {
        DownErrorFinish("ConditionalStm");
        delete node;
        return nullptr;
    }
    if (tokens[pos].sem != "ELSE") {
        DownErrorSymbol("ELSE", "ConditionalStm");
        delete node;
        return nullptr;
    }
    ++pos;
    node1 = down_program75();
    if (node1 == nullptr) {
        delete node;
        return nullptr;
    }
    node->child[2] = node1;
    if (pos == total) {
        DownErrorFinish("ConditionalStm");
        delete node;
        return nullptr;
    }
    if (tokens[pos].sem != "FI") {
        DownErrorSymbol("FI", "ConditionalStm");
        delete node;
        return nullptr;
    }
    ++pos;
    return node;
}

// Stm->ConditionalStm Stm->LoopStm Stm->InputStm Stm->OutputStm Stm->ReturnStm Stm->ID AssCall
Node* down_program69_70_71_72_73_74() {
    if (pos == total) {
        DownErrorFinish("Stm");
        return nullptr;
    }
    if (CheckPredict(69)) {
        return down_program16();
    }
    else if (CheckPredict(70)) {
        return down_program37();
    }
    else if (CheckPredict(71)) {
        return down_program35();
    }
    else if (CheckPredict(72)) {
        return down_program46();
    }
    else if (CheckPredict(73)) {
        return down_program68();
    }
    else if (CheckPredict(74)) {
        QString id = QString::fromStdString(tokens[pos++].sem);
        return down_program8_9(id);
    }
    DownErrorPredict({69, 70, 71, 72, 73, 74}, "Stm");
    return nullptr;
}

// StmList->Stm StmMore
Node* down_program75() {
    Node *node = down_program69_70_71_72_73_74();
    if (node == nullptr) {
        return nullptr;
    }
    int ans = down_program76_77(node);
    if (ans == -1) {
        delete node;
        return nullptr;
    }
    return node;
}

// ProgramBody->BEGIN StmList END
Node* down_program63() {
    if (pos == total) {
        DownErrorFinish("ProgramBody");
        return nullptr;
    }
    if (!CheckPredict(63)) {
        DownErrorPredict({63}, "ProgramBody");
        return nullptr;
    }
    ++pos;
    Node *node = new Node;
    node->nodekind = StmLK;
    Node *node1 = down_program75();
    if (node1 == nullptr) {
        delete node;
        return nullptr;
    }
    node->child[0] = node1;
    if (pos == total) {
        DownErrorFinish("ProgramBody");
        delete node;
        return nullptr;
    }
    if (tokens[pos].sem != "END") {
        DownErrorSymbol("END", "ProgramBody");
        delete node;
        return nullptr;
    }
    ++pos;
    return node;
}

// ProcDecMore->null ProcDecMore->ProcDeclaration
int down_program57_58(Node *node) {
    if (pos == total) {
        DownErrorFinish("ProcDecMore");
        return -1;
    }
    if (CheckPredict(57)) {
        return 0;
    }
    else if (CheckPredict(58)) {
        Node *node1 = down_program60();
        if (node1 == nullptr)
            return -1;
        node->sibling = node1;
        return 0;
    }
    DownErrorPredict({57, 58}, "ProcDecMore");
    return -1;
}

// ProcBody->ProgramBody
int down_program54(Node *node) {
    Node *node1 = down_program63();
    if (node1 == nullptr)
        return -1;
    node->child[2] = node1;
    return 0;
}

// ProcDecPart->DeclarePart
int down_program59(Node *node) {
    Node *node1 = down_program17();
    if (node1 == nullptr)
        return -1;
    node->child[1] = node1;
    return 0;
}

// ParamMore->null ParamMore->; ParamDecList
int down_program52_53(Node *node) {
    if (pos == total) {
        DownErrorFinish("ParamMore");
        return -1;
    }
    if (CheckPredict(52)) {
        return 0;
    }
    else if (CheckPredict(53)) {
        ++pos;
        Node *node1 = down_program49();
        if (node1 == nullptr)
            return -1;
        node->sibling = node1;
        return 0;
    }
    DownErrorPredict({52, 53}, "ParamMore");
    return -1;
}

// FidMore->null FidMore->, FormList
int down_program22_23(Node *node) {
    if (pos == total) {
        DownErrorFinish("FidMore");
        return -1;
    }
    if (CheckPredict(22)) {
        return 0;
    }
    else if (CheckPredict(23)) {
        ++pos;
        return down_program31(node);
    }
    DownErrorPredict({22, 23}, "FidMore");
    return -1;
}

// FormList->ID FidMore
int down_program31(Node *node) {
    if (pos == total) {
        DownErrorFinish("FormList");
        return -1;
    }
    if (!CheckPredict(31)) {
        DownErrorPredict({31}, "FormList");
        return -1;
    }
    node->name.emplace_back(tokens[pos].sem);
    node->lineno = tokens[pos++].rownumber;
    return down_program22_23(node);
}

// Param->TypeName FormList Param->VAR TypeName FormList
int down_program47_48(Node *node) {
    if (pos == total) {
        DownErrorFinish("Param");
        return -1;
    }
    if (CheckPredict(47)) {
        node->attr.ProcAttr.paramt = valparamtype;
        int ans = down_program89_90_91(node);
        if (ans == -1) {
            return -1;
        }
        return down_program31(node);
    }
    else if (CheckPredict(48)) {
        ++pos;
        node->attr.ProcAttr.paramt = varparamtype;
        int ans = down_program89_90_91(node);
        if (ans == -1) {
            return -1;
        }
        return down_program31(node);
    }
    DownErrorPredict({47, 48}, "Param");
    return -1;
}

// ParamDecList->Param ParamMore
Node* down_program49() {
    Node *node = new Node;
    node->nodekind = Deck;
    int ans = down_program47_48(node);
    if (ans != 0) {
        delete node;
        return nullptr;
    }
    ans = down_program52_53(node);
    if (ans != 0) {
        delete node;
        return nullptr;
    }
    return node;
}

// ParamList->null ParamList->ParamDecList
int down_program50_51(Node *node) {
    if (pos == total) {
        DownErrorFinish("ParamList");
        return -1;
    }
    if (CheckPredict(50)) {
        return 0;
    }
    else if (CheckPredict(51)) {
        Node *node1 = down_program49();
        if (node1 == nullptr)
            return -1;
        node->child[0] = node1;
        return 0;
    }
    DownErrorPredict({50, 51}, "ParamList");
    return -1;
}

// ProcName->ID
int down_program61(Node *node) {
    if (pos == total) {
        DownErrorFinish("ProcName");
        return -1;
    }
    if (tokens[pos].lex != ID) {
        DownErrorSymbol("ID", "ProcName");
        return -1;
    }
    node->lineno = tokens[pos].rownumber;
    node->name.emplace_back(tokens[pos++].sem);
    return 0;
}

// ProcDeclaration->PROCEDURE ProcName ( ParamList ) ; ProcDecPart ProcBody ProcDecMore
Node* down_program60() {
    if (pos == total) {
        DownErrorFinish("ProcDeclaration");
        return nullptr;
    }
    if (!CheckPredict(60)) {
        DownErrorPredict({60}, "ProcDeclaration");
        return nullptr;
    }
    ++pos;
    Node *node = new Node;
    node->nodekind = ProcDeck;
    int ans = down_program61(node);
    if (ans == -1) {
        delete node;
        return nullptr;
    }
    if (pos == total) {
        DownErrorFinish("ProcDeclaration");
        delete node;
        return nullptr;
    }
    if (tokens[pos].sem != "(") {
        DownErrorSymbol("(", "ProcDeclaration");
        delete node;
        return nullptr;
    }
    ++pos;
    ans = down_program50_51(node);
    if (ans == -1) {
        delete node;
        return nullptr;
    }
    if (pos == total) {
        DownErrorFinish("ProcDeclaration");
        delete node;
        return nullptr;
    }
    if (tokens[pos].sem != ")") {
        DownErrorSymbol(")", "ProcDeclaration");
        delete node;
        return nullptr;
    }
    ++pos;
    if (pos == total) {
        DownErrorFinish("ProcDeclaration");
        delete node;
        return nullptr;
    }
    if (tokens[pos].sem != ";") {
        DownErrorSymbol(";", "ProcDeclaration");
        delete node;
        return nullptr;
    }
    ++pos;
    ans = down_program59(node);
    if (ans == -1) {
        delete node;
        return nullptr;
    }
    ans = down_program54(node);
    if (ans == -1) {
        delete node;
        return nullptr;
    }
    ans = down_program57_58(node);
    if (ans == -1) {
        delete node;
        return nullptr;
    }
    return node;
}

// ProcDec->null ProcDec->ProcDeclaration
int down_program55_56(Node *node) {
    if (pos == total) {
        DownErrorFinish("ProcDec");
        return -1;
    }
    if (CheckPredict(55)) {
        return 0;
    }
    else if (CheckPredict(56)) {
        Node *node1 = down_program60();
        if (node1 == nullptr)
            return -1;
        node->child[0] = node1;
        return 0;
    }
    DownErrorPredict({55, 56}, "ProcDec");
    return -1;
}

// VarDecMore->null VarDecMore->VarDecList
int down_program95_96(Node *node) {
    if (pos == total) {
        DownErrorFinish("VarDecMore");
        return -1;
    }
    if (CheckPredict(95)) {
        return 0;
    }
    else if (CheckPredict(96)) {
        Node *node1 = down_program94();
        if (node1 == nullptr)
            return -1;
        node->sibling = node1;
        return 0;
    }
    DownErrorPredict({95, 96}, "VarDecMore");
    return -1;
}

// VarIdMore->null VarIdMore->, VarIdList
int down_program99_100(Node *node) {
    if (pos == total) {
        DownErrorFinish("VarIdMore");
        return -1;
    }
    if (CheckPredict(99)) {
        return 0;
    }
    else if (CheckPredict(100)) {
        ++pos;
        return down_program98(node);
    }
    DownErrorPredict({99, 100}, "VarIdMore");
    return -1;
}

// VarIdList->ID VarIdMore
int down_program98(Node *node) {
    if (pos == total) {
        DownErrorFinish("VarIdList");
        return -1;
    }
    if (!CheckPredict(98)) {
        DownErrorPredict({98}, "VarIdList");
        return -1;
    }
    node->name.emplace_back(tokens[pos++].sem);
    return down_program99_100(node);
}

// VarDecList->TypeName VarIdList ; VarDecMore
Node* down_program94() {
    Node *node = new Node;
    node->nodekind = Deck;
    int ans = down_program89_90_91(node);
    if (ans != 0) {
        delete node;
        return nullptr;
    }
    ans = down_program98(node);
    if (ans != 0) {
        delete node;
        return nullptr;
    }
    if (pos == total) {
        DownErrorFinish("VarDecList");
        delete node;
        return nullptr;
    }
    if (tokens[pos].sem != ";") {
        DownErrorSymbol(";", "VarDecList");
        delete node;
        return nullptr;
    }
    ++pos;
    ans = down_program95_96(node);
    if (ans != 0) {
        delete node;
        return nullptr;
    }
    return node;
}

// VarDeclaration->VAR VarDecList
int down_program97(Node *node) {
    if (pos == total) {
        DownErrorFinish("VarDeclaration");
        return -1;
    }
    if (!CheckPredict(97)) {
        DownErrorPredict({97}, "VarDeclaration");
        return -1;
    }
    ++pos;
    Node *node1 = down_program94();
    if (node1 == nullptr)
        return -1;
    node->child[0] = node1;
    return 0;
}

// VarDec->null VarDec->VarDeclaration
int down_program92_93(Node *node) {
    if (pos == total) {
        DownErrorFinish("VarDec");
        return -1;
    }
    if(CheckPredict(92)) {
        return 0;
    }
    else if (CheckPredict(93)) {
        return down_program97(node);
    }
    DownErrorPredict({92, 93}, "VarDec");
    return -1;
}

// TypeDecMore->null TypeDecMore->TypeDecList
int down_program85_86(Node *node) {
    if (pos == total) {
        DownErrorFinish("TypeDecMore");
        return -1;
    }
    if (CheckPredict(85)) {
        return 0;
    }
    else if (CheckPredict(86)) {
        Node *node1 = down_program84();
        if (node1 == nullptr)
            return -1;
        node->sibling = node1;
        return 0;
    }
    DownErrorPredict({85, 86}, "TypeDecMore");
    return -1;
}

// FieldDecMore->null FieldDecMore->FieldDecList
int down_program26_27(Node *node) {
    if (pos == total) {
        DownErrorFinish("FieldDecMore");
        return -1;
    }
    if (CheckPredict(26)) {
        return 0;
    }
    else if (CheckPredict(27)) {
        Node *node1 = down_program24_25();
        if (node1 == nullptr)
            return -1;
        node->sibling = node1;
        return 0;
    }
    DownErrorPredict({26, 27}, "FieldDecMore");
    return -1;
}

// IdMore->null IdMore->, IdList
int down_program33_34(Node *node) {
    if (pos == total) {
        DownErrorFinish("IdMore");
        return -1;
    }
    if (CheckPredict(33)) {
        return 0;
    }
    else if (CheckPredict(34)) {
        ++pos;
        return down_program32(node);
    }
    DownErrorPredict({33, 34}, "IdMore");
    return -1;
}

// IdList->ID IdMore
int down_program32(Node *node) {
    if (pos == total) {
        DownErrorFinish("IdList");
        return -1;
    }
    if (tokens[pos].lex != ID) {
        DownErrorSymbol("ID", "IdList");
        return -1;
    }
    node->name.emplace_back(tokens[pos++].sem);
    return down_program33_34(node);
}

// FieldDecList->BaseType IdList ; FieldDecMore FieldDecList->ArrayType IdList ; FieldDecMore
Node* down_program24_25() {
    if (pos == total) {
        DownErrorFinish("FieldDecList");
        return nullptr;
    }
    if (CheckPredict(24)) {
        Node *node = new Node;
        node->nodekind = Deck;
        int ans = down_program11_12(node);
        if (ans != 0) {
            delete node;
            return nullptr;
        }
        ans = down_program32(node);
        if (ans != 0) {
            delete node;
            return nullptr;
        }
        if (pos == total) {
            DownErrorFinish("FieldDecList");
            delete node;
            return nullptr;
        }
        if (tokens[pos].sem != ";") {
            DownErrorSymbol(";", "FieldDecList");
            delete node;
            return nullptr;
        }
        ++pos;
        ans = down_program26_27(node);
        if (ans != 0) {
            delete node;
            return nullptr;
        }
        return node;
    }
    else if (CheckPredict(25)) {
        Node *node = new Node;
        node->nodekind = Deck;
        int ans = down_program7(node);
        if (ans != 0) {
            delete node;
            return nullptr;
        }
        ans = down_program32(node);
        if (ans != 0) {
            delete node;
            return nullptr;
        }
        if (pos == total) {
            DownErrorFinish("FieldDecList");
            delete node;
            return nullptr;
        }
        if (tokens[pos].sem != ";") {
            DownErrorSymbol(";", "FieldDecList");
            delete node;
            return nullptr;
        }
        ++pos;
        ans = down_program26_27(node);
        if (ans != 0) {
            delete node;
            return nullptr;
        }
        return node;
    }
    DownErrorPredict({24, 25}, "FieldDecList");
    return nullptr;
}

// RecType->RECORD FieldDecList END
int down_program66(Node *node) {
    if (pos == total) {
        DownErrorFinish("RecType");
        return -1;
    }
    if (tokens[pos].sem != "RECORD") {
        DownErrorSymbol("RECORD", "RecType");
        return -1;
    }
    ++pos;
    Node *node1 = down_program24_25();
    if (node1 == nullptr)
        return -1;
    node->child[0] = node1;
    if (pos == total) {
        DownErrorFinish("RecType");
        return -1;
    }
    if (tokens[pos].sem != "END") {
        DownErrorSymbol("END", "RecType");
        return -1;
    }
    ++pos;
    return 0;
}

// Top->INTC
int down_program81(Node *node) {
    if (pos == total) {
        DownErrorFinish("Top");
        return -1;
    }
    if (tokens[pos].lex != DIGIT) {
        DownErrorSymbol("INTC", "Top");
        return -1;
    }
    node->attr.ArrayAttr.up = stoi(tokens[pos++].sem);
    return 0;
}

// Low->INTC
int down_program38(Node *node) {
    if (pos == total) {
        DownErrorFinish("Low");
        return -1;
    }
    if (tokens[pos].lex != DIGIT) {
        DownErrorSymbol("INTC", "Low");
        return -1;
    }
    node->attr.ArrayAttr.low = stoi(tokens[pos++].sem);
    return 0;
}

// ArrayType->ARRAY [ Low .. Top ] OF BaseType
int down_program7(Node *node) {
    if (pos == total) {
        DownErrorFinish("ArrayType");
        return -1;
    }
    if (tokens[pos].sem != "ARRAY") {
        DownErrorSymbol("ARRAY", "ArrayType");
        return -1;
    }
    ++pos;
    if (pos == total) {
        DownErrorFinish("ArrayType");
        return -1;
    }
    if (tokens[pos].sem != "[") {
        DownErrorSymbol("[", "ArrayType");
        return -1;
    }
    ++pos;
    int ans = down_program38(node);
    if (ans == -1)
        return -1;
    if (pos == total) {
        DownErrorFinish("ArrayType");
        return -1;
    }
    if (tokens[pos].sem != "..") {
        DownErrorSymbol("..", "ArrayType");
        return -1;
    }
    ++pos;
    ans = down_program81(node);
    if (ans == -1)
        return -1;
    if (pos == total) {
        DownErrorFinish("ArrayType");
        return -1;
    }
    if (tokens[pos].sem != "]") {
        DownErrorSymbol("]", "ArrayType");
        return -1;
    }
    ++pos;
    if (pos == total) {
        DownErrorFinish("ArrayType");
        return -1;
    }
    if (tokens[pos].sem != "OF") {
        DownErrorSymbol("OF", "ArrayType");
        return -1;
    }
    ++pos;
    ans = down_program11_12(node);
    node->attr.ArrayAttr.childType = node->kind.dec;
    node->kind.dec = ArrayK;
    return ans;
}

// StructureType->ArrayType StructureType->RecType
int down_program78_79(Node *node) {
    if (pos == total) {
        DownErrorFinish("StructureType");
        return -1;
    }
    if (CheckPredict(78)) {
        return down_program7(node);
    }
    else if (CheckPredict(79)) {
        node->kind.dec = RecordK;
        return down_program66(node);
    }
    DownErrorPredict({78, 79}, "StructureType");
    return -1;
}

// BaseType->INTEGER BaseType->CHAR
int down_program11_12(Node *node) {
    if (pos == total) {
        DownErrorFinish("BaseType");
        return -1;
    }
    if (tokens[pos].sem == "INTEGER") {
        node->lineno = tokens[pos++].rownumber;
        node->kind.dec = IntegerK;
        return 0;
    }
    else if (tokens[pos].sem == "CHAR") {
        node->lineno = tokens[pos++].rownumber;
        node->kind.dec = CharK;
        return 0;
    }
    DownErrorPredict({11, 12}, "BaseType");
    return -1;
}

// TypeName->BaseType TypeName->StructureType TypeName->ID
int down_program89_90_91(Node *node) {
    if (pos == total) {
        DownErrorFinish("TypeId");
        return -1;
    }
    if (CheckPredict(89)) {
        return down_program11_12(node);
    }
    else if (CheckPredict(90)) {
        return down_program78_79(node);
    }
    else if (tokens[pos].lex == ID) {
        node->lineno = tokens[pos].rownumber;
        node->kind.dec = Idk;
        node->type_name = tokens[pos++].sem;
        return 0;
    }
    DownErrorPredict({89, 90, 91}, "TypeName");
    return -1;
}

// TypeId->ID
int down_program88(Node *node) {
    if (pos == total) {
        DownErrorFinish("TypeId");
        return -1;
    }
    if (tokens[pos].lex != ID) {
        DownErrorSymbol("ID", "TypeId");
        return -1;
    }
    node->lineno = tokens[pos].rownumber;
    node->name.emplace_back(tokens[pos++].sem);
    return 0;
}

// TypeDecList->TypeId = TypeName ; TypeDecMore
Node* down_program84() {
    Node *node = new Node;
    node->nodekind = Deck;
    int ans = down_program88(node);
    if (ans != 0) {
        delete node;
        return nullptr;
    }
    if (pos == total) {
        DownErrorFinish("TypeDecList");
        delete node;
        return nullptr;
    }
    if (tokens[pos].sem != "=") {
        DownErrorSymbol("=", "TypeDecList");
        delete node;
        return nullptr;
    }
    ++pos;
    ans = down_program89_90_91(node);
    if (ans != 0) {
        delete node;
        return nullptr;
    }
    if (pos == total) {
        DownErrorFinish("TypeDecList");
        delete node;
        return nullptr;
    }
    if (tokens[pos].sem != ";") {
        DownErrorSymbol(";", "TypeDecList");
        delete node;
        return nullptr;
    }
    ++pos;
    ans = down_program85_86(node);
    if (ans != 0) {
        delete node;
        return nullptr;
    }
    return node;
}

// TypeDeclaration->TYPE TypeDecList
Node* down_program87() {
    if (pos == total) {
        DownErrorFinish("TypeDeclarartion");
        return nullptr;
    }
    if (tokens[pos].sem != "TYPE") {
        DownErrorSymbol("TYPE", "TypeDeclarartion");
        return nullptr;
    }
    ++pos;
    return down_program84();
}

// TypeDec->null TypeDec->TypeDeclaration
int down_program82_83(Node *node) {
    if (pos == total) {
        DownErrorFinish("TypeDec");
        return -1;
    }
    if (CheckPredict(82)) {
        return 0;
    }
    else if (CheckPredict(83)) {
        Node *typenodes = down_program87();
        if (typenodes == nullptr)
            return -1;
        node->child[0] = typenodes;
        return 0;
    }
    DownErrorPredict({82, 83}, "TypeDec");
    return -1;
}

// DeclarePart->TypeDec VarDec ProcDec
Node* down_program17() {
    Node *TypeNode = new Node, *VarNode = new Node, *ProcNode = new Node;
    TypeNode->sibling = VarNode, VarNode->sibling = ProcNode;
    TypeNode->nodekind = TypeK;
    VarNode->nodekind = VarK;
    ProcNode->nodekind = ProcK;
    int ans = down_program82_83(TypeNode);
    if (ans != 0) {
        delete TypeNode;
        return nullptr;
    }
    ans = down_program92_93(VarNode);
    if (ans != 0) {
        delete TypeNode;
        return nullptr;
    }
    ans = down_program55_56(ProcNode);
    if (ans != 0) {
        delete TypeNode;
        return nullptr;
    }
    return TypeNode;
}

// ProgramName->ID
int down_program65(Node *node) {
    if (pos == total) {
        DownErrorFinish("ProgramName");
        return -1;
    }
    if (tokens[pos].lex != ID) {
        DownErrorSymbol("ID", "ProgramName");
        return -1;
    }
    node->lineno = tokens[pos].rownumber;
    node->name.emplace_back(tokens[pos++].sem);
    return 0;
}

// ProgramHead->PROGRAM ProgramName
Node* down_program64() {
    if (pos == total) {
        DownErrorFinish("ProgramHead");
        return nullptr;
    }
    if (tokens[pos].sem != "PROGRAM") {
        DownErrorSymbol("PROGRAM", "ProgramHead");
        return nullptr;
    }
    ++pos;
    Node *node = new Node;
    node->nodekind = PheadK;
    int ans = down_program65(node);
    if (ans == -1) {
        delete node;
        return nullptr;
    }
    return node;
}

// Program->ProgramHead DeclarePart ProgramBody
Node* down_program62() {
    Node *root = new Node;
    root->nodekind = ProK;
    Node *node = down_program64();
    if (node == nullptr) {
        delete root;
        return nullptr;
    }
    root->child[0] = node;
    node = down_program17();
    if (node == nullptr) {
        delete root;
        return nullptr;
    }
    root->child[1] = node;
    node = down_program63();
    if (node == nullptr) {
        delete root;
        return nullptr;
    }
    root->child[2] = node;
    return root;
}

Node* DownParser() {
    pos = 0;
    total = tokens.size();
    return down_program62();
}
