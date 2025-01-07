#include "AST.h"
#include <iostream>

int koopa_inst_pc = 0;
std::stack<int> st_imdata;

// 所有 AST 的基类
void BaseAST::debugAST() const {
    std::cout << "this is BaseAST" << std::endl;
}


void CompUnitAST::DumpAST() const {
    std::cout << "CompUnitAST { " << std::endl;
    func_def->DumpAST();
    std::cout << "}";
}

void CompUnitAST::DumpIR() const {
    func_def->DumpIR();
}

void CompUnitAST::debugAST() const {
    std::cout << std::endl << "this is UnitAST" << std::endl;
    std::cout << "CompUnitAST { " << std::endl;
    func_def->debugAST();
    std::cout << "}";
}


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
    std::cout << std::endl << "this is FuncDefAST" << std::endl;
    std::cout << "  FuncDefAST { " << std::endl;
    func_type->debugAST();
    std::cout << "    FuncIdent { " << ident << " }, " << std::endl;
    block->debugAST();
    std::cout << "  }," << std::endl;
}


void FuncTypeAST::DumpAST() const {
    std::cout << "    FuncTypeAST { " << func_type << " }," << std::endl;
}

void FuncTypeAST::DumpIR() const {
    if (func_type == "int") {
      std::cout << "i32 ";
    }
}

void FuncTypeAST::debugAST() const {
    std::cout << std::endl << "this is FuncTypeAST" << std::endl;
    std::cout << "    FuncTypeAST { " << func_type << " }," << std::endl;
}


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
    std::cout << std::endl << "this is BlockAST" << std::endl;
    std::cout << "    BlockAST { " << std::endl;
    stmt->debugAST();
    std::cout << "    }," << std::endl;
}


void StmtAST::DumpAST() const {
    std::cout << "       StmtAST { " << std::endl;
    std::cout << "        ";
    exp->DumpAST();
    std::cout << std::endl;
    std::cout << "       }," << std::endl;
}

void StmtAST::DumpIR() const {
    // if (koopa_inst_pc == 0) {
    //   std::cout << "  ret 0" << std::endl;
    // }
    exp->DumpIR();
    if (koopa_inst_pc == 0) {
        std::cout << "  ret " << st_imdata.top() << std::endl;
        st_imdata.pop();
    }
    else {
        std::cout << "  ret %" << koopa_inst_pc - 1 << std::endl;
    }
    
    
}

void StmtAST::debugAST() const {
    std::cout << std::endl << "this is StmtAST" << std::endl;
    std::cout << "       StmtAST { " << std::endl;
    std::cout << "        ";
    exp->debugAST();
    std::cout << std::endl;
    std::cout << "       }," << std::endl;
}


void ExpAST::DumpAST() const {
    // std::cout << "        ";
    unaryExp->DumpAST();
  }

void ExpAST::DumpIR() const {
    unaryExp->DumpIR();
}

void ExpAST::debugAST() const {
    std::cout << std::endl << "*****debug: this is ExpAST*****" << std::endl;
    unaryExp->debugAST();
}


void unaryExp_2_primaryExp_AST::DumpAST() const {
    primaryExp->DumpAST();
  }

void unaryExp_2_primaryExp_AST::DumpIR() const {
    primaryExp->DumpIR();
  }

void unaryExp_2_primaryExp_AST::debugAST() const {
    std::cout << std::endl << "*****debug: this is unaryExp_2_primaryExp_AST*****" << std::endl;
    primaryExp->debugAST();
  }


void unaryExp_2_unaryOp_and_unaryExp_AST::DumpAST() const {
    std::cout << unaryOp;
    unaryExp->DumpAST();
}

void unaryExp_2_unaryOp_and_unaryExp_AST::DumpIR() const {
    if (unaryOp == "+") {
      // ignore '+'
      unaryExp->DumpIR();
    }
    else if (unaryOp == "-") {
      unaryExp->DumpIR();
      std::cout << "  %" << koopa_inst_pc << " = sub 0, ";
      if (koopa_inst_pc == 0) {
        std::cout << st_imdata.top() << std::endl;
        st_imdata.pop();
      } 
      else {
        std::cout << "%" << koopa_inst_pc - 1 << std::endl;
      }
      koopa_inst_pc ++;
    }
    else if (unaryOp == "!") {
      unaryExp->DumpIR();
      std::cout << "  %" << koopa_inst_pc << " = eq ";
      if (koopa_inst_pc == 0) {
        std::cout << st_imdata.top();
        st_imdata.pop();
        std::cout << ", 0" << std::endl;
      } 
      else {
        std::cout << "%" << koopa_inst_pc - 1 << ", 0" << std::endl;
      }
      koopa_inst_pc ++;
    }
    else {
      std::cout << "Unknown Operation!" << std::endl;
    }
}

void unaryExp_2_unaryOp_and_unaryExp_AST::debugAST() const {
    std::cout << std::endl << "*****debug: this is unaryExp_2_unaryOp_and_unaryExp_AST*****" << std::endl;
    std::cout << unaryOp;
    unaryExp->debugAST();
}


void primaryExp_2_Exp_AST::DumpAST() const {
    std::cout << "(";
    exp->DumpAST();
    std::cout << ")";
}

void primaryExp_2_Exp_AST::DumpIR() const {
    exp->DumpIR();
}

void primaryExp_2_Exp_AST::debugAST() const {
    std::cout << std::endl << "*****debug: this is primaryExp_2_Exp_AST*****" << std::endl;
    std::cout << "(";
    exp->debugAST();
    std::cout << ")";
}


void primaryExp_2_num_AST::DumpAST() const {
    std::cout << number;
}

void primaryExp_2_num_AST::DumpIR() const {
    //std::cout << " num:" << number << " ";
    st_imdata.push(number);
}

void primaryExp_2_num_AST::debugAST() const {
    std::cout << std::endl << "*****debug: primaryExp_2_num_AST*****" << std::endl;
    std::cout << number;
}