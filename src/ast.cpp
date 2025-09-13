

#include "ast.h"
#include <iostream>


std::unordered_map<std::string, int> Identifier::mem_map;
int Identifier::mem_loc = 1;


void PrintVisitor::printIndent() {
    for (int i = 0; i < indent; i++) {
        std::cout << "  ";
    }
}


void PrintVisitor::visit(Program* node) {
    std::cout << "Program:" << std::endl;
    indent++;
    for (auto& stmt : node->statements) {
        stmt->accept(this);
    }
    indent--;
}

void PrintVisitor::visit(VarDecl* node) {
    printIndent();
    std::cout << "VarDecl: " << node->name << std::endl;
}

void PrintVisitor::visit(VarDeclAssign* node) {
    printIndent();
    std::cout << "VarDeclAssign: " << node->name << " = " << std::endl;
    indent++;
    node->expr->accept(this);
    indent--;
}

void PrintVisitor::visit(AssignStmt* node) {
    printIndent();
    std::cout << "Assignment: " << node->varName << " = " << std::endl;
    indent++;
    node->expr->accept(this);
    indent--;
}

void PrintVisitor::visit(BinaryExpr* node) {
    printIndent();
    std::cout << "BinaryExpr: " << node->op << std::endl;
    indent++;
    node->left->accept(this);
    node->right->accept(this);
    indent--;
}

void PrintVisitor::visit(IfStmt* node) {
    printIndent();
    std::cout << "IfStmt:" << std::endl;
    indent++;

    printIndent();
    std::cout << "Condition:" << std::endl;
    indent++;
    node->condition->accept(this);
    indent--;

    printIndent();
    std::cout << "Then Body:" << std::endl;
    indent++;
    for (auto& stmt : node->thenBody) stmt->accept(this);
    indent--;

    if (!node->elseBody.empty()) {
        printIndent();
        std::cout << "Else Body:" << std::endl;
        indent++;
        for (auto& stmt : node->elseBody) stmt->accept(this);
        indent--;
    }

    indent--;
}

void PrintVisitor::visit(Identifier* node) {
    printIndent();
    std::cout << "Identifier: " << node->name << std::endl;
}

void PrintVisitor::visit(NumberLiteral* node) {
    printIndent();
    std::cout << "Number: " << node->value << std::endl;
}

Identifier::Identifier(std::string n) : name(n) {
    if (mem_map.find(name) == mem_map.end()) {
        mem_map[name] = mem_loc++;
    }
    loc = mem_map[name];
}

void Program::gencode(std::ostream& out) {
    for (auto& stmt : statements) {
        stmt->gencode(out);
    }
}

void Identifier::gencode(std::ostream& out) {
    out << "mov M A " << mem_map[name] << std::endl;
}
void Identifier::gencodeL(std::ostream& out) {
    out << "mov A M " << mem_map[name] << std::endl;
}
void Identifier::gencodeR(std::ostream& out) {
    out << "mov B M " << mem_map[name] << std::endl;
}

void NumberLiteral::gencode(std::ostream& out) {
    out << "ldi A " << value << std::endl;
}
void NumberLiteral::gencodeL(std::ostream& out) {
    out << "ldi A " << value << std::endl;
}
void NumberLiteral::gencodeR(std::ostream& out) {
    out << "ldi B " << value << std::endl;
}

void BinaryExpr::gencode(std::ostream& out) {
    left->gencodeL(out);
    right->gencodeR(out);

    if (op == "==") {
        out << "cmp" << std::endl;
    } else if (op == "+") {
        out << "add" << std::endl;
    } else if (op == "-") {
        out << "sub" << std::endl;
    } else {
        throw std::runtime_error("Unsupported operator in gencode: " + op);
    }
}

void VarDecl::gencode(std::ostream& out) {
    if (Identifier::mem_map.find(name) == Identifier::mem_map.end()) {
        Identifier::mem_map[name] = Identifier::mem_loc++;
    }
  
}

void VarDeclAssign::gencode(std::ostream& out) {
    if (Identifier::mem_map.find(name) == Identifier::mem_map.end()) {
        Identifier::mem_map[name] = Identifier::mem_loc++;
    }
    expr->gencode(out);
    out << "mov M A " << Identifier::mem_map[name] << std::endl;
}

void AssignStmt::gencode(std::ostream& out) {
    expr->gencode(out);
    out << "mov M A " << Identifier::mem_map[varName] << std::endl;
}

void IfStmt::gencode(std::ostream& out) {
    static int labelCount = 0;
    int id = labelCount++;

    condition->gencode(out);
    out << "jnz %else_" << id << std::endl;

    for (auto& stmt : thenBody) stmt->gencode(out);
    out << "jmp %endif_" << id << std::endl;

    out << "else_" << id << ":" << std::endl;
    for (auto& stmt : elseBody) stmt->gencode(out);

    out << "endif_" << id << ":" << std::endl;
}

