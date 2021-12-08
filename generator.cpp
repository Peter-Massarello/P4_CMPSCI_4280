#include "generator.hpp"

ofstream outputFile;
vector<string> variableStack;
int globalOffsetForStack = 0;

void driver(Node* &tree, string fileName) {
    createFile(fileName);

    addGlobals(tree->child1);

    generate(tree->child2, 0);

    outputFile << "STOP" << endl;

    for (int i = 0; i < globalOffsetForStack; i++){
        outputFile << variableStack.at(i) << " 0" << endl;
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

void generateEXPRESSION(Node* &tree, int variableCount) {
    cout << "found Expr" << endl;
    generate(tree->child1, variableCount);

}

void generateN(Node* &tree, int variableCount) {
    cout << "found N" << endl;
    generate(tree->child1, variableCount);

}

void generateA(Node* &tree, int variableCount) {
    cout << "found A" << endl;
    generate(tree->child1, variableCount);
}

void generateM(Node* &tree, int variableCount) {
    cout << "found M" << endl;
    generate(tree->child1, variableCount);
}

void generateR(Node* &tree, int variableCount) {
    cout << "found r" << endl;
    generate(tree->child1, variableCount);
    outputFile << "LOAD " << tree->tk1->token << endl;
}

void generateLABEL(Node* &tree, int variableCount) {
    cout << "found label" << endl;

    outputFile << tree->tk1->token << ": NOOP" << endl;
}

void generateGOTO(Node* &tree, int variableCount) {
    cout << "found goto" << endl;

    outputFile << "BR " << tree->tk1->token << endl;
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
    
    if (nodeType == "<ASSIGN>") { cout << "Found assign" << endl;}
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
    for (int i = variableStack.size(); i >= globalOffsetForStack; i--)
    {
        if (variableStack.at(i) == passedVariable) {
            cout << "found local" << endl;
            return variableStack.size() - i;
        } else {
            cout << "is a global" << endl;
            return -1;
        }
    }

    return -1;  
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