#ifndef NODE_H
#define NODE_H
#include "token.hpp"

struct Node {
    string nodeType;

    Token *tk1;
    Token *tk2;
    Token *tk3;

    Node *child1;
    Node *child2;
    Node *child3;
    Node *child4;
    Node *child5;
};

#endif