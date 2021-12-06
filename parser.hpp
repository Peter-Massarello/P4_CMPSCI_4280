#ifndef PARSER_H
#define PARSER_H

#include "scanner.hpp"

Node* parse(vector<Token*> tokens);
Node* Program();
Node* Block();
Node* Vars();
Node* Expression();
Node* N();
Node* A();
Node* M();
Node* R();
Node* Stats();
Node* Mstat();
Node* Stat();
Node* In();
Node* Out();
Node* If();
Node* Loop();
Node* Assign();
Node* RO();
Node* Label();
Node* Goto();
void printAndIncrement();
void printRootLevelToStdOut(Node* &root, int level);
void printPreorder(Node* &root, int level);
Node* initNode();
void error(string grammarLevel, Token* tk);

#endif