#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "instructions.h"

using namespace std;

int convertMnemonicToOpcode(const string& mnemonic) {
    if (mnemonic == "ADD") return ADD;
    if (mnemonic == "SUB") return SUB;
    if (mnemonic == "MUL") return MUL;
    if (mnemonic == "DIV") return DIV;
    if (mnemonic == "MV") return MV;
    if (mnemonic == "ST") return ST;
    if (mnemonic == "JMP") return JMP;
    if (mnemonic == "JEQ") return JEQ;
    if (mnemonic == "JGT") return JGT;
    if (mnemonic == "JLT") return JLT;
    if (mnemonic == "W") return W;
    return NOP;
}

bool symbolExistsGlobally(const vector<AssembledProgram>& programs, int currentIndex, const string& name, int symbolType) {
  for (int i = 0; i < currentIndex; ++i) {
      for (const auto& entry : programs[i].symbolTable) {
          if (entry.name == name && (symbolType == -1 || entry.type == symbolType)) {
              return true;
          }
      }
  }
  return false;
}


void restartFile(ifstream& file) {
    file.clear(); 
    file.seekg(0, ios::beg); 
}

string trim_left(const string& str) {
  const string pattern = " \f\n\r\t\v";
  return str.substr(str.find_first_not_of(pattern));
}

string trim_right(const string& str) {
  const string pattern = " \f\n\r\t\v";
  return str.substr(0,str.find_last_not_of(pattern) + 1);
}

string trim(const string& str) {
  return trim_left(trim_right(str));
}

ifstream getFile(const string& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
      cout << "Error: Could not open file " << filePath << endl;
      exit(EXIT_FAILURE);
    }
    return file;
}

void secondPass(AssembledProgram& prog, ifstream& file, int initOfProgram) {
    string line;
    int lineNumber = 0;

    while (getline(file, line) && lineNumber < MEMORY_SIZE) {
        lineNumber++;
    }
}

void dispatchVariable(vector<AssembledProgram>& programs, int currentIndex, const string& varName, int value, int lineNumber) {
  if (symbolExistsGlobally(programs, currentIndex, varName, SYMBOL_VARIABLE)) {
      cout << "Error: Duplicate variable '" << varName << "' at line " << lineNumber << endl;
      exit(EXIT_FAILURE);
  }

  AssembledProgram& prog = programs[currentIndex];

  int memoryPosition = prog.memory.size();

  MemoryCell cell;
  cell.type = MEM_DATA;
  cell.value = value;
  prog.memory.push_back(cell);

  SymbolTableEntry entry;
  entry.name = varName;
  entry.type = SYMBOL_VARIABLE;
  entry.memoryAddress = memoryPosition; 
  prog.symbolTable.push_back(entry);
}


void dispatchLabel(vector<AssembledProgram>& programs, int currentIndex, const string& label, int lineNumber) {
  string labelName = label.substr(0, label.size() - 1);
  if (symbolExistsGlobally(programs, currentIndex, labelName, SYMBOL_LABEL)) {
      cout << "Error: Duplicate label '" << labelName << "' at line " << lineNumber << endl;
      exit(EXIT_FAILURE);
  }

  AssembledProgram& prog = programs[currentIndex];

  SymbolTableEntry entry;
  entry.name = labelName;
  entry.type = SYMBOL_LABEL;
  entry.memoryAddress = lineNumber;
  prog.symbolTable.push_back(entry);
}

void firstPass(vector<AssembledProgram>& programs, int currentIndex, ifstream& file, int *initOfProgram) {
  AssembledProgram& prog = programs[currentIndex];
  string line;
  int wordCount = 0;
  int lineNumber = 0;
  int instructionCount = 0;

  while (getline(file, line) && lineNumber < MEMORY_SIZE) {
      lineNumber++;

      if (!line.empty()) {
          istringstream iss(trim(line));
          string token, varName, value;
          iss >> token >> varName >> value;

          if (token.back() == ':' && token.size() > 1) {
              dispatchLabel(programs, currentIndex, token, instructionCount);
          } else if (token == "WORD") {
              if (varName.empty()) {
                  cout << "Error: Invalid WORD declaration at line " << lineNumber << endl;
                  exit(EXIT_FAILURE);
              }
              dispatchVariable(programs, currentIndex, varName, stoi(value), wordCount++);
          } else if (convertMnemonicToOpcode(token) != NOP) {
              instructionCount++;
          }
      }
  }

  for (auto& entry : prog.symbolTable) {
    if (entry.type == SYMBOL_LABEL) {
        entry.memoryAddress += wordCount;
    }
  }

  *initOfProgram = wordCount;
}

void readProgram(vector<AssembledProgram>& programs, int currentIndex, const string& filePath) {
  ifstream file = getFile(filePath);
  int initOfProgram = 0;

  firstPass(programs, currentIndex, file, &initOfProgram);
  restartFile(file);
  secondPass(programs[currentIndex], file, initOfProgram);  

  file.close();
}

void assembleManyPrograms(vector<AssembledProgram>& programs, const vector<string>& filePaths) {
  for (unsigned int i = 0; i < programs.size(); i++) {
      readProgram(programs, i, filePaths[i]);
  }
}