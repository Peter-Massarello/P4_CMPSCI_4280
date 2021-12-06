#include "scanner.hpp"

using namespace std;

states FSA_TABLE[state][columns] = {
//        a-z     A-Z     0-9     _       $       =       <       >       :       +       -       *       /       %       .       (       )       ,       {       }       ;       [       ]      WS     EOF    ERROR   
/* s1  */ {s2,    ERROR,  s3,     ERROR,  s2,     s4,     s6,     s7,     s8,     s10,    s11,    s12,    s13,    s14,    s15,    s16,    s17,    s18,    s19,    s20,    s21,    s22,    s23,   s1,    FINAL,    ERROR},
/* s2  */ {s2,    s2,     s2,     s2,     ID,     ID,     ID,     ID,     ID,     ID,     ID,     ID,     ID,     ID,     ID,     ID,     ID,     ID,     ID,     ID,     ID,     ID,     ID,    ID,    ID,    ERROR},
/* s3  */ {DIGIT, DIGIT,  s3,     DIGIT,  DIGIT,  DIGIT,  DIGIT,  DIGIT,  DIGIT,  DIGIT,  DIGIT,  DIGIT,  DIGIT,  DIGIT,  DIGIT,  DIGIT,  DIGIT,  DIGIT,  DIGIT,  DIGIT,  DIGIT,  DIGIT,  DIGIT, DIGIT, DIGIT, ERROR},
/* s4  */ {EQUAL, EQUAL,  EQUAL,  EQUAL,  EQUAL,  s5,     EQUAL,  EQUAL,  EQUAL,  EQUAL,  EQUAL,  EQUAL,  EQUAL,  EQUAL,  EQUAL,  EQUAL,  EQUAL,  EQUAL,  EQUAL,  EQUAL,  EQUAL,  EQUAL,  EQUAL, EQUAL, EQUAL, ERROR},
/* s5  */ {DUBEQ, DUBEQ,  DUBEQ,  DUBEQ,  DUBEQ,  DUBEQ,  DUBEQ,  DUBEQ,  DUBEQ,  DUBEQ,  DUBEQ,  DUBEQ,  DUBEQ,  DUBEQ,  DUBEQ,  DUBEQ,  DUBEQ,  DUBEQ,  DUBEQ,  DUBEQ,  DUBEQ,  DUBEQ,  DUBEQ, DUBEQ, DUBEQ, ERROR},
/* s6  */ {LESS,  LESS,   LESS,   LESS,   LESS,   LESS,   LESS,   LESS,   LESS,   LESS,   LESS,   LESS,   LESS,   LESS,   LESS,   LESS,   LESS,   LESS,   LESS,   LESS,   LESS,   LESS,   LESS,  LESS,  LESS,  ERROR},
/* s7  */ {GREAT, GREAT,  GREAT,  GREAT,  GREAT,  GREAT,  GREAT,  GREAT,  GREAT,  GREAT,  GREAT,  GREAT,  GREAT,  GREAT,  GREAT,  GREAT,  GREAT,  GREAT,  GREAT,  GREAT,  GREAT,  GREAT,  GREAT, GREAT, GREAT, ERROR},
/* s8  */ {COLON, COLON,  COLON,  COLON,  COLON,  s9,     COLON,  COLON,  COLON,  COLON,  COLON,  COLON,  COLON,  COLON,  COLON,  COLON,  COLON,  COLON,  COLON,  COLON,  COLON,  COLON,  COLON, COLON, COLON, ERROR},
/* s9  */ {COLEQ, COLEQ,  COLEQ,  COLEQ,  COLEQ,  COLEQ,  COLEQ,  COLEQ,  COLEQ,  COLEQ,  COLEQ,  COLEQ,  COLEQ,  COLEQ,  COLEQ,  COLEQ,  COLEQ,  COLEQ,  COLEQ,  COLEQ,  COLEQ,  COLEQ,  COLEQ, COLEQ, COLEQ, ERROR},
/* s10 */ {PLUS,  PLUS,   PLUS,   PLUS,   PLUS,   PLUS,   PLUS,   PLUS,   PLUS,   PLUS,   PLUS,   PLUS,   PLUS,   PLUS,   PLUS,   PLUS,   PLUS,   PLUS,   PLUS,   PLUS,   PLUS,   PLUS,   PLUS,  PLUS,  PLUS,  ERROR},
/* s11 */ {MINUS, MINUS,  MINUS,  MINUS,  MINUS,  MINUS,  MINUS,  MINUS,  MINUS,  MINUS,  MINUS,  MINUS,  MINUS,  MINUS,  MINUS,  MINUS,  MINUS,  MINUS,  MINUS,  MINUS,  MINUS,  MINUS,  MINUS, MINUS, MINUS, ERROR},
/* s12 */ {MULT,  MULT,   MULT,   MULT,   MULT,   MULT,   MULT,   MULT,   MULT,   MULT,   MULT,   MULT,   MULT,   MULT,   MULT,   MULT,   MULT,   MULT,   MULT,   MULT,   MULT,   MULT,   MULT,  MULT,  MULT,  ERROR},
/* s13 */ {DIVID, DIVID,  DIVID,  DIVID,  DIVID,  DIVID,  DIVID,  DIVID,  DIVID,  DIVID,  DIVID,  DIVID,  DIVID,  DIVID,  DIVID,  DIVID,  DIVID,  DIVID,  DIVID,  DIVID,  DIVID,  DIVID,  DIVID, DIVID, DIVID, ERROR},
/* s14 */ {MODU,  MODU,   MODU,   MODU,   MODU,   MODU,   MODU,   MODU,   MODU,   MODU,   MODU,   MODU,   MODU,   MODU,   MODU,   MODU,   MODU,   MODU,   MODU,   MODU,   MODU,   MODU,   MODU,  MODU,  MODU,  ERROR},
/* s15 */ {DECI,  DECI,   DECI,   DECI,   DECI,   DECI,   DECI,   DECI,   DECI,   DECI,   DECI,   DECI,   DECI,   DECI,   DECI,   DECI,   DECI,   DECI,   DECI,   DECI,   DECI,   DECI,   DECI,  DECI,  DECI,  ERROR},
/* s16 */ {LPAR,  LPAR,   LPAR,   LPAR,   LPAR,   LPAR,   LPAR,   LPAR,   LPAR,   LPAR,   LPAR,   LPAR,   LPAR,   LPAR,   LPAR,   LPAR,   LPAR,   LPAR,   LPAR,   LPAR,   LPAR,   LPAR,   LPAR,  LPAR,  LPAR,  ERROR},
/* s17 */ {RPAR,  RPAR,   RPAR,   RPAR,   RPAR,   RPAR,   RPAR,   RPAR,   RPAR,   RPAR,   RPAR,   RPAR,   RPAR,   RPAR,   RPAR,   RPAR,   RPAR,   RPAR,   RPAR,   RPAR,   RPAR,   RPAR,   RPAR,  RPAR,  RPAR,  ERROR},
/* s18 */ {COMMA, COMMA,  COMMA,  COMMA,  COMMA,  COMMA,  COMMA,  COMMA,  COMMA,  COMMA,  COMMA,  COMMA,  COMMA,  COMMA,  COMMA,  COMMA,  COMMA,  COMMA,  COMMA,  COMMA,  COMMA,  COMMA,  COMMA, COMMA, COMMA, ERROR},
/* s19 */ {LBRAC, LBRAC,  LBRAC,  LBRAC,  LBRAC,  LBRAC,  LBRAC,  LBRAC,  LBRAC,  LBRAC,  LBRAC,  LBRAC,  LBRAC,  LBRAC,  LBRAC,  LBRAC,  LBRAC,  LBRAC,  LBRAC,  LBRAC,  LBRAC,  LBRAC,  LBRAC, LBRAC, LBRAC, ERROR},
/* s20 */ {RBRAC, RBRAC,  RBRAC,  RBRAC,  RBRAC,  RBRAC,  RBRAC,  RBRAC,  RBRAC,  RBRAC,  RBRAC,  RBRAC,  RBRAC,  RBRAC,  RBRAC,  RBRAC,  RBRAC,  RBRAC,  RBRAC,  RBRAC,  RBRAC,  RBRAC,  RBRAC, RBRAC, RBRAC, ERROR},
/* s21 */ {SEMI,  SEMI,   SEMI,   SEMI,   SEMI,   SEMI,   SEMI,   SEMI,   SEMI,   SEMI,   SEMI,   SEMI,   SEMI,   SEMI,   SEMI,   SEMI,   SEMI,   SEMI,   SEMI,   SEMI,   SEMI,   SEMI,   SEMI,  SEMI,  SEMI,  ERROR},
/* s22 */ {LBRAK, LBRAK,  LBRAK,  LBRAK,  LBRAK,  LBRAK,  LBRAK,  LBRAK,  LBRAK,  LBRAK,  LBRAK,  LBRAK,  LBRAK,  LBRAK,  LBRAK,  LBRAK,  LBRAK,  LBRAK,  LBRAK,  LBRAK,  LBRAK,  LBRAK,  LBRAK, LBRAK, LBRAK, ERROR},
/* s23 */ {RBRAK, RBRAK,  RBRAK,  RBRAK,  RBRAK,  RBRAK,  RBRAK,  RBRAK,  RBRAK,  RBRAK,  RBRAK,  RBRAK,  RBRAK,  RBRAK,  RBRAK,  RBRAK,  RBRAK,  RBRAK,  RBRAK,  RBRAK,  RBRAK,  RBRAK,  RBRAK, RBRAK, RBRAK, ERROR},
};

