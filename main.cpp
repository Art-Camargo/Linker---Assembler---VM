#include "instructions.h"
#include "assembler.h"
#include "linker.h"
#include "vm.h"
#include <iostream>
#include <vector>
#include <math.h>

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
  LinkedProgram linkedProgram;
  linkedProgram.memory.reserve(MEMORY_SIZE * totalPrograms);
  linkedProgram.symbolTable.reserve(MAX_SYMBOLS * totalPrograms); 
  linkedProgram.dataMemory.reserve(MEMORY_SIZE * totalPrograms);

  initPrograms(programs);
  initAdjustedFilePaths(filePaths, argc, argv);
  
  assembleManyPrograms(programs, filePaths);
  linkerFirstPass(programs, linkedProgram);
  linkerSecondPass(programs, linkedProgram);

  printSymbolTable(linkedProgram);
  printDataMemory(linkedProgram);
  printInstructionMemory(linkedProgram);
  cout << "Starting VM..." << endl << endl;

  vm(linkedProgram);



  return 0;
}
