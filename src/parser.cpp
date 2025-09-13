#include "parser.h"
#include <iostream>
#include <stdexcept>


Parser::Parser(std::vector<Token> toks) {
    tokens = toks;
    current = 0;
}


bool Parser::isEnd() {
    return current >= tokens.size() || tokens[current].type == TOK_EOF;
}

bool Parser::match(TokenType type) {
    if (isEnd()) return false;
    return tokens[current].type == type;
}

void Parser::expect(TokenType type) {
    if (!match(type)) {
        throw std::runtime_error("Expected token type but got different type");
    }
    advance();
}

void Parser::advance() {
    if (!isEnd()) {
        current++;
    }
}


std::unique_ptr<Program> Parser::parse() {
    auto program = std::make_unique<Program>();
    
  
    while (!isEnd()) {
        auto stmt = parseStatement();
        if (stmt) {
            program->addStatement(std::move(stmt));
        }
    }
    
    return program;
}

// now handles "int a = 4;"
std::unique_ptr<ASTNode> Parser::parseStatement() {
    if (match(TOK_INT)) {
       
        int savedPos = current;
        advance(); 
        
        if (match(TOK_ID)) {
            advance();
            if (match(TOK_ASSIGN)) {
                current = savedPos;
                return parseVarDeclAssign();
            } else {
                current = savedPos;
                return parseVarDecl();
            }
        } else {
            current = savedPos;
            return parseVarDecl();
        }
    }
    else if (match(TOK_IF)) {
        return parseIfStmt();
    }
    else if (match(TOK_ID)) {
        return parseAssignment();
    }
    else {
        advance(); 
        return nullptr;
    }
}

std::unique_ptr<VarDecl> Parser::parseVarDecl() {
    expect(TOK_INT);
    
    if (!match(TOK_ID)) {
        throw std::runtime_error("Expected identifier after 'int'");
    }
    
    std::string varName = tokens[current].value;
    advance();
    expect(TOK_SEMI);
    
    return std::make_unique<VarDecl>(varName);
}

std::unique_ptr<VarDeclAssign> Parser::parseVarDeclAssign() {
    expect(TOK_INT);
    
    if (!match(TOK_ID)) {
        throw std::runtime_error("Expected identifier after 'int'");
    }
    
    std::string varName = tokens[current].value;
    advance();
    expect(TOK_ASSIGN);
    
    auto expr = parseExpression();
    expect(TOK_SEMI);
    
    return std::make_unique<VarDeclAssign>(varName, std::move(expr));
}

std::unique_ptr<AssignStmt> Parser::parseAssignment() {
    std::string varName = tokens[current].value;
    advance();
    expect(TOK_ASSIGN);
    
    auto expr = parseExpression();
    expect(TOK_SEMI);
    
    return std::make_unique<AssignStmt>(varName, std::move(expr));
}

std::unique_ptr<IfStmt> Parser::parseIfStmt() {
    expect(TOK_IF);
    expect(TOK_LPAREN);
    
    auto condition = parseExpression();
    
    expect(TOK_RPAREN);
    expect(TOK_LBRACE);
    
    auto ifStmt = std::make_unique<IfStmt>(std::move(condition));
    
  
    while (!match(TOK_RBRACE) && !isEnd()) {
        auto stmt = parseStatement();
        if (stmt) {
            ifStmt->thenBody.push_back(std::move(stmt));
        }
    }
    
    expect(TOK_RBRACE);
    
   
    if (match(TOK_ELSE)) {
        advance();
        expect(TOK_LBRACE);
        
     
        while (!match(TOK_RBRACE) && !isEnd()) {
            auto stmt = parseStatement();
            if (stmt) {
                ifStmt->elseBody.push_back(std::move(stmt));
            }
        }
        
        expect(TOK_RBRACE);
    }
    
    return ifStmt;
}

std::unique_ptr<Expression> Parser::parseExpression() {
    return parseComparison();
}

std::unique_ptr<Expression> Parser::parseComparison() {
    auto expr = parseAddition();
    
    if (match(TOK_EQ)) {
        std::string op = tokens[current].value;
        advance();
        auto right = parseAddition();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    
    return expr;
}

std::unique_ptr<Expression> Parser::parseAddition() {
    auto expr = parsePrimary();
    
    while (match(TOK_PLUS) || match(TOK_MINUS)) {
        std::string op = tokens[current].value;
        advance();
        auto right = parsePrimary();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    
    return expr;
}

std::unique_ptr<Expression> Parser::parsePrimary() {
    if (match(TOK_NUM)) {
        int value = std::stoi(tokens[current].value);
        advance();
        return std::make_unique<NumberLiteral>(value);
    }
    else if (match(TOK_ID)) {
        std::string name = tokens[current].value;
        advance();
        return std::make_unique<Identifier>(name);
    }
    else {
        throw std::runtime_error("Expected number or identifier");
    }
}