// Token list
vector<Token*> tokens;

// Map for characters to column numbers for FSA Table
map<char, int> columnMap = {
    {'a', 0},
    {'b', 0},
    {'c', 0},
    {'d', 0},
    {'e', 0},
    {'f', 0},
    {'g', 0},
    {'h', 0},
    {'i', 0},
    {'j', 0},
    {'k', 0},
    {'l', 0},
    {'m', 0},
    {'n', 0},
    {'o', 0},
    {'p', 0},
    {'q', 0},
    {'r', 0},
    {'s', 0},
    {'t', 0},
    {'u', 0},
    {'v', 0},
    {'w', 0},
    {'x', 0},
    {'y', 0},
    {'z', 0},
    {'A', 1},
    {'B', 1},
    {'C', 1},
    {'D', 1},
    {'E', 1},
    {'F', 1},
    {'G', 1},
    {'H', 1},
    {'I', 1},
    {'J', 1},
    {'K', 1},
    {'L', 1},
    {'M', 1},
    {'N', 1},
    {'O', 1},
    {'P', 1},
    {'Q', 1},
    {'R', 1},
    {'S', 1},
    {'T', 1},
    {'U', 1},
    {'V', 1},
    {'W', 1},
    {'X', 1},
    {'Y', 1},
    {'Z', 1},
    {'0', 2},
    {'1', 2},
    {'2', 2},
    {'3', 2},
    {'4', 2},
    {'5', 2},
    {'6', 2},
    {'7', 2},
    {'8', 2},
    {'9', 2},
    {'_', 3},
    {'$', 4},
    {'=', 5},
    {'<', 6},
    {'>', 7},
    {':', 8},
    {'+', 9},
    {'-', 10},
    {'*', 11},
    {'/', 12},
    {'%', 13},
    {'.', 14},
    {'(', 15},
    {')', 16},
    {',', 17},
    {'{', 18},
    {'}', 19},
    {';', 20},
    {'[', 21},
    {']', 22},
    {' ', 23},
    {EOF, 24},
    {'\0',24}
};

