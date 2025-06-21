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

enum  ScopeType {
    SCOPE_GLOBAL,
    SCOPE_LOCAL,
    SCOPE_NONE
};
typedef struct {
    string name;
    SymbolType type;
    int memoryAddress;
    ScopeType scope;
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
  string labelToLinker; 
  int operandToLinker;
  ScopeType scope;

  union {
      int value;          
      Instruction instr;   
  };
} MemoryCell;

typedef struct {
    vector<MemoryCell> memory;
    vector<SymbolTableEntry> symbolTable;
} AssembledProgram;

typedef struct {
    vector<int> dataMemory;
    vector<SymbolTableEntry> symbolTable;
    vector<Instruction> memory;
} LinkedProgram;

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
  STP
};

#endif