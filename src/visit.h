#pragma once

//
//  访问 koopa ir
//

#include "koopa.h"

// 访问 raw program
void visit(const koopa_raw_program_t &program);

// 访问 raw slice
// slice 可以是一系列 func / basic block / value(指令) 的列表
// 具体种类见 koopa.h:70 
void visit(const koopa_raw_slice_t &slice);

// 访问 raw func 
void visit(const koopa_raw_function_t &func);

// 访问 raw basic block
void visit(const koopa_raw_basic_block_t &bb);

// 访问 raw 指令
void visit(const koopa_raw_value_t &value);

int visit(const koopa_raw_integer_t &integer);

void visit(const koopa_raw_return_t &ret);

void visit(const koopa_raw_binary_t &binary);