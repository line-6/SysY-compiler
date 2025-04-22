#include "AST.h"
#include <iostream>
#include <list>

static int temp_var_no = 0;
static std::list<int> temp_var_list;
static int NewTempVar() {
    temp_var_list.push_back(temp_var_no++);
    return temp_var_list.back();
}

// 所有 AST 的基类
void BaseAST::debugAST() const {
    std::cout << "this is BaseAST" << std::endl;
}

//===----------definition of CompUnitAST----------------===//
//===--------------------------------------------------===//
void CompUnitAST::DumpAST() const {
    std::cout << "CompUnitAST { " << std::endl;
    func_def->DumpAST();
    std::cout << "}" << std::endl;
}

void CompUnitAST::DumpIR() const {
    func_def->DumpIR();
}

void CompUnitAST::debugAST() const {
    std::cout << "CompUnitAST:" << std::endl;
    // std::cout << "CompUnitAST { " << std::endl;
    func_def->debugAST();
    // std::cout << "}";
}

//===----------definition of FuncDefAST----------------===//
//===--------------------------------------------------===//
void FuncDefAST::DumpAST() const {
    std::cout << "  FuncDefAST { " << std::endl;
    func_type->DumpAST();
    std::cout << "    FuncIdent { " << ident << " }, " << std::endl;
    block->DumpAST();
    std::cout << "  }," << std::endl;
}

void FuncDefAST::DumpIR() const {
    std::cout << "fun ";
    std::cout << "@" << ident << "(): ";
    func_type->DumpIR();
    block->DumpIR();
}

void FuncDefAST::debugAST() const {
    //std::cout << std::endl << "this is FuncDefAST" << std::endl;
    std::cout << "  FuncDefAST:" << std::endl;
    func_type->debugAST();
    // std::cout << "    FuncIdent { " << ident << " }, " << std::endl;
    block->debugAST();
    // std::cout << "  }," << std::endl;
}

//===----------definition of FuncTypeAST----------------===//
//===--------------------------------------------------===//
void FuncTypeAST::DumpAST() const {
    std::cout << "    FuncTypeAST { " << func_type << " }," << std::endl;
}

void FuncTypeAST::DumpIR() const {
    if (func_type == "int") {
      std::cout << "i32 ";
    }
}

void FuncTypeAST::debugAST() const {
    std::cout << "    FuncTypeAST:" << std::endl;
   // std::cout << "    FuncTypeAST { " << func_type << " }," << std::endl;
}

//===----------definition of BlockAST----------------===//
//===--------------------------------------------------===//
void BlockAST::DumpAST() const {
    std::cout << "    BlockAST { " << std::endl;
    stmt->DumpAST();
    std::cout << "    }," << std::endl;
}

void BlockAST::DumpIR() const {
    std::cout << "{" << std::endl;
    std::cout << "%entry:" << std::endl;
    //std::cout << "  ";
    stmt->DumpIR();
    std::cout << "}" << std::endl;
}

void BlockAST::debugAST() const {
    std::cout << "    BlockAST:" << std::endl;
    // std::cout << "    BlockAST { " << std::endl;
    stmt->debugAST();
    // std::cout << "    }," << std::endl;
}

//===----------definition of StmtAST----------------===//
//===--------------------------------------------------===//
void StmtAST::DumpAST() const {
    std::cout << "       StmtAST { " << std::endl;
    std::cout << "        ";
    exp->DumpAST();
    std::cout << std::endl;
    std::cout << "       }," << std::endl;
}

void StmtAST::DumpIR() const {
    exp->Eval();    // dump 前先 evaluate 每个节点
    exp->DumpIR();
    std::cout << "  ret " << exp->GetPc() << std::endl;
}

void StmtAST::debugAST() const {
    std::cout << "      StmtAST:" << std::endl;
    // std::cout << "       StmtAST { " << std::endl;
    // std::cout << "        ";
    exp->debugAST();
    // std::cout << std::endl;
    // std::cout << "       }," << std::endl;
}

//===----------definition of BaseExpAST----------------===//
//===--------------------------------------------------===//
std::string ExpBaseAST::GetPc() {
    if (is_number) return std::to_string(val);
    else {
        return "%" + std::to_string(addr);
    }
}

//===----------definition of ExpAST-------------------===//
// Exp ::= AddExp;
//===-------------------------------------------------===//
void ExpAST::DumpAST() const {
    // std::cout << "        ";
    add_exp->DumpAST();
  }

void ExpAST::DumpIR() const {
    add_exp->DumpIR();
}

void ExpAST::debugAST() const {
    std::cout << "        AddAST -> ExpAST" << std::endl;
    add_exp->debugAST();
}

void ExpAST::Eval() {
    add_exp->Eval();
    // whether is num depends on the son.
    is_number = add_exp->is_number;
    val = add_exp->val;
    addr = add_exp->addr;
}
//===----------definition of AddExpAST----------------===//
// addExp ::= mulExp | addExp ("+" | "-") mulExp;
//===-------------------------------------------------===//
void AddExpAST::DumpAST() const {
    if (add_exp) {
        add_exp->DumpAST();
        std::cout << op;
        mul_exp->DumpAST();
    }
    else {
        mul_exp->DumpAST();
    }
}

void AddExpAST::DumpIR() const {
    if (add_exp) {
        add_exp->DumpIR();
        mul_exp->DumpIR();
        if (op == "+") {
            std::cout << "  %" << addr << " = " << "add " << add_exp->GetPc() << ", " << mul_exp->GetPc()
                      << std::endl;
        }
        else if (op == "-") {
            std::cout << "  %" << addr << " = " << "sub " << add_exp->GetPc() << ", " << mul_exp->GetPc()
                      << std::endl;            
        }
    }
    else {
        mul_exp->DumpIR();
    }
}