// Map of final state value to Token type
map<int, TokenType> tokenTypes = {
    {2, DIGITtk},
    {3, EQUALtk},
    {4, DEQUALtk},
    {5, LESSTHANtk},
    {6, GREATERTHANtk},
    {7, COLONtk},
    {8, COLONEQtk},
    {9, PLUStk},
    {10, MINUStk},
    {11, MULTtk},
    {12, DIVtk},
    {13, MODtk},
    {14, DOTtk},
    {15, LPARtk},
    {16, RPARtk},
    {17, COMMAtk},
    {18, LBRACtk},
    {19, RBRACtk},
    {20, SEMItk},
    {21, LBRACKtk},
    {22, RBRACKtk}
};

// Map of final state value to Token type
map<string, TokenType> keyWordTkTypes = {
    {"start", STARTtk},
    {"stop", STOPtk},
    {"loop", LOOPtk},
    {"while", WHILEtk},
    {"for", FORtk},
    {"label", LABELtk},
    {"exit", EXITtk},
    {"listen", LISTENtk},
    {"talk", TALKtk},
    {"program", PROGRAMtk},
    {"if", IFtk},
    {"then", THENtk},
    {"assign", ASSIGNtk},
    {"declare", DECLAREtk},
    {"jump", JUMPtk},
    {"else", ELSEtk}
};

