#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H
#include <string>
#include <vector>

using namespace std;

#define MEMORY_SIZE 320
#define MAX_SYMBOLS 100
#define REGISTERS 4
#define FILLED 1
#define EMPTY 0

enum SymbolType {
    SYMBOL_LABEL,
    SYMBOL_VARIABLE
};

typedef struct {
    string name;
    SymbolType type;
    int memoryAddress;
} SymbolTableEntry;

typedef enum {
  MEM_DATA,
  MEM_INSTRUCTION
} MemoryType;

typedef struct {
  int opcode;
  int operand1;
  int operand2;
  int operand3;
  
} Instruction;

typedef struct {
  MemoryType type;  
  char labelToLinker[30]; 
  int operandToLinker;
  union {
      int value;          
      Instruction instr;   
  };
} MemoryCell;

typedef struct {
    vector<MemoryCell> memory;
    vector<SymbolTableEntry> symbolTable;
} AssembledProgram;

enum Intructions {
  NOP = -1,
  ADD = 0,
  SUB,
  MUL,
  DIV,
  MV,
  ST,
  JMP,
  JEQ,
  JGT,
  JLT,
  W,
  R,
  STOP
};

#endif