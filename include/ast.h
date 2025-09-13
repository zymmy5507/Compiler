// ast.h - Abstract Syntax Tree definitions with code generation support

#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <iostream>

// Forward declarations for visitor pattern
class ASTVisitor;
class Program;
class VarDecl;
class VarDeclAssign;
class AssignStmt;
class BinaryExpr;
class IfStmt;
class Identifier;
class NumberLiteral;

// ---------------- //
// Base AST Node    //
// ---------------- //
class ASTNode {
public:
    virtual ~ASTNode() {}
    virtual void accept(ASTVisitor* visitor) = 0;

    // Code generation methods
    virtual void gencode(std::ostream& out) = 0;
    virtual void gencodeL(std::ostream& out) = 0;
    virtual void gencodeR(std::ostream& out) = 0;
};

// ---------------- //
// Visitor Pattern  //
// ---------------- //
class ASTVisitor {
public:
    virtual void visit(Program* node) = 0;
    virtual void visit(VarDecl* node) = 0;
    virtual void visit(VarDeclAssign* node) = 0;
    virtual void visit(AssignStmt* node) = 0;
    virtual void visit(BinaryExpr* node) = 0;
    virtual void visit(IfStmt* node) = 0;
    virtual void visit(Identifier* node) = 0;
    virtual void visit(NumberLiteral* node) = 0;
};

// ---------------- //
// Program Node     //
// ---------------- //
class Program : public ASTNode {
public:
    std::vector<std::unique_ptr<ASTNode>> statements;

    Program() {}
    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    void addStatement(std::unique_ptr<ASTNode> stmt) {
        statements.push_back(std::move(stmt));
    }

    void gencode(std::ostream& out) override;
    void gencodeL(std::ostream& out) override {}
    void gencodeR(std::ostream& out) override {}
};

// ---------------- //
// Expressions      //
// ---------------- //
class Expression : public ASTNode {
public:
    virtual ~Expression() {}
};

// Identifier (variable reference)
class Identifier : public Expression {
public:
    std::string name;
    int loc;

    static std::unordered_map<std::string, int> mem_map;
    static int mem_loc;

    Identifier(std::string n);
    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    void gencode(std::ostream& out) override;
    void gencodeL(std::ostream& out) override;
    void gencodeR(std::ostream& out) override;
};

// Number literal
class NumberLiteral : public Expression {
public:
    int value;

    NumberLiteral(int v) : value(v) {}
    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    void gencode(std::ostream& out) override;
    void gencodeL(std::ostream& out) override;
    void gencodeR(std::ostream& out) override;
};

// Binary expression
class BinaryExpr : public Expression {
public:
    std::unique_ptr<Expression> left;
    std::string op;
    std::unique_ptr<Expression> right;

    BinaryExpr(std::unique_ptr<Expression> l, std::string o, std::unique_ptr<Expression> r)
        : left(std::move(l)), op(o), right(std::move(r)) {}

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    void gencode(std::ostream& out) override;
    void gencodeL(std::ostream& out) override {}
    void gencodeR(std::ostream& out) override {}
};

// ---------------- //
// Statements       //
// ---------------- //
class Statement : public ASTNode {
public:
    virtual ~Statement() {}
};

// VarDecl (int a;)
class VarDecl : public Statement {
public:
    std::string name;

    VarDecl(std::string n) : name(n) {}
    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    void gencode(std::ostream& out) override;
    void gencodeL(std::ostream& out) override {}
    void gencodeR(std::ostream& out) override {}
};

// VarDeclAssign (int a = expr;)
class VarDeclAssign : public Statement {
public:
    std::string name;
    std::unique_ptr<Expression> expr;

    VarDeclAssign(std::string n, std::unique_ptr<Expression> e)
        : name(n), expr(std::move(e)) {}
    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    void gencode(std::ostream& out) override;
    void gencodeL(std::ostream& out) override {}
    void gencodeR(std::ostream& out) override {}
};

// AssignStmt (a = expr;)
class AssignStmt : public Statement {
public:
    std::string varName;
    std::unique_ptr<Expression> expr;

    AssignStmt(std::string var, std::unique_ptr<Expression> e)
        : varName(var), expr(std::move(e)) {}
    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    void gencode(std::ostream& out) override;
    void gencodeL(std::ostream& out) override {}
    void gencodeR(std::ostream& out) override {}
};

// If statement
class IfStmt : public Statement {
public:
    std::unique_ptr<Expression> condition;
    std::vector<std::unique_ptr<ASTNode>> thenBody;
    std::vector<std::unique_ptr<ASTNode>> elseBody;

    IfStmt(std::unique_ptr<Expression> cond) : condition(std::move(cond)) {}
    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    void gencode(std::ostream& out) override;
    void gencodeL(std::ostream& out) override {}
    void gencodeR(std::ostream& out) override {}
};

// ---------------- //
// Print Visitor    //
// ---------------- //
class PrintVisitor : public ASTVisitor {
private:
    int indent;
    void printIndent();

public:
    PrintVisitor() : indent(0) {}

    void visit(Program* node) override;
    void visit(VarDecl* node) override;
    void visit(VarDeclAssign* node) override;
    void visit(AssignStmt* node) override;
    void visit(BinaryExpr* node) override;
    void visit(IfStmt* node) override;
    void visit(Identifier* node) override;
    void visit(NumberLiteral* node) override;
};

#endif

