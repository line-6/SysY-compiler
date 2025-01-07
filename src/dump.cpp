#include "dump.h"
#include <iostream>

// three-operands inst: sub / xor / 
void dump(std::string op, std::string dest, std::string src1, std::string src2) {
    std::cout << "  " << op << "   " << dest << ", " << src1 << ", " << src2 << std::endl;
}

// two-operands inst: li / seqz / mv / 
void dump(std::string op, std::string dest, std::string src1) {
    std::cout << "  " << op << "   " << dest << ", " << src1 << std::endl;
}

// one-operand inst:
void dump(std::string op, std::string dest) {
    std::cout << "  " << op << "   " << dest << std::endl;
}

//zero-operand inst: ret /
void dump(std::string op) {
    std::cout << "  " << op << std::endl;
}