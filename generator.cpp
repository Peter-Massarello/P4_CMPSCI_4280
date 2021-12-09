#include "generator.hpp"

ofstream outputFile;
vector<string> variableStack;
int globalOffsetForStack = 0;
int tempVariableCount = 0;

void driver(Node* &tree, string fileName) {
    createFile(fileName);

    addGlobals(tree->child1);

    generate(tree->child2, 0);

    outputFile << "STOP" << endl;

    for (int i = 0; i < globalOffsetForStack; i++){
        outputFile << variableStack.at(i) << " 0" << endl;
    }

    for (int i = 0; i < tempVariableCount; i++) {
        outputFile << "TV" << i << " 0" << endl;
    }
    outputFile << "TEMP 0" << endl;

    outputFile.close();
}

void generate(Node* &tree, int variableCount){
    if (tree == NULL) return;
    if (tree->nodeType == "<BLOCK>") {
        cout << "found block" << endl;
        int localVars = 0;
        pushLocalsToStack(tree->child1, localVars);
        generate(tree->child2, localVars);
    } else {
        branchToNextNonTerminal(tree, variableCount);
    }
}


void generateOUT(Node* &tree, int variableCount) {
    cout << "found out" << endl;
    generate(tree->child1, variableCount);

    outputFile << "STORE TEMP" << endl;
    outputFile << "WRITE TEMP" << endl;
}

// <N> + <expr> | <N>
void generateEXPRESSION(Node* &tree, int variableCount) {
    cout << "found Expr" << endl;
    if (tree->tk1->token != "") {
        cout << "found tk in expr" << endl;
        generate(tree->child2, variableCount);
        string tempVar = generateTempVariable();

        outputFile << "STORE " << tempVar << endl;

        generate(tree->child1, variableCount);
        if (tree->tk1->tokenType == PLUStk) {
            outputFile << "ADD " << tempVar << endl;
        }
    } else {
        cout << "did not find tk in expr" << endl;
        generate(tree->child1, variableCount);
    }
}

// <A> / <N> | <A> * <N> | <A>
void generateN(Node* &tree, int variableCount) {
    cout << "found N" << endl;
    if (tree->tk1->token != "") {
        cout << "found tk in N" << endl;
        generate(tree->child2, variableCount);
        string tempVar = generateTempVariable();

        outputFile << "STORE " << tempVar << endl;

        generate(tree->child1, variableCount);

        if (tree->tk1->tokenType == DIVtk) {
            outputFile << "DIV " << tempVar << endl;
        } else if (tree->tk1->tokenType == MULTtk) {
            outputFile << "MULT " << tempVar << endl;
        }
        
    } else {
        cout << "did not find tk in N" << endl;
        generate(tree->child1, variableCount);
    }

}

//<M> - <A> | <M>
void generateA(Node* &tree, int variableCount) {
    cout << "found A" << endl;
    if (tree->tk1->token != "") {
        cout << "found tk in A" << endl;
        generate(tree->child2, variableCount);
        string tempVar = generateTempVariable();

        outputFile << "STORE " << tempVar << endl;
        generate(tree->child1, variableCount);

        if (tree->tk1->tokenType == MINUStk) {
            outputFile << "SUB " << tempVar << endl;
        }
    } else {
        cout << "did not find tk in A" << endl;
        generate(tree->child1, variableCount);
    }
}

// .<M> | <R>
void generateM(Node* &tree, int variableCount) {
    cout << "found M" << endl;
    generate(tree->child1, variableCount);

    if (tree->child1->tk1->tokenType == DOTtk) {
        outputFile << "MULT -1" << endl;
    }
}

// ( <expr> ) | Identifier | Integer
void generateR(Node* &tree, int variableCount) {
    cout << "found r" << endl;
    if (tree->child1 == NULL) {
        int isLocal = checkIfLocal(tree->tk1->token);
        if (isLocal != -1) {
            outputFile << "STACKR " << isLocal << endl;
        } else {
            outputFile << "LOAD " << tree->tk1->token << endl;
        }
    } else {
        generate(tree->child1, variableCount);
    }
}

