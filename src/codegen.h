#pragma once

#include <memory>
#include <iostream>
#include "AST.h"

extern int num_reg_t;

void koopa_codegen(std::unique_ptr<BaseAST> &ast, const char* output_file);

void riscv_codegen(std::unique_ptr<BaseAST> &ast, const char* output_file);