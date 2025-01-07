#include "visit.h"
#include "dump.h"
#include <cassert>
#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include <unordered_map>

static std::vector<std::string> reg = {"t0", "t1", "t2", "t3", "t4", "t5", "t6",
                                       "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7"};
static int reg_pc = 0;                                     

// 为每个 value 记录其对应偏移量
static std::unordered_map<koopa_raw_value_t, size_t> val_offs;
static int offs = 0;

void visit(const koopa_raw_program_t &program){
    // ... 一些操作
    std::cout << "   .text" << std::endl;
    std::cout << "   .global main" << std::endl;
    visit(program.funcs);
    visit(program.values);
}
 
void visit(const koopa_raw_slice_t &slice){
    // 拿到这个 slice 对应的 kind
    int slice_kind = slice.kind;

    for (size_t i = 0; i < slice.len; i ++) {
        auto p = slice.buffer[i];

        switch (slice_kind) {
            case KOOPA_RSIK_FUNCTION:
                visit(reinterpret_cast<koopa_raw_function_t>(p));
                break;
            case KOOPA_RSIK_BASIC_BLOCK:
                visit(reinterpret_cast<koopa_raw_basic_block_t>(p));
                break;
            case KOOPA_RSIK_VALUE:
                visit(reinterpret_cast<koopa_raw_value_t>(p));
                break;
            default:
                assert(false); 
                break;
        }
    }
}
 
void visit(const koopa_raw_function_t &func){
    std::string func_name_with_head(func->name);
    std::string func_name = func_name_with_head.substr(1);
    std::cout << func_name << ":" << std::endl;
    visit(func->bbs);
}

void visit(const koopa_raw_basic_block_t &bb){
    visit(bb->insts);
}

void visit(const koopa_raw_value_t &value){
    // 这里需要改进，(设置一个 map?)为每个 value 记录其对应偏移量
    // %0 = eq 6, 0
    // %1 = sub 0, %0
    // %2 = sub 0, %1
    // 对应的偏移量分别是 0 1 2
    val_offs[value] = offs++;
    auto &kind = value->kind;
    switch (kind.tag) {   
        case KOOPA_RVT_INTEGER:
            visit(kind.data.integer);
            break;  
        case KOOPA_RVT_BINARY:
            visit(kind.data.binary);
            break;
        case KOOPA_RVT_RETURN:
            visit(kind.data.ret);
            break;
        default:
            assert(false);
    }
}

int visit(const koopa_raw_integer_t &integer) {
    return integer.value;
}

void visit(const koopa_raw_return_t &ret) {
    auto &kind = ret.value->kind;
    switch (kind.tag) {
        case KOOPA_RVT_INTEGER:
            std::cout << "  li    " << "a0, " << visit(kind.data.integer);
            break;
        case KOOPA_RVT_BINARY:
            {
                size_t ret_offs = val_offs[ret.value];
                std::cout << "  mv    a0, " << reg[ret_offs] << std::endl;
                break;
            }
        default: break;
    }
    std::cout << "  ret" << std::endl;
}

