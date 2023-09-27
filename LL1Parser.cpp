#include "LL1Parser.h"
#include "PreTask.h"
#include "ParserPreTask.h"
#include "SynTaxTree.h"
#include "lexical.h"

#include <stack>

enum LexType { ADD, SUB, MUL, DIV, END, LOWER, EQUAL, LEFT, RIGHT };

QString LL1ErrorMessage;
int LL1ErrorLine;

static std::stack<int> SymbolStack;
static std::stack<Node**> TreeStack;
static std::stack<LexType> OpStack;
static std::stack<Node*> ExpStack;
static Node* TreeRoot, *currentP;
static int pos, total, line, expFlag;
static Dec_kind *temp;
static std::string name;
static bool getExpResult, getExpResult2;

static Node* MakeNode(LexType op) {
    Node *ans = new Node;
    ans->nodekind = ExpK;
    ans->kind.exp = OpK;
    if (op == ADD) {
        ans->attr.ExpAttr.op = PLUS;
    }
    else if (op == SUB) {
        ans->attr.ExpAttr.op = MINUS;
    }
    else if (op == MUL) {
        ans->attr.ExpAttr.op = TIMES;
    }
    else if (op == DIV) {
        ans->attr.ExpAttr.op = OVER;
    }
    else if (op == LOWER) {
        ans->attr.ExpAttr.op = LT;
    }
    else {
        ans->attr.ExpAttr.op = EQ;
    }
    return ans;
}

static int Priority(LexType op) {
    switch (op) {
        case END:
            return 0;
        case LOWER:
        case EQUAL:
            return 1;
        case ADD:
        case SUB:
            return 2;
        case MUL:
        case DIV:
            return 3;
        default:
        return -1;
    }
}

static void PushRight(int pos) {
    const auto &rightPart = ProductionsInCode[pos].getRightPart();
    for (auto it = rightPart.rbegin(); it != rightPart.rend(); ++it) {
        SymbolStack.push(*it);
    }
}

static inline bool isTerminal(const int &code) {
    return TSymbolCode.count(code);
}

// ActParamList->null
static void program1() {
    TreeStack.pop();
}

// ActParamList->Exp ActParamMore
static void program2() {
    PushRight(1);
    OpStack.push(END);
}

// ActParamMore->null
static void program3() {
    return;
}

// ActParamMore->, ActParamList
static void program4() {
    TreeStack.push(&currentP->sibling);
}

// AddOp->+
static void program5() {
    PushRight(4);
}

// AddOp->-
static void program6() {
    PushRight(5);
}

// ArrayType->ARRAY [ Low .. Top ] OF BaseType
static void program7() {
    PushRight(6);
    temp = &currentP->attr.ArrayAttr.childType;
    currentP->kind.dec = ArrayK;
}

// AssCall->AssignmentRest
static void program8() {
    PushRight(7);
    currentP->kind.stmt = AssignK;
}

// AssCall->CallStmRest
static void program9() {
    PushRight(8);
    currentP->kind.stmt = CallK;
    currentP->name.emplace_back(name);
    currentP->lineno = line;
}

// AssignmentRest->VariMore := Exp
static void program10() {
    PushRight(9);
    TreeStack.push(&currentP->child[1]);
    currentP = currentP->child[0];
    OpStack.push(END);
    getExpResult = true;
}

// BaseType->INTEGER
static void program11() {
    PushRight(10);
    *temp = IntegerK;
}

// BaseType->CHAR
static void program12() {
    PushRight(11);
    *temp = CharK;
}

// CallStmRest->( ActParamList )
static void program13() {
    PushRight(12);
    TreeStack.push(&currentP->child[0]);
}

// CmpOp-><
static void program14() {
    PushRight(13);
}

// CmpOp->=
static void program15() {
    PushRight(14);
}

// ConditionalStm->IF RelExp THEN StmList ELSE StmList FI
static void program16() {
    PushRight(15);
    TreeStack.push(&currentP->child[2]);
    TreeStack.push(&currentP->child[1]);
    TreeStack.push(&currentP->child[0]);
}

