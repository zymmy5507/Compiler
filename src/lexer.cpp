#include "lexer.h"
#include <iostream>
#include <cctype>

Lexer::Lexer(const std::string &content) : pos(0), code(content) {
    std::cout << "Lexer initialized with: " << content << std::endl;
}

void Lexer::tokenize() {
    while (!is_end()) {
        // Skip whitespace
        if (std::isspace(code[pos])) {
            skip_whitespaces();
        }
        
        // Handle identifiers and keywords (int, if)
        if (std::isalpha(code[pos]) || code[pos] == '_') {
            read_identifier();
        }
        
        // Handle numbers
        if (std::isdigit(code[pos])) {
            read_number();
        }
        
        // Handle operators and punctuation
        switch (code[pos]) {
            case '=':
                if (pos + 1 < code.size() && code[pos + 1] == '=') {
                    tokens.push_back({TokenType::EQUAL, "=="});
                    pos += 2;
                } else {
                    tokens.push_back({TokenType::ASSIGN, "="});
                    pos++;
                }
                break;
            case '+':
                tokens.push_back({TokenType::PLUS, "+"});
                pos++;
                break;
            case '-':
                tokens.push_back({TokenType::MINUS, "-"});
                pos++;
                break;
            case '{':
                tokens.push_back({TokenType::LBRACE, "{"});
                pos++;
                break;
            case '}':
                tokens.push_back({TokenType::RBRACE, "}"});
                pos++;
                break;
            case '(':
                tokens.push_back({TokenType::LPAREN, "("});
                pos++;
                break;
            case ')':
                tokens.push_back({TokenType::RPAREN, ")"});
                pos++;
                break;
            case ';':
                tokens.push_back({TokenType::SEMICOLON, ";"});
                pos++;
                break;
            default:
                tokens.push_back({TokenType::UNKNOWN, std::string(1, code[pos])});
                pos++;
                break;
        }
    }
    
    tokens.push_back({TokenType::EOF_TOKEN, ""});
}

void Lexer::read_identifier() {
    std::string identifier;
    
    while (!is_end() && (std::isalnum(code[pos]) || code[pos] == '_')) {
        identifier += code[pos];
        pos++;
    }
    
    // check for keywords
    if (identifier == "int") {
        tokens.push_back({TokenType::INT, identifier});
    } else if (identifier == "if") {
        tokens.push_back({TokenType::IF, identifier});
    } else {
        tokens.push_back({TokenType::IDENTIFIER, identifier});
    }
}

void Lexer::read_number() {
    std::string number;
    
    while (!is_end() && std::isdigit(code[pos])) {
        number += code[pos];
        pos++;
    }
    
    tokens.push_back({TokenType::NUMBER, number});
}

void Lexer::skip_whitespaces() {
    while (!is_end() && std::isspace(code[pos])) {
        pos++;
    }
}

bool Lexer::is_end() {
    return pos >= code.size();
}

void Lexer::print_tokens() {
    std::cout << "\n=== TOKENS ===" << std::endl;
    for (const auto& token : tokens) {
        std::string type_name;
        switch (token.type) {
            case TokenType::INT: type_name = "INT"; break;
            case TokenType::IF: type_name = "IF"; break;
            case TokenType::IDENTIFIER: type_name = "IDENTIFIER"; break;
            case TokenType::NUMBER: type_name = "NUMBER"; break;
            case TokenType::ASSIGN: type_name = "ASSIGN"; break;
            case TokenType::PLUS: type_name = "PLUS"; break;
            case TokenType::MINUS: type_name = "MINUS"; break;
            case TokenType::EQUAL: type_name = "EQUAL"; break;
            case TokenType::LBRACE: type_name = "LBRACE"; break;
            case TokenType::RBRACE: type_name = "RBRACE"; break;
            case TokenType::LPAREN: type_name = "LPAREN"; break;
            case TokenType::RPAREN: type_name = "RPAREN"; break;
            case TokenType::SEMICOLON: type_name = "SEMICOLON"; break;
            case TokenType::UNKNOWN: type_name = "UNKNOWN"; break;
            case TokenType::EOF_TOKEN: type_name = "EOF"; break;
        }
        std::cout << type_name << ": '" << token.text << "'" << std::endl;
    }
}
