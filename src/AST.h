// CompUnit  ::= FuncDef;

// FuncDef   ::= FuncType IDENT "(" ")" Block;
// FuncType  ::= "int";

// Block     ::= "{" Stmt "}";
// Stmt      ::= "return" Number ";";
// Number    ::= INT_CONST;

// ---------------------------------------------

// Stmt        ::= "return" Exp ";";

// Exp         ::= UnaryExp;
// PrimaryExp  ::= "(" Exp ")" | Number;
// Number      ::= INT_CONST;
// UnaryExp    ::= PrimaryExp | UnaryOp UnaryExp;
// UnaryOp     ::= "+" | "-" | "!";

// ---------------------------------------------
// Exp         ::= AddExp;
// AddExp      ::= MulExp | AddExp ("+" | "-") MulExp;
// MulExp      ::= UnaryExp | MulExp ("*" | "/" | "%") UnaryExp;
// UnaryExp    ::= PrimaryExp | UnaryOp UnaryExp;
// PrimaryExp  ::= "(" Exp ")" | Number;
// Number      ::= INT_CONST;
// UnaryOp     ::= "+" | "-" | "!";


// 有一个问题，怎么较好地debug AST？（较好地打印AST信息）

#pragma once
#include <memory>
#include <string>
#include <stack>

extern int koopa_inst_num;
extern std::stack<int> st_imdata;

// 所有 AST 的基类
class BaseAST {
 public:
  virtual ~BaseAST() = default;
  virtual void DumpAST() const = 0;
  virtual void DumpIR() const = 0;

  virtual void debugAST() const;
};

class CompUnitAST : public BaseAST {
 public:
  std::unique_ptr<BaseAST> func_def;

  void DumpAST() const override;

  void DumpIR() const override;

  void debugAST() const override;
};

class FuncDefAST : public BaseAST {
 public:
  std::unique_ptr<BaseAST> func_type;
  std::string ident;
  std::unique_ptr<BaseAST> block;

  void DumpAST() const override;
  void DumpIR() const override;

  void debugAST() const override;
};

class FuncTypeAST : public BaseAST {
 public:
  std::string func_type;
  
  void DumpAST() const override;
  void DumpIR() const override;

  void debugAST() const override;
};

class BlockAST : public BaseAST {
 public:
  std::unique_ptr<BaseAST> stmt;

  void DumpAST() const override;
  void DumpIR() const override;

  void debugAST() const override;
};

class StmtAST : public BaseAST {
  public:
  // int number;
  std::unique_ptr<BaseAST> exp;

  void DumpAST() const override;
  void DumpIR() const override;

  void debugAST() const override;
};

class ExpAST : public BaseAST {
  public:
  std::unique_ptr<BaseAST> addExp;

  void DumpAST() const override;

  void DumpIR() const override;

  void debugAST() const override;
};

class addExp_2_mulExp_AST : public BaseAST {
  public:
    std::unique_ptr<BaseAST> mulExp;

    void DumpAST() const override;

    void DumpIR() const override;

    void debugAST() const override;
};

class addExp_2_addExp_op_mulExp_AST : public BaseAST {
  public:
    std::unique_ptr<BaseAST> addExp;
    std::string op;
    std::unique_ptr<BaseAST> mulExp;

    void DumpAST() const override;

    void DumpIR() const override;

    void debugAST() const override;
};

class mulExp_2_unaryExp_AST : public BaseAST {
  public:
    std::unique_ptr<BaseAST> unaryExp;

    void DumpAST() const override;

    void DumpIR() const override;

    void debugAST() const override;
};

class mulExp_2_mulExp_op_unaryExp_AST : public BaseAST {
  public:
    std::unique_ptr<BaseAST> mulExp;
    std::string op;
    std::unique_ptr<BaseAST> unaryExp;

    void DumpAST() const override;

    void DumpIR() const override;

    void debugAST() const override;
};

class unaryExp_2_primaryExp_AST : public BaseAST {
  public:
    std::unique_ptr<BaseAST> primaryExp;

  void DumpAST() const override;

  void DumpIR() const override;

  void debugAST() const override;
};

class unaryExp_2_unaryOp_and_unaryExp_AST : public BaseAST {
  public:
    std::string unaryOp;
    std::unique_ptr<BaseAST> unaryExp;

  void DumpAST() const override;

  void DumpIR() const override;

  void debugAST() const override;
};

class primaryExp_2_Exp_AST : public BaseAST {
public:
  std::unique_ptr<BaseAST> exp;

  void DumpAST() const override;

  void DumpIR() const override;

  void debugAST() const override;
};

class primaryExp_2_num_AST : public BaseAST {
public:
  int number;

  void DumpAST() const override;

  void DumpIR() const override;

  void debugAST() const override;
};