// DeclarePart->TypeDec VarDec ProcDec
static void program17() {
    PushRight(16);
    Node *TypeNode = new Node, *VarNode = new Node, *ProcNode = new Node;
    TypeNode->sibling = VarNode;
    VarNode->sibling = ProcNode;
    TypeNode->nodekind = TypeK;
    VarNode->nodekind = VarK;
    ProcNode->nodekind = ProcK;
    Node** node = TreeStack.top();
    TreeStack.pop();
    *node = TypeNode;
    TreeStack.push(&ProcNode->child[0]);
    TreeStack.push(&VarNode->child[0]);
    TreeStack.push(&TypeNode->child[0]);
}

// Exp->Term OtherTerm
static void program18() {
    PushRight(17);
}

// Factor->( Exp )
static void program19() {
    PushRight(18);
    ++expFlag;
    OpStack.push(LEFT);
}

// Factor->INTC
static void program20() {
    PushRight(19);
    currentP = new Node;
    currentP->nodekind = ExpK;
    currentP->kind.exp = ConstK;
    currentP->attr.ExpAttr.val = stoi(tokens[pos].sem);
    currentP->lineno = tokens[pos].rownumber;
    ExpStack.push(currentP);
}

// Factor->Variable
static void program21() {
    PushRight(20);
}

// FidMore->null
static void program22() {
    return;
}

// FidMore->, FormList
static void program23() {
    PushRight(22);
}

// FieldDecList->BaseType IdList ; FieldDecMore
static void program24() {
    PushRight(23);
    currentP = new Node;
    Node **node = TreeStack.top();
    TreeStack.pop();
    *node = currentP;
    TreeStack.push(&currentP->sibling);
    currentP->nodekind = Deck;
    temp = &currentP->kind.dec;
}

// FieldDecList->ArrayType IdList ; FieldDecMore
static void program25() {
    PushRight(24);
    currentP = new Node;
    Node **node = TreeStack.top();
    TreeStack.pop();
    *node = currentP;
    TreeStack.push(&currentP->sibling);
    currentP->nodekind = Deck;
}

// FieldDecMore->null
static void program26() {
    TreeStack.pop();
}

// FieldDecMore->FieldDecList
static void program27() {
    PushRight(26);
}

// FieldVar->ID FieldVarMore
static void program28() {
    PushRight(27);
    currentP = new Node;
    name = tokens[pos].sem;
    line = tokens[pos].rownumber;
    currentP->nodekind = ExpK;
    currentP->kind.exp = Exp_IdK;
    Node **node = TreeStack.top();
    TreeStack.pop();
    *node = currentP;
}

// FieldVarMore->null
static void program29() {
    currentP->name.emplace_back(name);
    currentP->lineno = line;
    currentP->attr.ExpAttr.varkind = IdV;
}

// FieldVarMore->[ Exp ]
static void program30() {
    PushRight(29);
    currentP->name.emplace_back(name);
    currentP->lineno = line;
    currentP->attr.ExpAttr.varkind = ArrayMembV;
    OpStack.push(END);
    TreeStack.push(&currentP->child[0]);
    getExpResult2 = true;
}

// FormList->ID FidMore
static void program31() {
    PushRight(30);
    currentP->name.emplace_back(tokens[pos].sem);
    currentP->lineno = tokens[pos].rownumber;
}

// IdList->ID IdMore
static void program32() {
    PushRight(31);
    currentP->name.emplace_back(tokens[pos].sem);
    currentP->lineno = tokens[pos].rownumber;
}

// IdMore->null
static void program33() {
    return;
}

// IdMore->, IdList
static void program34() {
    PushRight(33);
}

// InputStm->READ ( Invar )
static void program35() {
    PushRight(34);
}

// Invar->ID
static void program36() {
    PushRight(35);
    currentP->lineno = tokens[pos].rownumber;
    currentP->name.emplace_back(tokens[pos].sem);
}

// LoopStm->WHILE RelExp DO StmList ENDWH
static void program37() {
    PushRight(36);
    TreeStack.push(&currentP->child[1]);
    TreeStack.push(&currentP->child[0]);
}

// Low->INTC
static void program38() {
    PushRight(37);
    currentP->attr.ArrayAttr.low = std::stoi(tokens[pos].sem);
}

