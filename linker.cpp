#include "instructions.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

using namespace std;

void linkerFirstPass(const vector<AssembledProgram>& programs, LinkedProgram& linkedProgram) {
  int dataAddressCursor = 0;
  int codeOffset = 0;
  int currentProgramIndex = 0;

  for (const auto& prog : programs) {
      int i = 0;
      while (i <  (int)prog.memory.size() && prog.memory[i].type == MEM_DATA) {
          linkedProgram.dataMemory.push_back(prog.memory[i].value);
          ++i;
      }

      int localDataSize = i;
      int localInstructionCount = (int)prog.memory.size() - localDataSize;

      for (const auto& entry : prog.symbolTable) {
        cout <<  entry.scope << endl;
          if (entry.type == SYMBOL_VARIABLE) {
              SymbolTableEntry adjusted = entry;
              adjusted.memoryAddress += dataAddressCursor;
              adjusted.programIndex = currentProgramIndex;
              adjusted.scope = entry.scope;
              linkedProgram.symbolTable.push_back(adjusted);
          } else if (entry.type == SYMBOL_LABEL) {
              SymbolTableEntry adjusted = entry;
              adjusted.memoryAddress = entry.memoryAddress - localDataSize + codeOffset;
              adjusted.programIndex = currentProgramIndex;
              adjusted.scope = entry.scope;
              linkedProgram.symbolTable.push_back(adjusted);
          }
      }

      dataAddressCursor = (int)linkedProgram.dataMemory.size();
      codeOffset += localInstructionCount;
      currentProgramIndex++;
  }
}

void printSymbolTable(const LinkedProgram& linkedProgram) {
  cout << "Symbol Table:" << endl;
  for (const auto& entry : linkedProgram.symbolTable) {
      cout << "Symbol: " << entry.name 
           << ", Type: " << (entry.type == SYMBOL_LABEL ? "Label" : "Variable")
           << ", Address: " << entry.memoryAddress 
           << ", Scope: " << (entry.scope == SCOPE_GLOBAL ? "Global" : (entry.scope == SCOPE_LOCAL ? "Local" : "None"))
           << ", Program Index: " << entry.programIndex
           << endl;
  }
}

void printDataMemory(const LinkedProgram& linkedProgram) {
  cout << "Data Memory:" << endl;
  for (size_t i = 0; i < linkedProgram.dataMemory.size(); ++i) {
      cout << "Address " << i << ": " << linkedProgram.dataMemory[i] << endl;
  }
}

void printInstructionMemory(const LinkedProgram& linkedProgram) {
  cout << "Instruction Memory:" << endl;
  for (size_t i = 0; i < linkedProgram.memory.size(); ++i) {
      const Instruction& instr = linkedProgram.memory[i];
      cout << "Address " << i << ": "
           << "Opcode: " << instr.opcode 
           << ", Operand1: " << instr.operand1 
           << ", Operand2: " << instr.operand2 
           << ", Operand3: " << instr.operand3 
           << endl;
  }
}

void linkerSecondPass(const vector<AssembledProgram>& programs, LinkedProgram& linkedProgram) {
    int instructionCursor = 0;
    int currentProgramIndex = 0;
  
    for (const auto& prog : programs) {
        int i = 0;
        while (i < (int)prog.memory.size() && prog.memory[i].type == MEM_DATA) {
            ++i;
        }
  
        for (; i < (int)prog.memory.size(); ++i) {
            const MemoryCell& cell = prog.memory[i];
            Instruction instr = cell.instr;
  
            if (!cell.labelToLinker.empty() && cell.operandToLinker != -1) {
                bool found = false;
  
                for (const auto& entry : linkedProgram.symbolTable) {
                    if (entry.name == cell.labelToLinker &&
                        (entry.scope == SCOPE_GLOBAL || entry.programIndex == currentProgramIndex)) {
                        
                        if (cell.operandToLinker == 1) instr.operand1 = entry.memoryAddress;
                        else if (cell.operandToLinker == 2) instr.operand2 = entry.memoryAddress;
                        else if (cell.operandToLinker == 3) instr.operand3 = entry.memoryAddress;
  
                        found = true;
                        break;
                    }
                }
  
                if (!found) {
                    cerr << "Erro: símbolo '" << cell.labelToLinker 
                         << "' não encontrado na segunda passagem do linker.\n";
                    exit(EXIT_FAILURE);
                }
            }
  
            linkedProgram.memory.push_back(instr);
            ++instructionCursor;
        }
  
        ++currentProgramIndex;
    }
  }
  
