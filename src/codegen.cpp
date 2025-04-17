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

    koopa_delete_raw_program_builder(builder);
    cout.rdbuf(oldbuf); //恢复cout
}