// MultOp->*
static void program39() {
    PushRight(38);
}

// MultOp->/
static void program40() {
    PushRight(39);
}

// OtherFactor->null
static void program41() {
    return;
}

// OtherFactor->MultOp Term
static void program42() {
    PushRight(41);
    LexType NowOp;
    if (tokens[pos].sem == "*")
        NowOp = MUL;
    else
        NowOp = DIV;
    while (OpStack.top() == MUL || OpStack.top() == DIV) {
        Node *node = MakeNode(OpStack.top());
        OpStack.pop();
        node->child[1] = ExpStack.top();
        ExpStack.pop();
        node->child[0] = ExpStack.top();
        ExpStack.pop();
        ExpStack.push(node);
    }
    OpStack.push(NowOp);
}

// OtherRelE->CmpOp Exp
static void program43() {
    PushRight(42);
    LexType NowOp;
    if (tokens[pos].sem == "<") {
        NowOp = LOWER;
    }
    else {
        NowOp = EQUAL;
    }
    int NowPriority = 1;
    while (Priority(OpStack.top()) >= NowPriority) {
        LexType op = OpStack.top();
        OpStack.pop();
        Node *node = MakeNode(op);
        node->child[1] = ExpStack.top();
        ExpStack.pop();
        node->child[0] = ExpStack.top();
        ExpStack.pop();
        ExpStack.push(node);
    }
    getExpResult = true;
    OpStack.push(NowOp);
}

// OtherTerm->null
static void program44() {
    if (tokens[pos].sem == ")" && expFlag != 0) {
        while (OpStack.top() != LEFT) {
            Node *node = MakeNode(OpStack.top());
            OpStack.pop();
            node->child[1] = ExpStack.top();
            ExpStack.pop();
            node->child[0] = ExpStack.top();
            ExpStack.pop();
            ExpStack.push(node);
        }
        OpStack.pop();
        --expFlag;
    }
    else if (getExpResult || getExpResult2) {
        while (OpStack.top() != END) {
            Node *node = MakeNode(OpStack.top());
            OpStack.pop();
            node->child[1] = ExpStack.top();
            ExpStack.pop();
            node->child[0] = ExpStack.top();
            ExpStack.pop();
            ExpStack.push(node);
        }
        OpStack.pop();
        Node **node = TreeStack.top();
        TreeStack.pop();
        currentP = *node = ExpStack.top();
        ExpStack.pop();
        getExpResult2 = false;
    }
}

// OtherTerm->AddOp Exp
static void program45() {
    PushRight(44);
    LexType nowop;
    if (tokens[pos].sem == "+")
        nowop = ADD;
    else
        nowop = SUB;
    int NowPrio = 2;
    while (Priority(OpStack.top()) >= NowPrio) {
        Node *node = MakeNode(OpStack.top());
        OpStack.pop();
        node->child[1] = ExpStack.top();
        ExpStack.pop();
        node->child[0] = ExpStack.top();
        ExpStack.pop();
        ExpStack.push(node);
    }
    OpStack.push(nowop);
}

// OutputStm->WRITE ( Exp )
static void program46() {
    PushRight(45);
    TreeStack.push(&currentP->child[0]);
    OpStack.push(END);
}

// Param->TypeName FormList
static void program47() {
    PushRight(46);
    currentP = new Node;
    currentP->nodekind = Deck;
    currentP->attr.ProcAttr.paramt = valparamtype;
    Node **node = TreeStack.top();
    TreeStack.pop();
    *node = currentP;
    TreeStack.push(&currentP->sibling);
}

// Param->VAR TypeName FormList
static void program48() {
    PushRight(47);
    currentP = new Node;
    currentP->nodekind = Deck;
    currentP->attr.ProcAttr.paramt = varparamtype;
    Node **node = TreeStack.top();
    TreeStack.pop();
    *node = currentP;
    TreeStack.push(&currentP->sibling);
}

// ParamDecList->Param ParamMore
static void program49() {
    PushRight(48);
}

// ParamList->null
static void program50() {
    TreeStack.pop();
}

