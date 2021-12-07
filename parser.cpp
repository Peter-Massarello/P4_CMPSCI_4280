#include "parser.hpp"
#include "scanner.hpp"
#include "node.hpp"

static Token* tk; // Lookahead placeholder
static Token* nullTk = new Token("", NULLtk, 0, 0); // empty tk used in init
vector<Token*> tokenList; // vector of tokens from scanner
static int tokenIndex = 0; // token index used for lookahead and printing

// map of token identifiers to strings for stdout
map<TokenType, string> identifierMap = {
    {DIGITtk,"Digit"},
    {EQUALtk,"Equal"},
    {DEQUALtk,"Double Equal"},
    {LESSTHANtk,"Less Than"},
    {GREATERTHANtk,"Greater Than"},
    {PLUStk,"Plus"},
    {MINUStk,"Minus"},
    {MULTtk,"Multiply"},
    {DIVtk,"Divide"},
    {MODtk,"Modulus"},
    {DOTtk,"Dot"},
    {LBRACtk,"Left Brace"},
    {RBRACtk, "Right Brace"},
    {IDtk,"Identifier"}
};

// Main parsing function
Node* parse(vector<Token*> tokens){
    tokenList = tokens;
    Node *root;

    tk = scanner(tokenList, tokenIndex);
    tokenIndex++;

    root = Program();

    tokenIndex = 0;
    printPreorder(root, 0);

    return root;
}

// gets net lookahead
void incrementLookahead(){
    tk = scanner(tokenList, tokenIndex);
    tokenIndex++;
}

// Inits a blank child node
Node* initNode(){
    Node *newNode = new Node();

    newNode->nodeType = "";

    newNode->tk1 = nullTk;
    newNode->tk2 = nullTk;
    newNode->tk3 = nullTk;

    newNode->child1 = NULL;
    newNode->child2 = NULL;
    newNode->child3 = NULL;
    newNode->child4 = NULL;
    newNode->child5 = NULL;

    return newNode;
}

// <vars> program <block>
Node* Program(){
    Node *child = initNode();
    child->nodeType = "<PROGRAM>";

    child->child1 = Vars();
    if (tk->tokenType == PROGRAMtk){
        incrementLookahead();
       child->child2 = Block();
    }else error("Program", tk);

    if (tk->tokenType == EOFtk){
    } else error("Program", tk);

    return child;
}

// start <vars> <stats> stop
Node* Block(){
    if (tk->tokenType == STARTtk){
        Node *child = initNode();
        child->nodeType = "<BLOCK>";

        incrementLookahead();

        child->child1 = Vars();
        child->child2 = Stats();

        if (tk->tokenType == STOPtk){
            incrementLookahead();
            return child;
        } else error("Block Stop", tk);
    } else error("Block Start", tk);

    return NULL;
}

// empty | declare Identifier = Integer ; <vars>
Node* Vars(){
    Node *child = initNode();
    child->nodeType = "<VARS>";

    if (tk->tokenType == DECLAREtk){
        incrementLookahead();

        if (tk->tokenType == IDtk){
            child->tk1 = tk;
            incrementLookahead();
            

            if (tk->tokenType == EQUALtk){
                incrementLookahead();

                if (tk->tokenType == DIGITtk) {
                    child->tk2 = tk;
                    incrementLookahead();

                    if (tk->tokenType == SEMItk){
                        incrementLookahead();

                        child->child1 = Vars();
                        return child;
                    }
                } else error("Vars", tk);
            } else error("Vars", tk);
        } else error("Vars", tk);
    }

    return child;
}

// <stat> <mStat>
Node* Stats(){
    Node *child = initNode();
    child->nodeType = "<STATS>";

    child->child1 = Stat();
    child->child2 = Mstat();

    return child;
}

// <in> ; | <out> ; | <block> | <if> ; | <loop> ; | <assign> ; | <goto> ; | <label> ;
Node* Stat(){
    Node *child = initNode();
    child->nodeType = "<STAT>";

    if (tk->tokenType == LISTENtk){
        child->child1 = In();

        if (tk->tokenType == SEMItk){
            incrementLookahead();

            return child;
        } else error("In Stat", tk);
    } else if (tk->tokenType == TALKtk){
        child->child1 = Out();

        if (tk->tokenType == SEMItk){
            incrementLookahead();

            return child;
        } else error("Out Stat", tk);
    } else if (tk->tokenType == STARTtk){
        child->child1 = Block();

        return child;
    } else if (tk->tokenType == IFtk){
        child->child1 = If();

        if (tk->tokenType == SEMItk){
            incrementLookahead();

            return child;
        } else error("If Stat", tk);
    } else if (tk->tokenType == WHILEtk){
        child->child1 = Loop();

        if (tk->tokenType == SEMItk){
            incrementLookahead();

            return child;
        } else error("Loop Stat", tk);
    } else if (tk->tokenType == ASSIGNtk){
        child->child1 = Assign();

        if (tk->tokenType == SEMItk){
            incrementLookahead();

            return child;
        } else error("Assign Stat", tk);
    } else if (tk->tokenType == LABELtk){
        child->child1 = Label();

        if (tk->tokenType == SEMItk){
            incrementLookahead();

            return child;
        } else error("Label Stat", tk);
    }  else if (tk->tokenType == JUMPtk){
        child->child1 = Goto();

        if (tk->tokenType == SEMItk){
            incrementLookahead();

            return child;
        } else error("Goto Stat", tk);
    } else {
        error("Else Stat", tk);
        return NULL;
    } 

    return NULL;
}