void generateLABEL(Node* &tree, int variableCount) {
    cout << "found label" << endl;

    outputFile << tree->tk1->token << ": NOOP" << endl;
}

void generateGOTO(Node* &tree, int variableCount) {
    cout << "found goto" << endl;

    outputFile << "BR " << tree->tk1->token << endl;
}

void generateASSIGN(Node* &tree, int variableCount) {
    cout << "found assign" << endl;
    generate(tree->child1, variableCount);

    int isLocal = checkIfLocal(tree->tk1->token);
    if (isLocal != -1) {
        outputFile << "STACKW " << isLocal << endl;
    } else {
        outputFile << "STORE " << tree->tk1->token << endl;
    }
}


void pushLocalsToStack(Node* &tree, int &localVariableCount) {
    if(tree->tk1->token == "") return;

    cout << "PUSHING " << tree->tk1->token << endl;
    variableStack.push_back(tree->tk1->token);
    localVariableCount++;

    outputFile << "LOAD " << tree->tk2->token << endl;
    outputFile << "PUSH" << endl;
    outputFile << "STACKW 0" << endl;

    if (tree->child1 != NULL) pushLocalsToStack(tree->child1, localVariableCount);
}

void branchToNextNonTerminal(Node* &tree, int variableCount) {
    string nodeType = tree->nodeType;
    
    if (nodeType == "<ASSIGN>") { generateASSIGN(tree, variableCount);}
    else if (nodeType == "<OUT>") { generateOUT(tree, variableCount);}
    else if (nodeType == "<EXPRESSION>") { generateEXPRESSION(tree, variableCount);}
    else if (nodeType == "<N>") { generateN(tree, variableCount);}
    else if (nodeType == "<A>") { generateA(tree, variableCount);}
    else if (nodeType == "<M>") { generateM(tree, variableCount);}
    else if (nodeType == "<R>") { generateR(tree, variableCount);}
    else if (nodeType == "<LABEL>") { generateLABEL(tree, variableCount);}
    else if (nodeType == "<GOTO>") { generateGOTO(tree, variableCount);}
    else {
        if (tree->child1 != NULL) branchToNextNonTerminal(tree->child1, variableCount);
        if (tree->child2 != NULL) branchToNextNonTerminal(tree->child2, variableCount);
        if (tree->child3 != NULL) branchToNextNonTerminal(tree->child3, variableCount);
        if (tree->child4 != NULL) branchToNextNonTerminal(tree->child4, variableCount);
        if (tree->child5 != NULL) branchToNextNonTerminal(tree->child5, variableCount);
    }

}

void addGlobals(Node* &tree) {
    if (tree->tk1->token != ""){
        outputFile << "LOAD " << tree->tk2->token << endl;
        outputFile << "STORE " << tree->tk1->token << endl;
        variableStack.push_back(tree->tk1->token);
        globalOffsetForStack++;
    }

    if (tree->child1 != NULL) addGlobals(tree->child1);
}

int checkIfLocal(string passedVariable) {
    for (int i = variableStack.size(); i > globalOffsetForStack; i--)
    {
        if (variableStack.at(i - 1) == passedVariable) {
            cout << "found local" << variableStack.at(i - 1) << endl;
            return variableStack.size() - i;
        } else {
            cout << "is a global" << endl;
            return -1;
        }
    }

    return -1;  
}

string generateTempVariable() {
    string tempVariableString = "TV" + to_string(tempVariableCount);
    tempVariableCount++;
    return tempVariableString;
}

void createFile(string fileName){
    size_t pos = fileName.find('.');
    string tempFileName = fileName.substr(0, pos) + ".asm";
    outputFile.open(tempFileName);

    // Tries to open file, catches error if cannot and exits
    try {
        if (outputFile)
            return;
        else
            throw -1;
    } catch(int error){
        cout << "ERROR: File could not be created, existing...\n";
        exit(1);
    }
}