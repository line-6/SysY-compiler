#include <sstream>
#include <fstream>
#include <cassert>
#include "koopa.h"
#include "codegen.h"
#include "visit.h"

int num_reg_t = 0;

void koopa_codegen(std::unique_ptr<BaseAST> &ast, const char* output_file){
    ofstream out(output_file);
    streambuf *oldbuf = cout.rdbuf(out.rdbuf());
    ast->DumpIR();
    cout.rdbuf(oldbuf); //恢复cout
}

void riscv_codegen(std::unique_ptr<BaseAST> &ast, const char* output_file){
    // 重定向 cout 到字符串
    stringstream ss;
    streambuf* buffer = cout.rdbuf(); //oldbuffer,STDOUT的缓冲区
    cout.rdbuf(ss.rdbuf());

    ast->DumpIR();

    string ir(ss.str());
    cout.rdbuf(buffer); //恢复 cout
    const char* ir_str = ir.c_str();  // 拿到 char* 形式的ir

    // 重定向输出到文件
    ofstream out(output_file);
    streambuf *oldbuf = cout.rdbuf(out.rdbuf());

    // koopa ir -> raw program
    koopa_program_t program;
    koopa_error_code_t code_ret = koopa_parse_from_string(ir_str, &program);
    assert(code_ret == KOOPA_EC_SUCCESS);
    koopa_raw_program_builder_t builder = koopa_new_raw_program_builder();
    koopa_raw_program_t raw = koopa_build_raw_program(builder, program);
    koopa_delete_program(program);

    visit(raw);
    // 操作raw program
    // cout << "   .text" << endl;
    // cout << "   .global main" << endl;

    // 使用 for 循环遍历函数列表
    /*
    for (size_t i = 0; i < raw.funcs.len; ++i) {
      assert(raw.funcs.kind == KOOPA_RSIK_FUNCTION);
      koopa_raw_function_t func = (koopa_raw_function_t) raw.funcs.buffer[i];
      string func_name_with_head(func->name);
      string func_name = func_name_with_head.substr(1);
      cout << func_name << ":" << endl;
      // 遍历函数的基本块
      for (size_t j = 0; j < func->bbs.len; ++j) {
      assert(func->bbs.kind == KOOPA_RSIK_BASIC_BLOCK);
      koopa_raw_basic_block_t bb = (koopa_raw_basic_block_t) func->bbs.buffer[j];
      
        // 遍历基本块的指令
        for (size_t k = 0; k < bb->insts.len; ++k) {
          std::cout << "inst len: " << bb->insts.len << std::endl;
          assert(bb->insts.kind == KOOPA_RSIK_VALUE);
          // 拿到指令
          koopa_raw_value_t value =  (koopa_raw_value_t) bb->insts.buffer[k];

          switch (value->kind.tag){
            // 二元运算符指令
            case KOOPA_RVT_BINARY:
              // 拿到操作符种类
              koopa_raw_binary_op op = (koopa_raw_binary_op)value->kind.data.binary.op;
              // 拿到左右操作数
              int32_t left_num = value->kind.data.binary.lhs->kind.data.integer.value;
              int32_t right_num = value->kind.data.binary.rhs->kind.data.integer.value;
              switch (op) {
                case KOOPA_RBO_SUB:
                  
                  break;
                case KOOPA_RBO_EQ:
                  break;
                default: break;
              }
              break;
            // RET 指令
            // case KOOPA_RVT_RETURN:
            //   break;
            //default: break;
          }

          // 示例程序中指令一定是RETURN
          //assert(value->kind.tag == KOOPA_RVT_RETURN);
          //koopa_raw_value_t ret_value = value->kind.data.ret.value;

          // 示例程序的value一定是INT
          //assert(ret_value->kind.tag == KOOPA_RVT_INTEGER);
          //int32_t int_val = ret_value->kind.data.integer.value;

          // 示例程序的value一定是0
          //assert(int_val == 0);
          //cout << "   li a0, " << int_val << endl;
          //cout << "   ret" << endl;


        }
    
      }
    }
    */

    koopa_delete_raw_program_builder(builder);
    cout.rdbuf(oldbuf); //恢复cout
}