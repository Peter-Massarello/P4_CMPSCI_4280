#ifndef SEM_H
#define SEM_H
#include "token.hpp"
#include "node.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

const int NOT_FOUND = -1;

void staticSemantics(Node* &tree);

void buildStack(Node* &tree);

void checkLocalForDuplicates(string variable);

void checkGlobalForDuplicates(string variable);

void doubleDeclarationError(string variable);

void missingDeclarationError(string variable);

void addBlockToStack(Node* &tree);

void pop();

void push(string variable);

int find(string variable);

void clearStack();
#endif