void AddExpAST::debugAST() const {
    if (add_exp) {
        std::cout << "          AddAST op(" << op << ") MulAST" << "-> AddAST" << std::endl;
        add_exp->debugAST();
        //std::cout << op;
        mul_exp->debugAST();
    }
    else {
        std::cout << "          MulAST -> AddAST" << std::endl;
        mul_exp->debugAST();
    }
}

void AddExpAST::Eval() {
    // add := add op mul, absolutely is not number
    if (add_exp) {
        add_exp->Eval();
        mul_exp->Eval();
        is_number = false;
        addr = NewTempVar();
    }
    // add := mul
    else {
        mul_exp->Eval();
        is_number = mul_exp->is_number;
        val = mul_exp->val;
        addr = mul_exp->addr;
    }
}
//===----------definition of MulExpAST----------------===//
// mulExp ::= unaryExp | mulExp ("*" | "/" | "%") unaryExp;
//===-------------------------------------------------===//
void MulExpAST::DumpAST() const {
    if (mul_exp) {
        mul_exp->DumpAST();
        std::cout << op;
        unary_exp->DumpAST();
    }
    else {
        unary_exp->DumpAST();
    }
}

void MulExpAST::DumpIR() const {
    if (mul_exp) {
        mul_exp->DumpIR();
        unary_exp->DumpIR();
        if (op == "*") {
            std::cout << "  %" << addr << " = " << "mul " << mul_exp->GetPc() << ", " << unary_exp->GetPc()
                      << std::endl;
        }
        else if (op == "/") {
            std::cout << "  %" << addr << " = " << "div " << mul_exp->GetPc() << ", " << unary_exp->GetPc()
                      << std::endl;            
        }
        else if (op == "%") {
            std::cout << "  %" << addr << " = " << "mod " << mul_exp->GetPc() << ", " << unary_exp->GetPc()
                      << std::endl;            
        }        
    }
    else {
        unary_exp->DumpIR();
    }
}

void MulExpAST::debugAST() const {
    if (mul_exp) {
        std::cout << "          MulAST op(" << op << ") UnaryAST" << "-> MulAST" << std::endl;
        mul_exp->debugAST();
        //std::cout << op;
        unary_exp->debugAST();
    }
    else {
        std::cout << "          UnaryAST -> MulAST" << std::endl;
        unary_exp->debugAST();
    }
} 

void MulExpAST::Eval() {
    // mul := mul op unary
    if (mul_exp) {
        mul_exp->Eval();
        unary_exp->Eval();
        is_number = false;
        addr = NewTempVar();
    }
    // mul := unary
    else {
        unary_exp->Eval();
        is_number = unary_exp->is_number;
        val = unary_exp->val;
        addr = unary_exp->addr;
    }
}
//===----------definition of UnEaryExpAST----------------===//
// unaryExp ::= primaryExp | unaryOp unaryExp;
//===-------------------------------------------------===//
void UnaryExpAST::DumpAST() const {
    if (primary_exp) {
        primary_exp->DumpAST();
    }
    else {
        std::cout << unary_op;
        unary_exp->DumpAST();
    }
}

void UnaryExpAST::DumpIR() const {
    if (unary_exp) {
        unary_exp->DumpIR();
        if (unary_op == "+") {
            unary_exp->DumpIR();
        }
        else if (unary_op == "-") {
            unary_exp->DumpIR();
            std::cout << "  %" << addr << " = " << "sub 0, " << unary_exp->GetPc()
                      << std::endl;            
        }
        else if (unary_op == "!") {
            std::cout << "  %" << addr << " = " << "eq 0, " << unary_exp->GetPc()
                      << std::endl;
        }
    }
    else {
        primary_exp->DumpIR();
    }
}

void UnaryExpAST::debugAST() const {
    if (primary_exp) {
        std::cout << "          PrimaryAST -> UnaryAST" << std::endl;
        primary_exp->debugAST();
    }
    else {
        std::cout << "          unary_op(" << unary_op <<") UnaryAST -> MulAST" << std::endl;
        //std::cout << unary_op;
        unary_exp->debugAST();
    }
}

void UnaryExpAST::Eval() {
    // unary := op unary
    if (unary_exp) {
        unary_exp->Eval();
        is_number = false;
        addr = NewTempVar();
    }
    // unary := primary
    else {
        primary_exp->Eval();
        is_number = primary_exp->is_number;
        val = primary_exp->val;
        addr = primary_exp->addr;
    }
}
//===----------definition of PrimaryExpAST----------------===//
// primaryExp ::= "(" Exp ")" | Number;
//===-----------------------------------------------------===//
void PrimaryExpAST::DumpAST() const {
    if (exp) {
        std::cout << "(";
        exp->DumpAST();
        std::cout << ")";
    }
    else {
        std::cout << number;
    }
}

void PrimaryExpAST::DumpIR() const {
    if (exp) {
        exp->DumpIR();
    }    
}

void PrimaryExpAST::debugAST() const {
    if (exp) {
        std::cout << "          (ExpAST) -> PrimaryAST" << std::endl;
        //std::cout << "(";
        exp->debugAST();
        //std::cout << ")";
    }
    else {
        std::cout << "          number ("  << number << ")" << std::endl;
        //std::cout << number;
    }
}

void PrimaryExpAST::Eval() {
    // primary := exp
    if (exp) {
        exp->Eval();
        is_number = exp->is_number;
        val = exp->val;
        addr = exp->addr;
    }
    // primary := NUMBER
    else {
        is_number = true;
        val = number;
    }
}