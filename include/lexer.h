#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <iostream>

enum class TokenType {
    INT,
    IF,
    IDENTIFIER,
    NUMBER,
    ASSIGN,        // =
    PLUS,          // +
    MINUS,         // -
    EQUAL,         // ==
    LBRACE,        // {
    RBRACE,        // }
    LPAREN,        // (
    RPAREN,        // )
    SEMICOLON,     // ;
    UNKNOWN,
    EOF_TOKEN
};

struct Token {
    TokenType type;
    std::string text;
    
    Token(TokenType t, const std::string& txt) : type(t), text(txt) {}
};

class Lexer {
private:
    size_t pos;
    std::string code;
    std::vector<Token> tokens;
    
    void read_identifier();
    void read_number();
    void skip_whitespaces();
    bool is_end();
    
public:
    Lexer(const std::string &content);
    void tokenize();
    void print_tokens();
    std::vector<Token> get_tokens() { return tokens; }
};

#endif
