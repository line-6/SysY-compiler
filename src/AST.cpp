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


void StmtAST::DumpAST() const {
    std::cout << "       StmtAST { " << std::endl;
    std::cout << "        ";
    exp->DumpAST();
    std::cout << std::endl;
    std::cout << "       }," << std::endl;
}

void StmtAST::DumpIR() const {
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
    std::cout << "      StmtAST:" << std::endl;
    // std::cout << "       StmtAST { " << std::endl;
    // std::cout << "        ";
    exp->debugAST();
    // std::cout << std::endl;
    // std::cout << "       }," << std::endl;
}


void ExpAST::DumpAST() const {
    // std::cout << "        ";
    addExp->DumpAST();
  }

void ExpAST::DumpIR() const {
    addExp->DumpIR();
}

void ExpAST::debugAST() const {
    std::cout << "        addAST -> ExpAST" << std::endl;
    addExp->debugAST();
}


void addExp_2_mulExp_AST::DumpAST() const {
    mulExp->DumpAST();
}

void addExp_2_mulExp_AST::DumpIR() const {
    mulExp->DumpIR();
}

void addExp_2_mulExp_AST::debugAST() const {
    std::cout << "        mulExp -> addExp" << std::endl;
    mulExp->debugAST();
}


void addExp_2_addExp_op_mulExp_AST::DumpAST() const {
    // std::cout << "        ";
    addExp->DumpAST();
    std::cout << op;
    mulExp->DumpAST();
}

void addExp_2_addExp_op_mulExp_AST::DumpIR() const {
    addExp->DumpIR();
    mulExp->DumpIR();
    // 两个立即数相加，先前没有其他指令执行，直接add
    if (koopa_inst_pc == 0) {
        int right = st_imdata.top();
        st_imdata.pop();
        int left = st_imdata.top();
        st_imdata.pop();
        std::cout << "  %" << koopa_inst_pc << " = add " << left << ", "  << right << std::endl;
        koopa_inst_pc ++;
    }
    // 问题来了，形如 1 * 2 + 3 * 4 的形式，如何获得 1 * 2 结果的指令pc和 3 * 4 结果的指令pc？
    // 潜在的解决方案：给每个 ast 类新加一个变量 inst_pc，记录当前被执行时的 pc ？仅在 $$ -> $1 op $2时记录pc
    // 好像不行，因为类在不停变换，pc值在真正要用的时候已经找不到其具体在那个类成员里了
}

void addExp_2_addExp_op_mulExp_AST::debugAST() const {
    std::cout << "        addExp op mulExp -> addExp" << std::endl;
    addExp->debugAST();
    mulExp->debugAST();
    //std::cout << "        addExp op mulExp -> addExp" << std::endl;
}


void mulExp_2_unaryExp_AST::DumpAST() const {
    unaryExp->DumpAST();
}

void mulExp_2_unaryExp_AST::DumpIR() const {
    unaryExp->DumpIR();
}

void mulExp_2_unaryExp_AST::debugAST() const {
    std::cout << "        unaryExp -> mulExp" << std::endl;
    unaryExp->debugAST();
} 


void mulExp_2_mulExp_op_unaryExp_AST::DumpAST() const {
   mulExp->DumpAST();
   std::cout << op;
   unaryExp->DumpAST();
}

void mulExp_2_mulExp_op_unaryExp_AST::DumpIR() const {
    mulExp->DumpIR();
    unaryExp->DumpIR();
    if (koopa_inst_pc == 0) {
        int right = st_imdata.top();
        st_imdata.pop();
        int left = st_imdata.top();
        st_imdata.pop();
        std::cout << "  %" << koopa_inst_pc << " = mul " << left << ", "  << right << std::endl;
        koopa_inst_pc ++;
    }
}

void mulExp_2_mulExp_op_unaryExp_AST::debugAST() const {
    std::cout << "        mulExp op unaryExp -> mulExp" << std::endl;
    mulExp->debugAST();
    unaryExp->debugAST();
}


void unaryExp_2_primaryExp_AST::DumpAST() const {
    primaryExp->DumpAST();
  }

void unaryExp_2_primaryExp_AST::DumpIR() const {
    primaryExp->DumpIR();
  }

void unaryExp_2_primaryExp_AST::debugAST() const {
    std::cout << "        primaryExp -> unaryExp" << std::endl;
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
    std::cout << "        unaryOp: " << unaryOp << " unaryExp -> unaryExp" << std::endl;
    //std::cout << unaryOp;
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
    std::cout << "        Exp -> primaryExp" << std::endl;
    exp->debugAST();
}


void primaryExp_2_num_AST::DumpAST() const {
    std::cout << number;
}

void primaryExp_2_num_AST::DumpIR() const {
    //std::cout << " num:" << number << " ";
    st_imdata.push(number);
}

void primaryExp_2_num_AST::debugAST() const {
    std::cout << "        num " << number << " -> primaryExp" << std::endl;
}