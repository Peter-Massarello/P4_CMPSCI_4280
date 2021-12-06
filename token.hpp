 /*
Name: Peter Massarello
Instructor: Mark Hauschild
Assignment: P1
Date: September 20th 2021
Class: CMPSCI 4280
*/
#ifndef TOKEN_H
#define TOKEN_H

#include <vector>
#include <string>
using namespace std;

enum TokenType{
    STARTtk, STOPtk, LOOPtk, WHILEtk, FORtk, LABELtk, EXITtk, LISTENtk, TALKtk, PROGRAMtk, IFtk, THENtk, ASSIGNtk, DECLAREtk, JUMPtk, ELSEtk,
    DIGITtk, EQUALtk, DEQUALtk, LESSTHANtk, GREATERTHANtk, COLONtk, COLONEQtk, PLUStk, MINUStk, MULTtk, DIVtk, MODtk, DOTtk, LPARtk, RPARtk, COMMAtk,
    LBRACtk, RBRACtk, SEMItk, LBRACKtk, RBRACKtk, IDtk, EOFtk, NULLtk
};

// Struct for Token
struct Token {
    string token;
    TokenType tokenType;
    int lineNum;
    int charNum;

    Token(string tokenName, TokenType type, int line, int charNumber){
        token = tokenName;
        tokenType = type;
        lineNum = line;
        charNum = charNumber;
    };
};

#endif