// ParamList->ParamDecList
static void program51() {
    PushRight(50);
}

// ParamMore->null
static void program52() {
    TreeStack.pop();
}

// ParamMore->; ParamDecList
static void program53() {
    PushRight(52);
}

// ProcBody->ProgramBody
static void program54() {
    PushRight(53);
}

// ProcDec->null
static void program55() {
    TreeStack.pop();
}

// ProcDec->ProcDeclaration
static void program56() {
    PushRight(55);
}

// ProcDecMore->null
static void program57() {
    TreeStack.pop();
}

// ProcDecMore->ProcDeclaration
static void program58() {
    PushRight(57);
}

// ProcDecPart->DeclarePart
static void program59() {
    PushRight(58);
}

// ProcDeclaration->PROCEDURE ProcName ( ParamList ) ; ProcDecPart ProcBody ProcDecMore
static void program60() {
    PushRight(59);
    currentP = new Node;
    currentP->nodekind = ProcDeck;
    Node **node = TreeStack.top();
    TreeStack.pop();
    *node = currentP;
    TreeStack.push(&currentP->sibling);
    TreeStack.push(&currentP->child[2]);
    TreeStack.push(&currentP->child[1]);
    TreeStack.push(&currentP->child[0]);
}

// ProcName->ID
static void program61() {
    PushRight(60);
    currentP->name.emplace_back(tokens[pos].sem);
    currentP->lineno = tokens[pos].rownumber;
}

// Program->ProgramHead DeclarePart ProgramBody
static void program62() {
    PushRight(61);
    TreeRoot = new Node;
    TreeRoot->nodekind = ProK;
    TreeStack.push(&TreeRoot->child[2]);
    TreeStack.push(&TreeRoot->child[1]);
    TreeStack.push(&TreeRoot->child[0]);
}

// ProgramBody->BEGIN StmList END
static void program63() {
    PushRight(62);
    currentP = new Node;
    currentP->nodekind = StmLK;
    Node **node = TreeStack.top();
    TreeStack.pop();
    *node = currentP;
    TreeStack.push(&currentP->child[0]);
}

// ProgramHead->PROGRAM ProgramName
static void program64() {
    PushRight(63);
    Node** node = TreeStack.top();
    TreeStack.pop();
    currentP = new Node;
    *node = currentP;
    currentP->nodekind = PheadK;
}

// ProgramName->ID
static void program65() {
    PushRight(64);
    currentP->name.emplace_back(tokens[pos].sem);
    currentP->lineno = tokens[pos].rownumber;
}

// RecType->RECORD FieldDecList END
static void program66() {
    PushRight(65);
    currentP->kind.dec = RecordK;
    TreeStack.push(&currentP->child[0]);
}

// RelExp->Exp OtherRelE
static void program67() {
    PushRight(66);
    OpStack.push(END);
    getExpResult = false;
}

// ReturnStm->RETURN
static void program68() {
    PushRight(67);
}

// Stm->ConditionalStm
static void program69() {
    PushRight(68);
    currentP = new Node;
    currentP->nodekind = StmtK;
    currentP->kind.stmt = IfK;
    Node **node = TreeStack.top();
    TreeStack.pop();
    *node = currentP;
    TreeStack.push(&currentP->sibling);
}

// Stm->LoopStm
static void program70() {
    PushRight(69);
    currentP = new Node;
    currentP->nodekind = StmtK;
    currentP->kind.stmt = WhileK;
    Node **node = TreeStack.top();
    TreeStack.pop();
    *node = currentP;
    TreeStack.push(&currentP->sibling);
}

// Stm->InputStm
static void program71() {
    PushRight(70);
    currentP = new Node;
    currentP->nodekind = StmtK;
    currentP->kind.stmt = ReadK;
    Node **node = TreeStack.top();
    TreeStack.pop();
    *node = currentP;
    TreeStack.push(&currentP->sibling);
}

// Stm->OutputStm
static void program72() {
    PushRight(71);
    currentP = new Node;
    currentP->nodekind = StmtK;
    currentP->kind.stmt = WriteK;
    Node **node = TreeStack.top();
    TreeStack.pop();
    *node = currentP;
    TreeStack.push(&currentP->sibling);
}