// 代码略显臃肿，后续优化
// 1. 把例如 std::cout << "  xor   " << reg[reg_pc++] << ", " << "x0, x0" << std::endl;
//     这种cout都写成一个函数 xor(string a, string b, string c);不再写复杂的cout表达式了
static void koopa_to_rsicv_bo_eq(const koopa_raw_binary_t &binary) {
   
    /* 分三种情况：
        1. 左右操作数都是立即数:    %0 = eq 6, 0
            lhs(rhs)->kind.tag = KOOPA_RVT_INTEGER
        2. 左立即,右变量:          %1 = sub 0, %0
            rhs->kind.tag = KOOPA_RVT_BINARY; 寄存器 pc ++
        3. 左右变量:               %3 = sub %2, %1
            lhs(rhs)->kind.tag = KOOPA_RVT_BINARY;寄存器pc ++ 
    */ 

   if (binary.lhs->kind.tag == KOOPA_RVT_INTEGER && binary.rhs->kind.tag == KOOPA_RVT_INTEGER) {
        // 立即数除了 0 (直接调用x0)，都需要先 load 进寄存器
        int left_num  = binary.lhs->kind.data.integer.value;
        int right_num = binary.rhs->kind.data.integer.value;
        if (left_num != 0 && right_num != 0) {
            // 目前见不到这种情况
        }

        else if (left_num == 0 && right_num == 0) {
            dump(std::string("xor"), reg[reg_pc++], std::string("x0"), std::string("x0"));
            dump(std::string("seqz"), reg[reg_pc - 1],reg[reg_pc - 1]);
        }

        else if (left_num != 0 && right_num == 0) {
            dump(std::string("li"), reg[reg_pc++], std::to_string(left_num));
            dump(std::string("xor"), reg[reg_pc - 1], reg[reg_pc - 1], std::string("x0"));
            dump(std::string("seqz"), reg[reg_pc - 1], reg[reg_pc - 1]); 
        } 

        else if (left_num == 0 && right_num != 0) {
            dump(std::string("li"), reg[reg_pc++], std::to_string(right_num));
            dump(std::string("xor"), reg[reg_pc - 1], std::string("x0"), reg[reg_pc - 1]);
            dump(std::string("seqz"), reg[reg_pc - 1], reg[reg_pc - 1]);
        }
   }

   else if (binary.lhs->kind.tag == KOOPA_RVT_INTEGER && binary.rhs->kind.tag == KOOPA_RVT_BINARY) {
        int left_num  = binary.lhs->kind.data.integer.value;
        size_t right_offs = val_offs[binary.rhs];

        if (left_num == 0) {
            dump(std::string("xor"), reg[reg_pc++], std::string("x0"), reg[right_offs]);
            dump(std::string("seqz"), reg[reg_pc - 1], reg[reg_pc - 1]);
        }
        else {
            dump(std::string("li"), reg[reg_pc++], std::to_string(left_num));
            dump(std::string("xor"), reg[reg_pc - 1], reg[reg_pc - 1], reg[right_offs]);
            dump(std::string("seqz"), reg[reg_pc - 1], reg[reg_pc - 1]);
        }
   }

   else if (binary.lhs->kind.tag == KOOPA_RVT_BINARY && binary.rhs->kind.tag == KOOPA_RVT_BINARY) {
        size_t left_offs = val_offs[binary.lhs];
        size_t right_offs = val_offs[binary.rhs];

        dump(std::string("xor"), reg[reg_pc++], reg[left_offs], reg[right_offs]);
        dump(std::string("seqz"), reg[reg_pc - 1], reg[reg_pc - 1]);
   }

   else {
        int right_num  = binary.rhs->kind.data.integer.value;
        size_t left_offs = val_offs[binary.lhs];

        if (right_num == 0) {
            dump(std::string("xor"), reg[reg_pc++], reg[left_offs], std::string("x0"));
            dump("seqz", reg[reg_pc - 1], reg[reg_pc - 1]);
        }
        else {
            dump(std::string("li"), reg[reg_pc++], std::to_string(right_num));
            dump(std::string("xor"), reg[reg_pc - 1], reg[left_offs], reg[reg_pc - 1]);
            dump(std::string("seqz"), reg[reg_pc - 1], reg[reg_pc - 1]);
        }
   }
}

static void koopa_to_rsicv_bo_sub(const koopa_raw_binary_t &binary) {
    if (binary.lhs->kind.tag == KOOPA_RVT_INTEGER) {
        int left_num = binary.lhs->kind.data.integer.value;
        if (binary.rhs->kind.tag == KOOPA_RVT_INTEGER) {
            int left_num = binary.lhs->kind.data.integer.value;
            int right_num = binary.rhs->kind.data.integer.value;

            if (left_num != 0 && right_num != 0) {
            }
            else if (left_num == 0 && right_num == 0) {
            }
            else if (left_num != 0 && right_num == 0) {
            }
            else if (left_num == 0 && right_num != 0) {
                dump(std::string("li"), reg[reg_pc++], std::to_string(right_num));
                dump(std::string("sub"), reg[reg_pc - 1], std::string("x0"), reg[reg_pc - 1]);
            }
        }
        else if (binary.rhs->kind.tag == KOOPA_RVT_BINARY) {
            size_t right_offs = val_offs[binary.rhs];
            if (left_num == 0) {
                dump(std::string("sub"), reg[reg_pc++], std::string("x0"), reg[right_offs]);
            }
            else {
                dump(std::string("li"), reg[reg_pc++], std::to_string(left_num));
                dump(std::string("sub"), reg[reg_pc - 1], reg[reg_pc - 1], reg[right_offs]);
            }
        }
    }
    else if (binary.lhs->kind.tag == KOOPA_RVT_BINARY) {
        size_t left_offs = val_offs[binary.lhs];
        if (binary.rhs->kind.tag == KOOPA_RVT_INTEGER)  {
            int right_num = binary.rhs->kind.data.integer.value;
            if (right_num == 0) {
                dump(std::string("sub"), reg[reg_pc++], reg[left_offs], std::string("x0"));
            }
            else {
                dump(std::string("li"), reg[reg_pc++], std::to_string(right_num));
                dump(std::string("sub"), reg[reg_pc - 1], reg[left_offs], reg[reg_pc - 1]);
            }
        }
        else if (binary.rhs->kind.tag == KOOPA_RVT_BINARY) {
            size_t left_offs = val_offs[binary.lhs];
            size_t right_offs = val_offs[binary.rhs];
            dump(std::string("sub"), reg[reg_pc++], reg[left_offs], reg[right_offs]);
        }
    }
}

void visit(const koopa_raw_binary_t &binary) {
    switch (binary.op) {
        /// Equal to.
        case KOOPA_RBO_EQ:
            koopa_to_rsicv_bo_eq(binary);  
            break;

        /// Subtraction.
        case KOOPA_RBO_SUB:
            koopa_to_rsicv_bo_sub(binary);
            break;

        default: break;
    }
}