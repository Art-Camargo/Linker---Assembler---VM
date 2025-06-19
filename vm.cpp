#include "instructions.h"
#include <vector>
#include <iostream>
#include <string>

using namespace std;

void alu(int opcode, int registers[], int operand1, int operand2, int operand3) {
  switch (opcode) {
    case ADD:
      registers[operand1] = registers[operand2] + registers[operand3];
      break;
    case SUB:
      registers[operand1] = registers[operand2] - registers[operand3];
      break;
    case MUL:
      registers[operand1] = registers[operand2] * registers[operand3];
      break;
    case DIV:
      if (registers[operand3] == 0) {
        registers[operand1] = 0;
      } else {
        registers[operand1] = registers[operand2] / registers[operand3];
      }
      break;
  }
}

void handleJump(int opcode, int registers[], unsigned int* pc, int operand1, int operand2, int operand3, vector<int>& dataMemory) {
  switch (opcode) {
    case JMP:
      *pc = dataMemory[operand1];
      break;
    case JEQ:
      if (registers[operand1] == registers[operand2]) {
        *pc = dataMemory[operand3];
      } else {
        (*pc)++;
      }
      break;
    case JGT:
      if (registers[operand1] > registers[operand2]) {
        *pc = dataMemory[operand3];
      } else {
        (*pc)++;
      }
      break;
    case JLT:
      if (registers[operand1] < registers[operand2]) {
        *pc = dataMemory[operand3];
      } else {
        (*pc)++;
      }
      break;
  }
}

void dispatch(const Instruction& instruction, int registers[], vector<int>& dataMemory, unsigned int& pc) {
  switch (instruction.opcode) {
    case ADD:
    case SUB:
    case MUL:
    case DIV:
      alu(instruction.opcode, registers, instruction.operand1, instruction.operand2, instruction.operand3);
      pc++;
      break;
    case MV:
      registers[instruction.operand1] = dataMemory[instruction.operand2];
      pc++;
      break;
    case ST:
      dataMemory[instruction.operand1] = registers[instruction.operand2];
      pc++;
      break;
    case JMP:
    case JEQ:
    case JGT:
    case JLT:
      handleJump(instruction.opcode, registers, &pc, instruction.operand1, instruction.operand2, instruction.operand3, dataMemory);
      break;
    case W: {
      int value;
      cout << "Enter value for register " << instruction.operand1 << ": ";
      cin >> value;
      dataMemory[instruction.operand1] = value;
      pc++;
      break;
    }
    case R:
      cout << "Value: " << dataMemory[instruction.operand1] << endl;
      pc++;
      break;
    case STP:
      cout << "Program stopped." << endl;
      // Não incrementa PC para que o loop pare
      break;
  }
}

void printRegisters(const int registers[]) {
  cout << "Registers: " << endl << endl;
  for (int i = 0; i < REGISTERS; ++i) {
    cout << "A" << i << ": " << registers[i] << " ";
  }
  cout << endl << endl;
}

void run(const vector<Instruction>& instructionMemory, vector<int>& dataMemory) {
  unsigned int pc = 0;
  int registers[REGISTERS] = {0};

  while (pc < instructionMemory.size() && pc < MEMORY_SIZE && instructionMemory[pc].opcode != STP) {
    const Instruction& instruction = instructionMemory[pc];
    cout << "Executing instruction at PC: " << pc << endl;
    cout << "Instruction: " << instruction.opcode << " "
         << instruction.operand1 << " "
         << instruction.operand2 << " "
         << instruction.operand3 << endl;

    dispatch(instruction, registers, dataMemory, pc);
  }

  printRegisters(registers);
}

void vm(LinkedProgram memory) {
  vector<Instruction> instructionMemory = memory.memory;
  vector<int> dataMemory = memory.dataMemory;

  run(instructionMemory, dataMemory);
}