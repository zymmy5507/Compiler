

#include "lexer.h"
#include <iostream>
#include <cctype>


Lexer::Lexer(std::string code) {
    input = code;
    pos = 0;
    
}

void Lexer::tokenize() {
    while (!isAtEnd()) {
   
        if (std::isspace(input[pos])) {
            skipWhitespace();
        }
     
        else if (std::isalpha(input[pos])) {
            readIdentifier();  
        }
      
        else if (std::isdigit(input[pos])) {
            readNumber();
        }
    
        else {
            char c = input[pos];
            switch (c) {
                case '=':
                    
                    if (pos + 1 < input.length() && input[pos + 1] == '=') {
                        tokens.push_back(Token(TOK_EQ, "=="));
                        pos += 2;  
                    } else {
                        tokens.push_back(Token(TOK_ASSIGN, "="));
                        pos++;
                    }
                    break;
                case '+':
                    tokens.push_back(Token(TOK_PLUS, "+"));
                    pos++;
                    break;
                case '-':
                    tokens.push_back(Token(TOK_MINUS, "-"));
                    pos++;
                    break;
                case '{':
                    tokens.push_back(Token(TOK_LBRACE, "{"));
                    pos++;
                    break;
                case '}':
                    tokens.push_back(Token(TOK_RBRACE, "}"));
                    pos++;
                    break;
                case '(':
                    tokens.push_back(Token(TOK_LPAREN, "("));
                    pos++;
                    break;
                case ')':
                    tokens.push_back(Token(TOK_RPAREN, ")"));
                    pos++;
                    break;
                case ';':
                    tokens.push_back(Token(TOK_SEMI, ";"));
                    pos++;
                    break;
                default:
                 
                    tokens.push_back(Token(TOK_UNKNOWN, std::string(1, c)));
                    pos++;
                    break;
            }
        }
    }
  
    tokens.push_back(Token(TOK_EOF, ""));
}


void Lexer::readIdentifier() {
    std::string id = "";
   
    while (!isAtEnd() && (std::isalnum(input[pos]) || input[pos] == '_')) {
        id += input[pos];
        pos++;
    }
    
   
    if (id == "int") {
        tokens.push_back(Token(TOK_INT, id));
    } else if (id == "if") {
        tokens.push_back(Token(TOK_IF, id));
    } else if (id == "else") {  
        tokens.push_back(Token(TOK_ELSE, id));
    } else {
       
        tokens.push_back(Token(TOK_ID, id));
    }
}

void Lexer::readNumber() {
    std::string num = "";
    
    while (!isAtEnd() && std::isdigit(input[pos])) {
        num += input[pos];
        pos++;
    }
    tokens.push_back(Token(TOK_NUM, num));
}


void Lexer::skipWhitespace() {
    while (!isAtEnd() && std::isspace(input[pos])) {
        pos++;
    }
}


bool Lexer::isAtEnd() {
    return pos >= input.length();
}


void Lexer::printTokens() {
    for (size_t i = 0; i < tokens.size(); i++) {
        Token t = tokens[i];
        std::cout << "Token[" << i << "]: ";
        
       
        switch (t.type) {
            case TOK_INT: std::cout << "INT"; break;
            case TOK_IF: std::cout << "IF"; break;
            case TOK_ELSE: std::cout << "ELSE"; break; 
            case TOK_ID: std::cout << "ID"; break;
            case TOK_NUM: std::cout << "NUM"; break;
            case TOK_ASSIGN: std::cout << "ASSIGN"; break;
            case TOK_PLUS: std::cout << "PLUS"; break;
            case TOK_MINUS: std::cout << "MINUS"; break;
            case TOK_EQ: std::cout << "EQ"; break;
            case TOK_LBRACE: std::cout << "LBRACE"; break;
            case TOK_RBRACE: std::cout << "RBRACE"; break;
            case TOK_LPAREN: std::cout << "LPAREN"; break;
            case TOK_RPAREN: std::cout << "RPAREN"; break;
            case TOK_SEMI: std::cout << "SEMI"; break;
            case TOK_EOF: std::cout << "EOF"; break;
            default: std::cout << "UNKNOWN"; break;
        }
        
        
        if (t.value != "") {
            std::cout << " (" << t.value << ")";
        }
        std::cout << std::endl;
    }
}
