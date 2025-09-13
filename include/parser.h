#define PARSER_H

#include "lexer.h"
#include "ast.h"
#include <memory>

class Parser {
private:
    std::vector<Token> tokens;      
    int current;                    
    
    
    bool isEnd();                   
    bool match(TokenType type);     
    void expect(TokenType type);    
    void advance();                 
    
    
    std::unique_ptr<ASTNode> parseStatement();       
    std::unique_ptr<VarDecl> parseVarDecl();    
    std::unique_ptr<VarDeclAssign> parseVarDeclAssign(); 
    std::unique_ptr<AssignStmt> parseAssignment();  
    std::unique_ptr<IfStmt> parseIfStmt();         
    
    
    std::unique_ptr<Expression> parseExpression();   
    std::unique_ptr<Expression> parseComparison();   
    std::unique_ptr<Expression> parseAddition();     
    std::unique_ptr<Expression> parsePrimary();      
    
public:
    Parser(std::vector<Token> toks);                
    std::unique_ptr<Program> parse(); 
};

#endif
