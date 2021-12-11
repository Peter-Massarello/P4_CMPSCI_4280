#include "generator.hpp"

ofstream outputFile;
vector<string> variableStack;
int globalOffsetForStack = 0;
int tempVariableCount = 0;
int labelCount = 0;
string currentLabel = "";
string tempFileName = "";

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

    cout << ".asm file " << tempFileName << " created" << endl;

    outputFile.close();
}

void generate(Node* &tree, int variableCount){
    if (tree == NULL) return;
    if (tree->nodeType == "<BLOCK>") {
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
            outputFile << "READ " << tree->tk1->token << endl;
        }
    } else {
        generate(tree->child1, variableCount);
    }
}

void generateLABEL(Node* &tree, int variableCount) {
    outputFile << tree->tk1->token << ": NOOP" << endl;
}

void generateGOTO(Node* &tree, int variableCount) {
    outputFile << "BR " << tree->tk1->token << endl;
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

void generateIF(Node* &tree, int variableCount) {
    generate(tree->child3, variableCount);

    string tempVar = newName("<VARS>");
    outputFile << "STORE " << tempVar << endl;

    generate(tree->child1, variableCount);
    outputFile << "SUB " << tempVar << endl;

    string tempLabel = newName("<LABEL>");

    if (tree->child2->tk1->token == "{") {
        outputFile << "BRZERO " << tempLabel << endl;
    } else if (tree->child2->tk1->token == ">") {
        outputFile << "BRZNEG " << tempLabel << endl;
    } else if (tree->child2->tk1->token == "<") {
        outputFile << "BRZPOS " << tempLabel << endl;
    } else if (tree->child2->tk1->token == "==") {
        outputFile << "BRPOS " << tempLabel << endl;
        outputFile << "BRNEG " << tempLabel << endl;
    }

    generate(tree->child4, variableCount);
    cout << "printing " << tempLabel << endl;
    outputFile << tempLabel <<  ": NOOP" << endl;

    string tempLabel2 = newName("<LABEL>");
    outputFile << "BR " << tempLabel2 << endl;
    generate(tree->child5, variableCount);
    cout << "printing " << tempLabel2 << endl;
    outputFile << tempLabel2 <<  ": NOOP" << endl;
}


void generateWHILE(Node* &tree, int variableCount) {
    string tempLabel = newName("<LABEL>");
    outputFile << tempLabel << ": NOOP" << endl;
    generate(tree->child3, variableCount);

    string tempVar = newName("<VARS>");
    outputFile << "STORE " << tempVar << endl;

    generate(tree->child1, variableCount);
    outputFile << "SUB " << tempVar << endl;

    string tempLabel2 = newName("<LABEL>");

    if (tree->child2->tk1->token == "{") {
        outputFile << "BRZERO " << tempLabel2 << endl;
    } else if (tree->child2->tk1->token == ">") {
        outputFile << "BRZNEG " << tempLabel2 << endl;
    } else if (tree->child2->tk1->token == "<") {
        outputFile << "BRZPOS " << tempLabel2 << endl;
    } else if (tree->child2->tk1->token == "==") {
        outputFile << "BRPOS " << tempLabel2 << endl;
        outputFile << "BRNEG " << tempLabel2 << endl;
    }

    generate(tree->child4, variableCount);
    outputFile << "BR " << tempLabel << endl;
    outputFile << tempLabel2 <<  ": NOOP" << endl;
}


void pushLocalsToStack(Node* &tree, int &localVariableCount) {
    if(tree->tk1->token == "") return;

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
        string tempVariableString = "V" + to_string(tempVariableCount);
        tempVariableCount++;
        return tempVariableString;
    } else {
        string tempVariableString = "L" + to_string(labelCount);
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
    else if (nodeType == "<IF>") { generateIF(tree, variableCount);}
    else if (nodeType == "<LOOP>") { generateWHILE(tree, variableCount);}
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
    for (int i = variableStack.size(); i > globalOffsetForStack; i--)
    {
        if (variableStack.at(i - 1) == passedVariable) {
            return variableStack.size() - i;
        } 
    }

    return -1;
}

void createFile(string fileName){
    size_t pos = fileName.find('.');
    tempFileName = fileName.substr(0, pos) + ".asm";
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