// Set of keywords 
set<string> keyWords = {"start", "stop", "loop", "while", "for", "label", "exit", "listen", "talk", "program", "if", "then", "assign", "declare", "jump", "else"};

bool inComment = false;

// Data structure used to contain data needed for Tokens
struct DriverData{
    int currentLineCount;
    int currentCharCount;
    int charIndex;
    char nextChar;

    DriverData(int initLine, int initChar, int initIndex){
        currentLineCount = initLine;
        currentCharCount = initChar;
        charIndex = initIndex;
    }
};

// Get next characters from a line
char getNextChar(string line, DriverData* data){
    char nextChar = line[data->charIndex];

    if (nextChar == '\0'){
        return '\0';
    } else {
        data->charIndex += 1;
        return nextChar;
    }
}

// Get the next column from column map given a character.
int getNextIndex(char nextChar){
    if (nextChar == EOF){
        cout << "EOF FOUND" << endl;
    }
    // If char is there then return its value otherwise return the invalid char column
    if (columnMap.find(nextChar) != columnMap.end()){
        return columnMap[nextChar];
    } else {
        return 25;
    }
    
}

// Error call function which returns the line it was on and the character that created the error
void errorCall(DriverData* data, char nextChar){
    cout << "SCANNER ERROR: Scanner has encountered error at line " << data->currentLineCount << ", Error Character " << nextChar << endl;
    exit(0);
}

// Given token data from drive, generated ID token
Token* generateToken(string tokenValue, TokenType type, DriverData* data){
    Token* generatedToken = new Token("", DIGITtk, 0, 0);

    generatedToken->token = tokenValue;
    generatedToken->tokenType = type;
    generatedToken->charNum = data->currentCharCount;
    generatedToken->lineNum = data->currentLineCount;

    return generatedToken;
}

// Given token data from drive, generated ID token
Token* generateToken(string tokenValue, TokenType type, int charCount, int lineCount){
    Token* generatedToken = new Token("", DIGITtk, 0, 0);

    generatedToken->token = tokenValue;
    generatedToken->tokenType = type;
    generatedToken->charNum = charCount;
    generatedToken->lineNum = lineCount;

    return generatedToken;
}

TokenType findKeywordTkType(string tokenType){
    return keyWordTkTypes.at(tokenType);
}

// Main function called from driver which builds a vector of tokens from each line
vector<Token*> buildTokenFromLine(string line, DriverData* data){
    Token* currentToken;
    vector<Token*> tokenList;
    states currentState = s1; // initial state
    states nextState;
    string tokenValue = "";

    // Inits nextChar and nextIndex
    char nextChar = getNextChar(line, data);
    int nextIndex = getNextIndex(nextChar);
    nextState = FSA_TABLE[currentState][nextIndex];

    // While current state is not over final or not reached end of line
    while (currentState < FINAL || nextChar != '\0') {
        // If ERROR state is found, print error and exit
        if (nextState == ERROR)
            errorCall(data, nextChar);
        //  If FINAL state is found then run logic to create Token
        if (nextState > FINAL){
            // If nextState is ID, must figure out if ID is a KeyWord Token or not
            if (nextState == ID){
                if (keyWords.find(tokenValue) != keyWords.end()){
                    currentToken = generateToken(tokenValue, findKeywordTkType(tokenValue), data);

                    tokenList.push_back(currentToken);
                } else {
                    currentToken = generateToken(tokenValue, IDtk, data);

                    tokenList.push_back(currentToken);
                }
            } else { // If nextState is any other final state, generate Token with given data.
                currentToken = generateToken(tokenValue, tokenTypes.at(currentState), data);

                tokenList.push_back(currentToken);
            }
            // reset values for next Token
            currentState = s1;
            tokenValue = "";
            nextState = FSA_TABLE[currentState][nextIndex];
        }
        else {
            // Set currentState to nextState and set up data for next token
            currentState = nextState;
            if (nextChar != ' ') tokenValue.push_back(nextChar);
            nextChar = getNextChar(line, data);
            nextIndex = getNextIndex(nextChar);
            nextState = FSA_TABLE[currentState][nextIndex];
            data->currentCharCount += 1;
        }
    }

    return tokenList;
}

