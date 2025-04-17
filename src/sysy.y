// Stmt        ::= "return" Exp ";";

// Exp         ::= unaryExp    =====>
    
// Exp         ::= addExp;
// primaryExp  ::= "(" Exp ")" | Number;
// Number      ::= INT_CONST;
// unaryExp    ::= primaryExp | unaryOp unaryExp;
// unaryOp     ::= "+" | "-" | "!";

// mulExp      ::= unaryExp | mulExp ("*" | "/" | "%") unaryExp;
// addExp      ::= mulExp | addExp ("+" | "-") mulExp;


%code requires {
  #include <memory>
  #include <string>
  #include "AST.h"
}

// 声明 lexer 函数和错误处理函数
%{

#include <iostream>
#include <memory>
#include <string>
#include "AST.h"

int yylex();
void yyerror(std::unique_ptr<BaseAST> &ast, const char* s);
BaseAST* handle_unaryOp(const std::string& op, BaseAST* exp);
BaseAST* handle_addOp(const std::string& op, BaseAST* addExp, BaseAST* mulExp);
BaseAST* handle_mulOp(const std::string& op, BaseAST* mulExp, BaseAST* unaryExp);

%}

// 定义 parser 函数和错误处理函数的附加参数
// 我们需要返回一个字符串作为 AST, 所以我们把附加参数定义成字符串的智能指针
// 解析完成后, 我们要手动修改这个参数, 把它设置成解析得到的字符串

%parse-param { std::unique_ptr<BaseAST> &ast }

// %union 修改 yylval 的定义, 我们把它定义成了一个联合体 (union), yylval 默认是 int 类型 
// 因为 token 的值有的是字符串指针, 有的是整数
// 之前我们在 lexer 中用到的 str_val 和 int_val 就是在这里被定义的
// 至于为什么要用字符串指针而不直接用 string 或者 unique_ptr<string>?
// 请自行 STFW 在 union 里写一个带析构函数的类会出现什么情况
%union {
  std::string *str_val;
  int int_val;
  BaseAST *ast_val;
}

%token INT RETURN 
%token <str_val> IDENT 
%token <int_val> INT_CONST

%type <ast_val> FuncDef FuncType Block Stmt Exp addExp mulExp unaryExp primaryExp
%type <int_val> Number 

%%

// {}中的动作是在已经 **分析完FuncDef，即将归约为CompUnit前执行**
// $1 拿到了FuncDef的值
CompUnit
  : FuncDef {
    //std::cout << std::endl << "this is CompUnitAST" << std::endl;
    auto comp_unit = std::make_unique<CompUnitAST>();
    comp_unit->func_def = std::unique_ptr<BaseAST>($1);
    ast = std::move(comp_unit);
  }
  ;

FuncDef   
  : FuncType IDENT '(' ')' Block {
    //std::cout << std::endl << "this is FuncDefAST" << std::endl;
    //auto ast = std::make_unique<FuncDefAST>();
    auto ast = new FuncDefAST();
    ast->func_type = std::unique_ptr<BaseAST>($1);
    ast->ident = *std::unique_ptr<std::string>($2);
    ast->block = std::unique_ptr<BaseAST>($5);
    $$ = ast;
  }
  ;

FuncType
  : INT {
    //std::cout << std::endl << "this is FuncTypeAST" << std::endl;
    //auto ast = std::make_unique<FuncTypeAST>();
    auto ast = new FuncTypeAST();
    ast->func_type = std::string("int");
    $$ = ast;
  }
  ;

Block
  : '{' Stmt '}' {
    //std::cout << std::endl << "this is BlcokAST" << std::endl;
    //auto ast = std::make_unique<BlockAST>();
    auto ast = new BlockAST();
    ast->stmt = std::unique_ptr<BaseAST>($2);
    $$ = ast;
  }
  ;

Stmt  
  : RETURN Exp ';' {
    //std::cout << std::endl << "this is StmtAST" << std::endl;
    auto ast = new StmtAST();
    ast->exp = std::unique_ptr<BaseAST>($2);    
    $$ = ast;
  }
  ;

Exp
  : addExp {
    auto ast = new ExpAST();
    ast->addExp = std::unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  ;

addExp
  : mulExp {
    auto ast = new addExp_2_mulExp_AST();
    ast->mulExp = std::unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  | addExp '+' mulExp { $$ = handle_addOp("+", $1, $3); }
  | addExp '-' mulExp { $$ = handle_addOp("-", $1, $3); }
  ;

mulExp
  : unaryExp {
    auto ast = new mulExp_2_unaryExp_AST();
    ast->unaryExp = std::unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  | mulExp '*' unaryExp { $$ = handle_mulOp("*", $1, $3); }
  | mulExp '/' unaryExp { $$ = handle_mulOp("/", $1, $3); }
  | mulExp '%' unaryExp { $$ = handle_mulOp("%", $1, $3); }
  ;

unaryExp
  : primaryExp {
    auto ast = new unaryExp_2_primaryExp_AST();
    ast->primaryExp = std::unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  | '+' unaryExp {  $$ = handle_unaryOp("+", $2); }
  | '-' unaryExp {  $$ = handle_unaryOp("-", $2); }
  | '!' unaryExp {  $$ = handle_unaryOp("!", $2); }
  ;

primaryExp
  : '(' Exp ')' {
    auto ast = new primaryExp_2_Exp_AST();
    ast->exp = std::unique_ptr<BaseAST>($2);
    $$ = ast;
  }
  | Number {
    auto ast = new primaryExp_2_num_AST();
    ast->number = ($1);
    $$ = ast;
  }
  ;

Number    
  : INT_CONST {
    //std::cout << std::endl << "this is NumberAST" << std::endl;
    $$ = ($1);
  }
  ;

%%

void yyerror(std::unique_ptr<BaseAST> &ast, const char *s) {
  std::cerr << "error: " << s << std::endl;
}

BaseAST* handle_unaryOp(const std::string& op, BaseAST* exp) {
    auto ast = new unaryExp_2_unaryOp_and_unaryExp_AST();
    ast->unaryExp = std::unique_ptr<BaseAST>(exp);
    ast->unaryOp = op;
    return ast;
}

BaseAST* handle_addOp(const std::string& op, BaseAST* addExp, BaseAST* mulExp){
    auto ast = new addExp_2_addExp_op_mulExp_AST();
    ast->addExp = std::unique_ptr<BaseAST>(addExp);
    ast->op = op;
    ast->mulExp = std::unique_ptr<BaseAST>(mulExp);
    return ast;
}

BaseAST* handle_mulOp(const std::string& op, BaseAST* mulExp, BaseAST* unaryExp){
    auto ast = new mulExp_2_mulExp_op_unaryExp_AST();
    ast->mulExp = std::unique_ptr<BaseAST>(mulExp);
    ast->op = op;
    ast->unaryExp = std::unique_ptr<BaseAST>(unaryExp);
    return ast;
}
