#include "generator.hpp"

ofstream outputFile;
vector<string> variableStack;
int globalOffsetForStack = 0;
int tempVariableCount = 1;
int labelCount = 1;
string currentLabel = "";

void driver(Node* &tree, string fileName) {
    createFile(fileName);

    addGlobals(tree->child1);

    generate(tree->child2, 0);

    outputFile << "STOP" << endl;

    for (int i = 0; i < globalOffsetForStack; i++){
        outputFile << variableStack.at(i) << " 0" << endl;
    }

    for (int i = 0; i < tempVariableCount; i++) {
        outputFile << "V" << i << " 0" << endl;
    }

    for (int i = 0; i < labelCount; i++) {
        outputFile << "L" << i << " 0" << endl;
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
        popLocals(localVars);
    } else {
        branchToNextNonTerminal(tree, variableCount);
    }
}

// talk <expr>
void generateOUT(Node* &tree, int variableCount) {
    generate(tree->child1, variableCount);

    outputFile << "STORE TEMP" << endl;
    outputFile << "WRITE TEMP" << endl;
}

// <N> + <expr> | <N>
void generateEXPRESSION(Node* &tree, int variableCount) {
    if (tree->tk1->token != "") {
        generate(tree->child2, variableCount);
        string tempVar = newName("<VARS>");

        outputFile << "STORE " << tempVar << endl;

        generate(tree->child1, variableCount);
        if (tree->tk1->tokenType == PLUStk) {
            outputFile << "ADD " << tempVar << endl;
        }
    } else {
        generate(tree->child1, variableCount);
    }
}

// <A> / <N> | <A> * <N> | <A>
void generateN(Node* &tree, int variableCount) {
    if (tree->tk1->token != "") {
        generate(tree->child2, variableCount);
        string tempVar = newName("<VARS>");

        outputFile << "STORE " << tempVar << endl;

        generate(tree->child1, variableCount);

        if (tree->tk1->tokenType == DIVtk) {
            outputFile << "DIV " << tempVar << endl;
        } else if (tree->tk1->tokenType == MULTtk) {
            outputFile << "MULT " << tempVar << endl;
        }
        
    } else {
        generate(tree->child1, variableCount);
    }

}

//<M> - <A> | <M>
void generateA(Node* &tree, int variableCount) {
    if (tree->tk1->token != "") {
        generate(tree->child2, variableCount);
        string tempVar = newName("<VARS>");

        outputFile << "STORE " << tempVar << endl;
        generate(tree->child1, variableCount);

        if (tree->tk1->tokenType == MINUStk) {
            outputFile << "SUB " << tempVar << endl;
        }
    } else {
        generate(tree->child1, variableCount);
    }
}

// .<M> | <R>
void generateM(Node* &tree, int variableCount) {
    generate(tree->child1, variableCount);

    if (tree->child1->tk1->tokenType == DOTtk) {
        outputFile << "MULT -1" << endl;
    }
}

// ( <expr> ) | Identifier | Integer
void generateR(Node* &tree, int variableCount) {
    if (tree->child1 == NULL) {
        cout << "Token is  " << tree->tk1->token << endl;
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

void generateIN(Node* &tree, int variableCount) {
    if (tree->child1 == NULL) {
        int isLocal = checkIfLocal(tree->tk1->token);
        if (isLocal != -1) {
            string tempVar = newName("<VARS>");
            outputFile << "READ " << tempVar << endl;
            outputFile << "LOAD " << tempVar << endl;
            outputFile << "STACKR " << isLocal << endl;
        } else {
            outputFile << "LOAD " << tree->tk1->token << endl;
        }
    } else {
        generate(tree->child1, variableCount);
    }
}

void generateLABEL(Node* &tree, int variableCount) {
    cout << "in label" << endl;
    string label = newName("<LABEL>");
    outputFile << label << ": NOOP" << endl;
    generate(tree->child1, variableCount);
}

void generateGOTO(Node* &tree, int variableCount) {
    string label = newName("<LABEL>");
    generate(tree->child1, variableCount);
    outputFile << "BR " << label << endl;
}

void generateASSIGN(Node* &tree, int variableCount) {
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

void popLocals(int variableCount) {
    for (int i = 0; i < variableCount; i++) {
        outputFile << "POP" << endl;
    }
}

string newName(string type) {
    if (type == "<VARS>") {
        string tempVariableString = "V" + to_string(tempVariableCount - 1);
        tempVariableCount++;
        return tempVariableString;
    } else {
        string tempVariableString = "L" + to_string(labelCount - 1);
        labelCount++;
        return tempVariableString;
    }
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
    else if (nodeType == "<IN>") { generateIN(tree, variableCount);}
    else {
        if (tree->child1 != NULL) generate(tree->child1, variableCount);
        if (tree->child2 != NULL) generate(tree->child2, variableCount);
        if (tree->child3 != NULL) generate(tree->child3, variableCount);
        if (tree->child4 != NULL) generate(tree->child4, variableCount);
        if (tree->child5 != NULL) generate(tree->child5, variableCount);
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
    cout << "printing stack with global offset " << globalOffsetForStack << endl;
    for (int i = variableStack.size(); i > globalOffsetForStack; i--)
    {
        cout << "on stack " << variableStack.at(i - 1) << endl;
    }

    for (int i = variableStack.size(); i > globalOffsetForStack; i--)
    {
        if (variableStack.at(i - 1) == passedVariable) {
            cout << "found local" << variableStack.at(i - 1) << endl;
            return variableStack.size() - i;
        } 
    }

    cout << "is a global " << passedVariable << endl;
    return -1;
}

string generateTempVariable() {
    string tempVariableString = "V" + to_string(tempVariableCount);
    tempVariableCount++;
    return tempVariableString;
}

string generateTempLabel() {
    string tempVariableString = "L" + to_string(labelCount);
    labelCount++;
    currentLabel = tempVariableString;
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