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