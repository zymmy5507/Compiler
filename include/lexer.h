

#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>


enum TokenType {
    TOK_INT,       
    TOK_IF,        
    TOK_ELSE,       
    TOK_ID,         
    TOK_NUM,       
    TOK_ASSIGN,    
    TOK_PLUS,       
    TOK_MINUS,      
    TOK_EQ,        
    TOK_LBRACE,     
    TOK_RBRACE,     
    TOK_LPAREN,    
    TOK_RPAREN,     
    TOK_SEMI,     
    TOK_EOF,        
    TOK_UNKNOWN     
};

struct Token {
    TokenType type;
    std::string value;
    
    Token() : type(TOK_UNKNOWN), value("") {}
    Token(TokenType t, std::string v) : type(t), value(v) {}
};


class Lexer {
private:
    std::string input;              
    size_t pos;                     
    std::vector<Token> tokens;      
    
  
    void skipWhitespace();          
    void readIdentifier();    
    void readNumber();              
    bool isAtEnd();                
    
public:
    Lexer(std::string code);        
    void tokenize();               
    void printTokens();             
    std::vector<Token>& getTokens() { return tokens; }  
};

#endif
