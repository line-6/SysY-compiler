/*
    dump riscv instructions

.text
.global main
main:
  li    t0, 6
  sub   t0, x0, t0
  xor   t1, t0, x0
  seqz  t1, t1
  sub   t2, x0, t1
  mv    a0, t2
  ret
*/
#include <string>

// three-operands inst: sub / xor / 
void dump(std::string op, std::string dest, std::string src1, std::string src2);

// two-operands inst: li / seqz / mv / 
void dump(std::string op, std::string dest, std::string src1, std::string src2);

// one-operand inst:
void dump(std::string op, std::string dest, std::string src1);

//zero-operand inst: ret /
void dump(std::string op);