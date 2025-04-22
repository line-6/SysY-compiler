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

class ExpBaseAST;


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
  std::unique_ptr<ExpBaseAST> exp;

  void DumpAST() const override;
  void DumpIR() const override;

  void debugAST() const override;
};

//===----------Decalre of BaseExpAST----------------===//
// addExp ::= mulExp | addExp ("+" | "-") mulExp;
// Base class of all the Exp ast.
//===-------------------------------------------------===//
class ExpBaseAST : public BaseAST {
public:
  bool is_number;
  int val;  // 记录 number 的值
  int addr;

  virtual ~ExpBaseAST() = default;
  // 对每个 ast 节点：
  // 1. 如果是 number， 返回 number 的值
  // 2. 如果不是 number 返回 addr
  std::string GetPc();
  // 判断每个节点：
  // 1. 是 number：设置 is_number = true 和 val;
  // 2. 不是 number：设置 addr 为临时变量 pc 的值
  virtual void Eval() = 0;

};

//===----------Decalre of ExpAST----------------===//
// Exp ::= AddExp;
//===-------------------------------------------------===//
class ExpAST : public ExpBaseAST {
  public:
  std::unique_ptr<ExpBaseAST> add_exp;

  void DumpAST() const override;

  void DumpIR() const override;

  void debugAST() const override;

  void Eval() override;
};

//===----------Decalre of AddExpAST----------------===//
// addExp ::= mulExp | addExp ("+" | "-") mulExp;
//===-------------------------------------------------===//
class AddExpAST : public ExpBaseAST {
  public:
    // 这里需要判断 addExp 由哪种形式归约 
    // 若 addExp 为 null, 说明归约表达式为 addExp ::= mulExp
    // 反之 addExp ::= addExp ("+" | "-") mulExp;
    std::unique_ptr<ExpBaseAST> mul_exp = nullptr;
    std::unique_ptr<ExpBaseAST> add_exp = nullptr;
    std::string op;

    void DumpAST() const override;

    void DumpIR() const override;

    void debugAST() const override;

    void Eval() override;
};

//===----------Decalre of MulExpAST----------------===//
// mulExp ::= unaryExp | mulExp ("*" | "/" | "%") unaryExp;
//===-------------------------------------------------===//
class MulExpAST : public ExpBaseAST {
  public:
    std::unique_ptr<ExpBaseAST> unary_exp = nullptr;
    std::unique_ptr<ExpBaseAST> mul_exp = nullptr;
    std::string op;

    void DumpAST() const override;

    void DumpIR() const override;

    void debugAST() const override;

    void Eval() override;
};

//===----------Decalre of UnaryExpAST----------------===//
// unaryExp ::= primaryExp | unaryOp unaryExp;
//===----------------------------------------------------===//
class UnaryExpAST : public ExpBaseAST {
  public:
    std::unique_ptr<ExpBaseAST> primary_exp = nullptr;
    std::unique_ptr<ExpBaseAST> unary_exp = nullptr;
    std::string unary_op;

  void DumpAST() const override;

  void DumpIR() const override;

  void debugAST() const override;

  void Eval() override;
};

//===----------Decalre of PrimaryExpAST----------------===//
// primaryExp ::= "(" Exp ")" | Number;
//===-----------------------------------------------------===//
class PrimaryExpAST : public ExpBaseAST {
public:
  std::unique_ptr<ExpBaseAST> exp = nullptr;
  int number;

  void DumpAST() const override;

  void DumpIR() const override;

  void debugAST() const override;

  void Eval() override;
};