// empty | <stat> <mStat>
Node* Mstat(){
    Node *child = initNode();
    child->nodeType = "<MSTAT>";

    if (tk->tokenType == LISTENtk ||
        tk->tokenType == TALKtk || 
        tk->tokenType == IFtk || 
        tk->tokenType == WHILEtk || 
        tk->tokenType == ASSIGNtk || 
        tk->tokenType == LABELtk || 
        tk->tokenType == JUMPtk ||
        tk->tokenType == STARTtk)
    {
        child->child1 = Stat();
        child->child2 = Mstat();    
    }

    return child;
}

// listen Identfier 
Node* In(){
    Node *child = initNode();
    child->nodeType = "<IN>";

    incrementLookahead();

    if (tk->tokenType == IDtk){
        child->tk1 = tk;
        incrementLookahead();
        return child;

    } else {
        error("In", tk);
        return NULL;
    }
}

// talk <expr>
Node* Out(){
    Node *child = initNode();
    child->nodeType = "<OUT>";

    incrementLookahead();

    child->child1 = Expression();

    return child;
}

// if [ <expr> <RO> <expr>] then <stat> | if [ <expr> <RO> <expr>] then <stat> else <stat>
Node* If(){
    Node *child = initNode();
    child->nodeType = "<IF>";
    incrementLookahead();

    if (tk->tokenType == LBRACKtk){
        incrementLookahead();

        child->child1 = Expression();
        child->child2 = RO();
        child->child3 = Expression();

        if (tk->tokenType == RBRACKtk){
            incrementLookahead();

            if (tk->tokenType == THENtk){
                incrementLookahead();

                child->child4 = Stat();

                if (tk->tokenType == ELSEtk){
                    incrementLookahead();

                    child->child5 = Stat();

                    return child;
                }
            } else error("If", tk);
        }else error("If", tk);
    }else {
        error("If", tk);

        return NULL;
    }

    return child;
}

// while [ <expr> <RO> <expr>] <stat>
Node* Loop(){
    Node *child = initNode();
    child->nodeType = "<LOOP>";
    incrementLookahead();

    if (tk->tokenType == LBRACKtk){
        incrementLookahead();

        child->child1 = Expression();
        child->child2 = RO();
        child->child3 = Expression();

        if (tk->tokenType == RBRACKtk){
            incrementLookahead();

            child->child4 = Stat();

            return child;
        }else error("Loop", tk);
    }else {
        error("Loop", tk);

        return NULL;
    }

    return NULL;
}

// assign Identifier = <expr>
Node* Assign(){
    Node *child = initNode();
    child->nodeType = "<ASSIGN>";
    incrementLookahead();

    if (tk->tokenType == IDtk){
        child->tk1 = tk;
        incrementLookahead();

        if (tk->tokenType == EQUALtk) {
            incrementLookahead();

            child->child1 = Expression();

            return child;
        } else error("Assign", tk);
    } else {
        error("Assign", tk);

        return NULL;
    }

    return NULL;
}

// jump Identifier
Node* Goto(){
    Node *child = initNode();
    child->nodeType = "<GOTO>";
    incrementLookahead();

    if (tk->tokenType == IDtk){
        child->tk1 = tk;
        incrementLookahead();

        return child;
    } else {
        error("Goto", tk);

        return NULL;
    }
}

// label Identifier
Node* Label(){
    Node *child = initNode();
    child->nodeType = "<LABEL>";
    incrementLookahead();

    if (tk->tokenType == IDtk){
        child->tk1 = tk;
        incrementLookahead();

        return child;
    } else {
        error("Label", tk);

        return NULL;
    }
}

