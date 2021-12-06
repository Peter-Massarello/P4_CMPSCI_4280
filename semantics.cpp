#include "semantics.hpp"
using namespace std;

vector<string> stack;
int globalOffset = 0;
int localVarsCount = 0;
bool inGlobal = true;

// Main driver function for the static semantics
void staticSemantics(Node* &tree) {
    cout << "Checking static semantics..." << endl;

    buildStack(tree);
    clearStack();

    cout << "Program clear!" << endl;
}

// clears entire stack for the end of the program
void clearStack() {
    for (int i = stack.size(); i > 0; i--) { pop(); }
}

// finds a variable inside the stack
int find(string token) {
    if (inGlobal){ // If inside global declarations make sure to check entire stack
        for (int i = stack.size()-1; i >= 0; i--){
            if (stack.at(i) == token) {
                return (stack.size()) - i;
            }
        }
    } else { // otherwise only check local scope for duplicates
        for (int i = stack.size()-1; i >= globalOffset + localVarsCount; i--){
            if (stack.at(i) == token) {
                return (stack.size()) - i;
            }
        }
        for (int i = globalOffset - 1; i >= 0; i--) { // if found in global declaration, means not declared in local scope
            if (stack.at(i) == token) {
                return NOT_FOUND;
            }
        }
    }
    return NOT_FOUND;
}

// checks for missing variable declaration in scope
bool checkMissing(string token) {
    for (int i = stack.size()-1; i >= 0; i--){
        if (stack.at(i) == token) { return true; }
    }
    return false;
}

// clears the stack up to the local level
void clearLocalStack() {
    for (int i = stack.size(); i >= globalOffset + localVarsCount; i--){
        pop();
    }
    localVarsCount = 0;
}

// gathers variables for stack inside of a certain block
void getVariablesInBlock(Node* &tree) {
    if (tree->tk1->tokenType != IDtk) return; // if not a idtk then return out of function
    if (tree->nodeType != "<VARS>") return;

    if (find(tree->tk1->token) == NOT_FOUND) { // if variable is not found in stack then add to top of stack
        push(tree->tk1->token);
        if (inGlobal) globalOffset++; else localVarsCount++;
    } else {
        doubleDeclarationError(tree->tk1->token);
    }

    if (tree->child1 != NULL) getVariablesInBlock(tree->child1); // go into next child
}

// Add a blocks variables to stack
void addBlockToStack(Node* &tree) {
    getVariablesInBlock(tree->child1); // gather all variables

    if (tree->child2 != NULL) buildStack(tree->child2); // go to next child

    clearLocalStack(); // clear local stack of block
}

// main function that builds the entire stack
void buildStack(Node* &tree) {
    if (tree->nodeType == "<PROGRAM>") { // if inside global scope
        getVariablesInBlock(tree->child1);
        inGlobal = false;
        buildStack(tree->child2);
    } else if (tree->nodeType == "<BLOCK>") { // if in local scope
        addBlockToStack(tree);
    } 
    else {
        string tk1 = tree->tk1->token;
        string tk2 = tree->tk2->token;
        string tk3 = tree->tk3->token;

        // find if a variable that is trying to be used is declared or not
        if (tree->tk1->tokenType == IDtk && checkMissing(tk1) == false) missingDeclarationError(tk1);
        if (tree->tk2->tokenType  == IDtk && checkMissing(tk2) == false) missingDeclarationError(tk2);
        if (tree->tk3->tokenType  == IDtk && checkMissing(tk3) == false) missingDeclarationError(tk3);

        //subtrees
        if (tree->child1 != NULL) buildStack(tree->child1);

        //subtrees
        if (tree->child2 != NULL) buildStack(tree->child2);

        //subtrees
        if (tree->child3 != NULL) buildStack(tree->child3);

        //subtrees
        if (tree->child4 != NULL) buildStack(tree->child4);

        //subtrees
        if (tree->child5 != NULL) buildStack(tree->child5);
    }
}

// Pop top of stack
void pop() {
    stack.pop_back();
}

// push variable to top of stack
void push(string variable) {
    stack.push_back(variable);
}

// Errors for double declare and missing declare
void doubleDeclarationError(string variable) {
    cout << "STATIC SEMANTICS ERROR: Double declaration of " << variable << endl;
    exit(0);
}

void missingDeclarationError(string variable) {
    cout << "STATIC SEMANTICS ERROR: Missing declaration of " << variable << endl;
    exit(0);
}
