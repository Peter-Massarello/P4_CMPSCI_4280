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
        branchToNextNonTerminal(tree->child1, variableCount);
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
    
    if (nodeType == "<ASSIGN>") { cout << "Found assign" << endl;}
    else if (nodeType == "<EXPRESSION>") { cout << "Found expression" << endl;}
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