// Stm->ReturnStm
static void program73() {
    PushRight(72);
    currentP = new Node;
    currentP->nodekind = StmtK;
    currentP->kind.stmt = ReturnK;
    Node **node = TreeStack.top();
    TreeStack.pop();
    *node = currentP;
    TreeStack.push(&currentP->sibling);
}

// Stm->ID AssCall
static void program74() {
    PushRight(73);
    currentP = new Node;
    currentP->nodekind = StmtK;
    currentP->child[0] = new Node;
    currentP->child[0]->nodekind = ExpK;
    name = tokens[pos].sem;
    line = tokens[pos].rownumber;
    Node **node = TreeStack.top();
    TreeStack.pop();
    *node = currentP;
    TreeStack.push(&currentP->sibling);
}

// StmList->Stm StmMore
static void program75() {
    PushRight(74);
}

// StmMore->null
static void program76() {
    TreeStack.pop();
}

// StmMore->; StmList
static void program77() {
    PushRight(76);
}

// StructureType->ArrayType
static void program78() {
    PushRight(77);
}

// StructureType->RecType
static void program79() {
    PushRight(78);
}

// Term->Factor OtherFactor
static void program80() {
    PushRight(79);
}

// Top->INTC
static void program81() {
    PushRight(80);
    currentP->attr.ArrayAttr.up = std::stoi(tokens[pos].sem);
}

// TypeDec->null
static void program82() {
    TreeStack.pop();
}

// TypeDec->TypeDeclaration
static void program83() {
    PushRight(82);
}

// TypeDecList->TypeId = TypeName ; TypeDecMore
static void program84() {
    PushRight(83);
    currentP = new Node;
    currentP->nodekind = Deck;
    Node **node = TreeStack.top();
    TreeStack.pop();
    *node = currentP;
    TreeStack.push(&currentP->sibling);
}

// TypeDecMore->null
static void program85() {
    TreeStack.pop();
}

// TypeDecMore->TypeDecList
static void program86() {
    PushRight(85);
}

// TypeDeclaration->TYPE TypeDecList
static void program87() {
    PushRight(86);
}

// TypeId->ID
static void program88() {
    PushRight(87);
    currentP->lineno = tokens[pos].rownumber;
    currentP->name.emplace_back(tokens[pos].sem);
}

// TypeName->BaseType
static void program89() {
    PushRight(88);
    temp = &currentP->kind.dec;
}

// TypeName->StructureType
static void program90() {
    PushRight(89);
}

// TypeName->ID
static void program91() {
    PushRight(90);
    currentP->lineno = tokens[pos].rownumber;
    currentP->kind.dec = Idk;
    currentP->type_name = tokens[pos].sem;
}

// VarDec->null
static void program92() {
    TreeStack.pop();
}

// VarDec->VarDeclaration
static void program93() {
    PushRight(92);
}

// VarDecList->TypeName VarIdList ; VarDecMore
static void program94() {
    PushRight(93);
    currentP = new Node;
    currentP->nodekind = Deck;
    Node **node = TreeStack.top();
    TreeStack.pop();
    *node = currentP;
    TreeStack.push(&currentP->sibling);
}

// VarDecMore->null
static void program95() {
    TreeStack.pop();
}

// VarDecMore->VarDecList
static void program96() {
    PushRight(95);
}

// VarDeclaration->VAR VarDecList
static void program97() {
    PushRight(96);
}

// VarIdList->ID VarIdMore
static void program98() {
    PushRight(97);
    currentP->lineno = tokens[pos].rownumber;
    currentP->name.emplace_back(tokens[pos].sem);
}

// VarIdMore->null
static void program99() {
    return;
}

// VarIdMore->, VarIdList
static void program100() {
    PushRight(99);
}

// VariMore->null
static void program101() {
    currentP->name.emplace_back(name);
    currentP->lineno = line;
    currentP->kind.exp = Exp_IdK;
    currentP->attr.ExpAttr.varkind = IdV;
}