// Print out all token data
void printTokens(vector<Token*> tokens){
    for (int i = 0; i < tokens.size(); i++){
        cout << "Token " << i+1 << " {'" << tokens.at(i)->token << "', " << tokens.at(i)->tokenType << ", line # " << tokens.at(i)->lineNum << "}" << endl;
    }
}

// Frees all tokens
void freeTokenVector(vector<Token*> tokens){
    for (int i = 0; i < tokens.size(); i++){
        delete(tokens.at(i));
    }
}

// Main driver function which goes through each line taken from file
vector<Token*> driverFunction(vector<string> lines){
    vector<Token*> tempTokensList;
    DriverData* data = new DriverData(1,0,0);

    for (int i = 0; i < lines.size(); i++){
        tempTokensList = buildTokenFromLine(lines.at(i), data); // Builds a token vector from a line

        tokens.insert(tokens.end(), tempTokensList.begin(), tempTokensList.end()); // attaches created tokenList to the final token vector

        // Reset data object after each iteration
        data->charIndex = 0;
        data->currentCharCount = 0;
        data->currentLineCount += 1;
    }
    Token* eofToken = generateToken("", EOFtk, data);
    tokens.push_back(eofToken);

    delete data; // free data
    return tokens;
}

string cleanLine(string line, int commentCount) {
    if (inComment && commentCount == 2){ // Now need to be out of comment
        line.erase(line.begin(), line.begin() + line.find_first_of("&"));
        line.erase(remove(line.begin(), line.end(), '&'), line.end());
        inComment = false;
    } else if (commentCount == 4){ // if single line comment then remove
        line.erase(line.begin() + line.find_first_of("&"), line.begin() + line.find_last_of("&"));
        line.erase(remove(line.begin(), line.end(), '&'), line.end());
    } else if (commentCount == 2) { // if multiline begin comment
        inComment = true;
        line.erase(line.begin() + line.find_first_of("&"), line.end());
        line.erase(remove(line.begin(), line.end(), '&'), line.end());
    } 

    return line;
}

// Reads from keyboard and returns a vector of all strings given
vector<string> readFromKeyBoard(){
    string line;
    vector<string> wordList;
    while (getline(cin, line)) // read input until none are left in stdin
    {
        int commentCount = count(line.begin(), line.end(), '&');

        line = cleanLine(line, commentCount);
        
        if (!line.empty()) // if line wasnt fully erased then add
            wordList.push_back(line);
    }
    
    return wordList;
}

// Reads from keyboard and returns a vector of all strings given
vector<string> readFromFile(ifstream &file){
    string line;
    vector<string> wordList;
    while (getline(file, line)) // Read from file until no lines are left
    {   
        int commentCount = count(line.begin(), line.end(), '&');

        line = cleanLine(line, commentCount);

        if (!line.empty()) // if line wasnt fully erased then add
            wordList.push_back(line);
    }

    return wordList;
}

// Opens the file of a specfic filename and reads from that file to get the words in there
vector<string> openFile(string fileName){
    ifstream file(fileName);
    vector<string> wordList;

    // Tries to open file, catches error if cannot and exits
    try {
        if (file)
            wordList = readFromFile(file);
        else
            throw -1;
    } catch(int error){
        cout << "ERROR: File could not be found, existing...\n";
        exit(1);
        
    }
    
    file.close();
    return wordList;
}

// Creates tokenVector from keyboard input
vector<Token*> createTokenVector(){
    vector<Token*> tokens;
    vector<string> lines = readFromKeyBoard();

    tokens = driverFunction(lines);

    return tokens;
}

// Creates token vector from file input
vector<Token*> createTokenVector(string fileName){
    vector<Token*> tokens;
    vector<string> lines = openFile(fileName);

    tokens = driverFunction(lines);

    return tokens;
}

Token* scanner(vector<Token*> tokens, int tokenIndex){
    return tokens.at(tokenIndex);
}