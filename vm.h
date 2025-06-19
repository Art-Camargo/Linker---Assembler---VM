// vm.h
#ifndef VM_H
#define VM_H

#include <vector>
#include "instructions.h"

void alu(int opcode, int registers[], int operand1, int operand2, int operand3);

void handleJump(int opcode, int registers[], int& pc, int operand1, int operand2, int operand3, std::vector<int>& dataMemory);

void dispatch(const Instruction& instruction, int registers[], std::vector<int>& dataMemory, int pc);

void run(const std::vector<Instruction>& instructionMemory, std::vector<int>& dataMemory);

void vm(LinkedProgram memory);

#endif // VM_H