// VariMore->[ Exp ]
static void program102() {
    PushRight(101);
    currentP->name.emplace_back(name);
    currentP->lineno = line;
    currentP->kind.exp = Exp_IdK;
    currentP->attr.ExpAttr.varkind = ArrayMembV;
    OpStack.push(END);
    TreeStack.push(&currentP->child[0]);
    getExpResult2 = true;
}

// VariMore->. FieldVar
static void program103() {
    PushRight(102);
    currentP->name.emplace_back(name);
    currentP->lineno = line;
    currentP->kind.exp = Exp_IdK;
    currentP->attr.ExpAttr.varkind = FieldMembV;
    TreeStack.push(&currentP->child[0]);
}

// Variable->ID VariMore
static void program104() {
    PushRight(103);
    currentP = new Node;
    name = tokens[pos].sem;
    line = tokens[pos].rownumber;
    currentP->nodekind = ExpK;
    ExpStack.push(currentP);
}

static void CallProgram(int num) {
    if (num == 0) {
        program1();
    }
    else if (num == 1) {
        program2();
    }
    else if (num == 2) {
        program3();
    }
    else if (num == 3) {
        program4();
    }
    else if (num == 4) {
        program5();
    }
    else if (num == 5) {
        program6();
    }
    else if (num == 6) {
        program7();
    }
    else if (num == 7) {
        program8();
    }
    else if (num == 8) {
        program9();
    }
    else if (num == 9) {
        program10();
    }
    else if (num == 10) {
        program11();
    }
    else if (num == 11) {
        program12();
    }
    else if (num == 12) {
        program13();
    }
    else if (num == 13) {
        program14();
    }
    else if (num == 14) {
        program15();
    }
    else if (num == 15) {
        program16();
    }
    else if (num == 16) {
        program17();
    }
    else if (num == 17) {
        program18();
    }
    else if (num == 18) {
        program19();
    }
    else if (num == 19) {
        program20();
    }
    else if (num == 20) {
        program21();
    }
    else if (num == 21) {
        program22();
    }
    else if (num == 22) {
        program23();
    }
    else if (num == 23) {
        program24();
    }
    else if (num == 24) {
        program25();
    }
    else if (num == 25) {
        program26();
    }
    else if (num == 26) {
        program27();
    }
    else if (num == 27) {
        program28();
    }
    else if (num == 28) {
        program29();
    }
    else if (num == 29) {
        program30();
    }
    else if (num == 30) {
        program31();
    }
    else if (num == 31) {
        program32();
    }
    else if (num == 32) {
        program33();
    }
    else if (num == 33) {
        program34();
    }
    else if (num == 34) {
        program35();
    }
    else if (num == 35) {
        program36();
    }
    else if (num == 36) {
        program37();
    }
    else if (num == 37) {
        program38();
    }
    else if (num == 38) {
        program39();
    }
    else if (num == 39) {
        program40();
    }
    else if (num == 40) {
        program41();
    }
    else if (num == 41) {
        program42();
    }
    else if (num == 42) {
        program43();
    }
    else if (num == 43) {
        program44();
    }
    else if (num == 44) {
        program45();
    }
    else if (num == 45) {
        program46();
    }
    else if (num == 46) {
        program47();
    }
    else if (num == 47) {
        program48();
    }
    else if (num == 48) {
        program49();
    }
    else if (num == 49) {
        program50();
    }
    else if (num == 50) {
        program51();
    }
    else if (num == 51) {
        program52();
    }
    else if (num == 52) {
        program53();
    }
    else if (num == 53) {
        program54();
    }
    else if (num == 54) {
        program55();
    }
    else if (num == 55) {
        program56();
    }
    else if (num == 56) {
        program57();
    }
    else if (num == 57) {
        program58();
    }
    else if (num == 58) {
        program59();
    }
    else if (num == 59) {
        program60();
    }
    else if (num == 60) {
        program61();
    }
    else if (num == 61) {
        program62();
    }
    else if (num == 62) {
        program63();
    }
    else if (num == 63) {
        program64();
    }
    else if (num == 64) {
        program65();
    }
    else if (num == 65) {
        program66();
    }
    else if (num == 66) {
        program67();
    }
    else if (num == 67) {
        program68();
    }
    else if (num == 68) {
        program69();
    }
    else if (num == 69) {
        program70();
    }
    else if (num == 70) {
        program71();
    }
    else if (num == 71) {
        program72();
    }
    else if (num == 72) {
        program73();
    }
    else if (num == 73) {
        program74();
    }
    else if (num == 74) {
        program75();
    }
    else if (num == 75) {
        program76();
    }
    else if (num == 76) {
        program77();
    }
    else if (num == 77) {
        program78();
    }
    else if (num == 78) {
        program79();
    }
    else if (num == 79) {
        program80();
    }
    else if (num == 80) {
        program81();
    }
    else if (num == 81) {
        program82();
    }
    else if (num == 82) {
        program83();
    }
    else if (num == 83) {
        program84();
    }
    else if (num == 84) {
        program85();
    }
    else if (num == 85) {
        program86();
    }
    else if (num == 86) {
        program87();
    }
    else if (num == 87) {
        program88();
    }
    else if (num == 88) {
        program89();
    }
    else if (num == 89) {
        program90();
    }
    else if (num == 90) {
        program91();
    }
    else if (num == 91) {
        program92();
    }
    else if (num == 92) {
        program93();
    }
    else if (num == 93) {
        program94();
    }
    else if (num == 94) {
        program95();
    }
    else if (num == 95) {
        program96();
    }
    else if (num == 96) {
        program97();
    }
    else if (num == 97) {
        program98();
    }
    else if (num == 98) {
        program99();
    }
    else if (num == 99) {
        program100();
    }
    else if (num == 100) {
        program101();
    }
    else if (num == 101) {
        program102();
    }
    else if (num == 102) {
        program103();
    }
    else if (num == 103) {
        program104();
    }
}

