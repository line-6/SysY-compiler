#include <sstream>
#include <fstream>
#include <cassert>
#include "koopa.h"
#include "codegen.h"
#include "visit.h"

int num_reg_t = 0;

void koopa_codegen(std::unique_ptr<BaseAST> &ast, const char* output_file){
    std::ofstream out(output_file);
    std::streambuf *oldbuf = std::cout.rdbuf(out.rdbuf());
    ast->DumpIR();
    std::cout.rdbuf(oldbuf); //恢复cout
}

void riscv_codegen(std::unique_ptr<BaseAST> &ast, const char* output_file){
    // 重定向 cout 到字符串
    std::stringstream ss;
    std::streambuf* buffer = std::cout.rdbuf(); //oldbuffer,STDOUT的缓冲区
    std::cout.rdbuf(ss.rdbuf());

    ast->DumpIR();

    std::string ir(ss.str());
    std::cout.rdbuf(buffer); //恢复 cout
    const char* ir_str = ir.c_str();  // 拿到 char* 形式的ir

    // 重定向输出到文件
    std::ofstream out(output_file);
    std::streambuf *oldbuf = std::cout.rdbuf(out.rdbuf());

    // koopa ir -> raw program
    koopa_program_t program;
    koopa_error_code_t code_ret = koopa_parse_from_string(ir_str, &program);
    assert(code_ret == KOOPA_EC_SUCCESS);
    koopa_raw_program_builder_t builder = koopa_new_raw_program_builder();
    // 拿到 raw program：内存形式的 koopa ir
    koopa_raw_program_t raw = koopa_build_raw_program(builder, program);
    koopa_delete_program(program);

    visit(raw);

    koopa_delete_raw_program_builder(builder);
    std::cout.rdbuf(oldbuf); //恢复cout
}