// > | < | == | {==} | %
Node* RO(){
    Node *child = initNode();
    child->nodeType = "<RO>";
    if (tk->tokenType == GREATERTHANtk){
        child->tk1 = tk;
        incrementLookahead();

        return child;
    } else if (tk->tokenType == LESSTHANtk){
        child->tk1 = tk;
        incrementLookahead();

        return child;
    } else if (tk->tokenType == DEQUALtk){
        child->tk1 = tk;
        incrementLookahead();

        return child;
    } else if (tk->tokenType == LBRACtk){
        child->tk1 = tk;
        incrementLookahead();

        if (tk->tokenType == DEQUALtk){
            child->tk2 = tk;
            incrementLookahead();

            if (tk->tokenType == RBRACtk){
                child->tk3 = tk;
                incrementLookahead();

                return child;
            } else error("RO", tk);
        } else error("RO", tk);
    } else if (tk->tokenType == MODtk){
        child->tk1 = tk;
        incrementLookahead();

        return child;
    } else {
        error("RO", tk);

        return NULL;
    }

    return NULL;
}

// <N> + <expr> | <N>
Node* Expression(){
    Node *child = initNode();
    child->nodeType = "<EXPRESSION>";

    child->child1 = N();
    if (tk->tokenType == PLUStk){
        child->tk1 = tk;
        incrementLookahead();

        child->child2 = Expression();

        return child;
    }

    return child;
}

// <A> / <N> | <A> * <N> | <A>
Node* N(){
    Node *child = initNode();
    child->nodeType = "<N>";

    child->child1 = A();

    if (tk->tokenType == DIVtk){
        child->tk1 = tk;
        incrementLookahead();

        child->child2 = N();

        return child;
    } else if (tk->tokenType == MULTtk){
        child->tk1 = tk;
        incrementLookahead();

        child->child2 = N();

        return child;
    }

    return child;
}

//<M> - <A> | <M>
Node* A(){
    Node *child = initNode();
    child->nodeType = "<A>";

    child->child1 = M();

    if (tk->tokenType == MINUStk){
        child->tk1 = tk;
        incrementLookahead();

        child->child2 = A();

        return child;
    }

    return child;
}

// .<M> | <R>
Node* M(){
    Node *child = initNode();
    child->nodeType = "<M>";

    if (tk->tokenType == DOTtk){
        child->tk1 = tk;
        incrementLookahead();

        child->child1 = M();

        return child;
    } else {
        child->child1 = R();

        return child;
    }

    return child;
}

// ( <expr> ) | Identifier | Integer
Node* R(){
    Node *child = initNode();
    child->nodeType = "<R>";

    if (tk->tokenType == LPARtk){
        incrementLookahead();

        child->child1 = Expression();

        if (tk->tokenType == RPARtk){
            incrementLookahead();

            return child;
        } else error("R", tk);
    } else if (tk->tokenType == IDtk){
        child->tk1 = tk;
        incrementLookahead();

        return child;
    } else if (tk->tokenType == DIGITtk){
        child->tk1 = tk;
        incrementLookahead();

        return child;
    } else {
        error("R", tk);

        return NULL;
    }

    return NULL;
}

// Prints current node level to file
void printRootLevelToStdOut(Node* &root, int level){
    cout << string(level*2, ' ') << root->nodeType << endl;

    if (root->tk1->tokenType != NULLtk){
        tokenIndex++;
        cout << string(level*2, ' ') << "{Token" << tokenIndex << ": " << identifierMap[root->tk1->tokenType] << ", " << root->tk1->token << ", Line #: " << root->tk1->lineNum << "}" << endl;
    }
        
    if (root->tk2->tokenType != NULLtk){
        tokenIndex++;
        cout << string(level*2, ' ') << "{Token" << tokenIndex << ": " << identifierMap[root->tk2->tokenType] << ", " << root->tk2->token << ", Line #: " << root->tk2->lineNum << "}" << endl;
    }
       

    if (root->tk3->tokenType != NULLtk){
        tokenIndex++;
        cout << string(level*2, ' ') << "{Token" << tokenIndex << ": " << identifierMap[root->tk3->tokenType] << ", " << root->tk2->token << ", Line #: " << root->tk2->lineNum << "}" << endl;
    }

}

// root left right
void printPreorder(Node* &root, int level){
    if (root == NULL) return;

    //root
    printRootLevelToStdOut(root, level);  

    //subtrees
    printPreorder(root->child1, level+1);  

    //subtrees
    printPreorder(root->child2, level+1);

    //subtrees
    printPreorder(root->child3, level+1);

    //subtrees
    printPreorder(root->child4, level+1);

    //subtrees
    printPreorder(root->child5, level+1);
}

//error printing function
void error(string grammarLevel, Token* tk){
    cout << "PARSER ERROR: Parser has encountered error at on grammar " << grammarLevel << ", token " << tk->token << endl;
    exit(0);
}