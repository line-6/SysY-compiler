#include <cassert>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <memory>
#include <cstring>
#include <string>
#include "AST.h"
#include "koopa.h"
#include "codegen.h"

using namespace std;

extern FILE *yyin;
extern int yyparse(unique_ptr<BaseAST> &ast);

// compiler mode input -o output
int main(int argc, const char *argv[]) {
  assert(argc == 5);
  auto mode = argv[1];
  auto input = argv[2];
  auto output = argv[4];

  // 拿到输入 Sysy 程序的 ast
  yyin = fopen(input, "r");
  assert(yyin);
  unique_ptr<BaseAST> ast;
  auto ret = yyparse(ast);
  assert(!ret);

  // ast->debugAST();
  // ast->DumpAST();

  // koopa_ir codegen
  if (strcmp(mode, "-koopa") == 0) {
    koopa_codegen(ast, output);
  }

  // riscv codegen
  else if (strcmp(mode, "-riscv") == 0) {
    riscv_codegen(ast, output);
  }

  return 0;
}
