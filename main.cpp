#include "instructions.h"
#include "assembler.h"
#include <iostream>
#include <vector>

#define PROGRAMS_DIR "./programs/"

using namespace std;

int getTotalPrograms(int argc) {
  const int total = argc - 1;
  if (total < 1) {
    cout << "No programs provided. Please provide at least one program." << endl;
    exit(EXIT_FAILURE);
  }
  return total;
}

void initializeProgram(AssembledProgram& prog) {
  prog.memory.clear();       
  prog.symbolTable.clear();  
}

void adjustFilePath(string& filePath) {
  if (filePath.find(PROGRAMS_DIR) != 0) {
    filePath = PROGRAMS_DIR + filePath;
  }
}

void initPrograms(vector<AssembledProgram>& programs) {
  for (auto& prog : programs) {
    initializeProgram(prog);
  }
}

void initAdjustedFilePaths(vector<string>& filePaths, int argc, char* argv[]) {
  for (int i = 0; i < argc - 1; i++) {
    string filePath = argv[i + 1];
    adjustFilePath(filePath);
    filePaths.push_back(filePath);
  }
}

int main(int argc, char* argv[]) {
  const int totalPrograms = getTotalPrograms(argc);
  vector<AssembledProgram> programs(totalPrograms);
  vector<string> filePaths;

  initPrograms(programs);
  initAdjustedFilePaths(filePaths, argc, argv);
  
  assembleManyPrograms(programs, filePaths);

  for (const auto& prog : programs) {
    cout << "Program Memory: " << endl;
    for (const auto& cell : prog.memory) {
      if (cell.type == MEM_INSTRUCTION) {
        cout << "Instruction: " << cell.instr.opcode << " "
             << cell.instr.operand1 << " "
             << cell.instr.operand2 << " "
             << cell.instr.operand3 << endl;
      } else {
        cout << "Data: " << cell.value << endl;
      }
    }
    cout << "Symbol Table: " << endl;
    for (const auto& entry : prog.symbolTable) {
      cout << entry.name << " (" 
           << (entry.type == SYMBOL_LABEL ? "Label" : "Variable") 
           << ") at address: " 
           << entry.memoryAddress << endl;
    }
  }

  

  return 0;
}