Node* LL1Parser() {
    while (!SymbolStack.empty())
        SymbolStack.pop();
    while (!TreeStack.empty())
        TreeStack.pop();
    while (!OpStack.empty())
        OpStack.pop();
    while (!ExpStack.empty())
        ExpStack.pop();
    name = "";
    temp = nullptr;
    currentP = TreeRoot = nullptr;
    expFlag = line = pos = 0;
    total = tokens.size();
    getExpResult = true;
    getExpResult2 = false;
    SymbolStack.push(SymboltoCode[StartSymbol]);
    while (!SymbolStack.empty()) {
        if (pos == total) {
            if (TreeRoot != nullptr)
                delete TreeRoot;
            TreeRoot = nullptr;
            LL1ErrorLine = tokens[pos - 1].rownumber;
            LL1ErrorMessage = "Program unexpected finish";
            break;
        }
        if (isTerminal(SymbolStack.top())) {
            if (SymbolStack.top() == tokens[pos].lex) {
                SymbolStack.pop();
                ++pos;
            }
            else {
                if (TreeRoot != nullptr)
                    delete TreeRoot;
                TreeRoot = nullptr;
                LL1ErrorLine = tokens[pos].rownumber;
                LL1ErrorMessage = QString("Suppose to get '%1', but '%2'").arg(QString::fromStdString(CodetoSymbol[SymbolStack.top()])).arg(QString::fromStdString(CodetoSymbol[tokens[pos].lex]));
                break;
            }
        }
        else {
            if (Table[SymbolStack.top()].count(tokens[pos].lex) != 0) {
                auto symbol = SymbolStack.top();
                SymbolStack.pop();
                CallProgram(Table[symbol][tokens[pos].lex]);
            }
            else {
                if (TreeRoot != nullptr)
                    delete TreeRoot;
                TreeRoot = nullptr;
                LL1ErrorLine = tokens[pos].rownumber;
                LL1ErrorMessage = "Suppose to get";
                for (auto &p : Table[SymbolStack.top()]) {
                    LL1ErrorMessage += " '";
                    LL1ErrorMessage += QString::fromStdString(CodetoSymbol[p.first]);
                    LL1ErrorMessage += "'";
                }
                LL1ErrorMessage += ", but '";
                LL1ErrorMessage += QString::fromStdString(CodetoSymbol[tokens[pos].lex]);
                LL1ErrorMessage += "' instead";
                break;
            }
        }
    }
    return TreeRoot;
}
