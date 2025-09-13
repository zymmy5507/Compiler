#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include <iostream>
#include <fstream>
#include <sstream>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <source-file> <outfile.asm>\n";
        return 1;
    }

    // Read source file
    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << argv[1] << "\n";
        return 1;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string program = buffer.str();

    std::cout << "=== SimpleLang Compiler Test ===\n";
    std::cout << "Input program: " << program << "\n\n";

    // Step 1: Lexical analysis
    std::cout << "=== Lexer Output ===\n";
    Lexer lexer(program);
    lexer.tokenize();
    lexer.printTokens();

    // Step 2: Syntax analysis - now returns single Program node
    std::cout << "\n=== Parser Output ===\n";
    Parser parser(lexer.getTokens());
    auto programNode = parser.parse();

    std::cout << "Successfully parsed program with " 
              << programNode->statements.size() << " statements\n\n";

    // Step 3: AST visualization
    std::cout << "=== Abstract Syntax Tree ===\n";
    PrintVisitor printer;
    programNode->accept(&printer);

    std::string outfile = argv[2];
    std::ofstream out_f(outfile);
    out_f << ".text" << std::endl;
    // Step 4: Code generation
    std::cout << "\n=== Generated Code ===\n";
    programNode->gencode(out_f);
    out_f << "hlt" << std::endl;

    std::cout << "\nCompilation completed successfully!\n";
    return 0;
}

