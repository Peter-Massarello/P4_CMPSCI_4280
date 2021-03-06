#include "scanner.hpp"
#include "parser.hpp"
#include "semantics.hpp"
#include "generator.hpp"

using namespace std;

int main(int argc, char** argv){
    vector<Token*> tokens;
    Node* tree;
    // Read from command line and determine path based on arg count
    if (argc == 1) { 
        tokens = createTokenVector();
        tree = parse(tokens);
        staticSemantics(tree);
        driver(tree, "kb");
    } else if (argc == 2) { 
        tokens = createTokenVector(argv[1]);
        tree = parse(tokens);
        staticSemantics(tree);
        driver(tree, argv[1]);
    } else cout << "ERROR: Too many args given, existing...\n";

    freeTokenVector(tokens);
    return 0;
}