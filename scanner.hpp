#ifndef SCAN_H
#define SCAN_H

#include "token.hpp"
#include "node.hpp"
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <map>
#include <set>
#include <regex>
#include <algorithm>

const int state = 23;
const int columns = 26;

enum states {s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14, s15, s16, s17, s18, s19, s20, s21, s22, s23, ERROR, FINAL, TBD, ID, DIGIT, EQUAL, DUBEQ, LESS, GREAT, COLON, COLEQ, PLUS, MINUS, MULT, DIVID, MODU, DECI, LPAR, RPAR, COMMA, LBRAC, RBRAC, SEMI, LBRAK, RBRAK};

extern map<char, int> columnMap;

extern states FSA_TABLE[state][columns];

extern set<string> keyWords;

vector<Token*> createTokenVector();

vector<Token*> createTokenVector(string fileName);

Token* generateToken(string tokenValue, TokenType type, int charCount, int lineCount);

void printTokens(vector<Token*> tokens);

void freeTokenVector(vector<Token*> tokens);

Token* scanner(vector<Token*> tokens, int tokenIndex);

TokenType findKeywordTkType(string tokenType);
#endif