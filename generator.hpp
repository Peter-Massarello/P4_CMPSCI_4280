#include "semantics.hpp"
#include <fstream>

const string EXTENSTION = ".asm";

void generate(Node* &tree, int variableCount);

void driver(Node* &tree, string fileName);

void createFile(string fileName);

void addGlobals(Node* &tree);

void branchToNextNonTerminal(Node* &tree, int variableCount);

void pushLocalsToStack(Node* &tree, int &localVarCount);

void generateASSIGN(Node* &tree, int variableCount);

void generateOUT(Node* &tree, int variableCount);

void generateEXPRESSION(Node* &tree, int variableCount);

void generateN(Node* &tree, int variableCount);

void generateA(Node* &tree, int variableCount);

void generateM(Node* &tree, int variableCount);

void generateR(Node* &tree, int variableCount);

void generateLABEL(Node* &tree, int variableCount);

void generateGOTO(Node* &tree, int variableCount);

void generateIN(Node* &tree, int vairableCount);

int checkIfLocal(string passedVariable);

string generateTempVariable();

string generateTempLabel();

string newName(string type);

void popLocals(int variableCount);

