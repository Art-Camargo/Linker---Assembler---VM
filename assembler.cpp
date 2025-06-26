#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
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
    if (mnemonic == "R") return R;
    if (mnemonic == "STP") return STP;
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
  size_t start = str.find_first_not_of(pattern);
  if (start == string::npos) return "";
  return str.substr(start);
}

string trim_right(const string& str) {
  const string pattern = " \f\n\r\t\v";
  size_t end = str.find_last_not_of(pattern);
  if (end == string::npos) return "";
  return str.substr(0, end + 1);
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

void copyStringToCharArray(const string& str, char* destination, size_t maxLength) {
  strncpy(destination, str.c_str(), maxLength - 1);
  destination[maxLength - 1] = '\0';  
}

int registerToCode(const string& reg) {
  if (reg == "A0") return 0;
  if (reg == "A1") return 1;
  if (reg == "A2") return 2;
  if (reg == "A3") return 3;

  cout << "Error: Invalid register '" << reg << "'" << endl;
  exit(EXIT_FAILURE);
}

int getValueByNameIgnoringType(const vector<AssembledProgram>& programs, int currentIndex, const string& name) {
  for (int i = 0; i < currentIndex; ++i) {
      for (const auto& entry : programs[i].symbolTable) {
          if (entry.name == name) {
              return entry.memoryAddress;
          }
      }
  }
  cout << "Warnning: Variable or label '" << name << "' not found. Returning -1." << endl;
  return -1;
}



void dispatchInstruction(vector<AssembledProgram>& programs, int currentIndex, const string& opcodeStr, const string& op1, const string& op2, const string& op3, int position) {
  
  if (programs[currentIndex].memory.size() <= (size_t)position) {
    programs[currentIndex].memory.resize(position + 1);
  }
  
  int opcode = convertMnemonicToOpcode(opcodeStr);
  Instruction instr; 
  instr.opcode = opcode;
  if (opcodeStr == "ADD" || opcodeStr == "SUB" || opcodeStr == "MUL" || opcodeStr == "DIV") {
      instr.operand1 = registerToCode(op1);
      instr.operand2 = registerToCode(op2);
      instr.operand3 = registerToCode(op3);
  } else if (opcodeStr == "MV") {
      instr.operand1 = registerToCode(op1);
      instr.operand2 = getValueByNameIgnoringType(programs, position, op2);
  } else if (opcodeStr == "ST") {
      instr.operand1 = getValueByNameIgnoringType(programs, position, op1);
      instr.operand2 = registerToCode(op2);
  } else if (opcodeStr == "JMP") {
      instr.operand1 = getValueByNameIgnoringType(programs, position, op1); 
  } else if (opcodeStr == "JLT" || opcodeStr == "JGT" || opcodeStr == "JEQ") {
      instr.operand1 = registerToCode(op1);
      instr.operand2 = registerToCode(op2);
      instr.operand3 = getValueByNameIgnoringType(programs, position, op3); 
  } else if (opcodeStr == "R" || opcodeStr == "W") {
      instr.operand1 = getValueByNameIgnoringType(programs, position, op1);
  } else if (opcodeStr == "STP") {
      instr.operand1 = -1;
      instr.operand2 = -1;
      instr.operand3 = -1;
  } else {
      cout << "Error: Unknown instruction '" << opcodeStr << "'" << endl;
      exit(EXIT_FAILURE);
  }

  if (opcodeStr != "ADD" && opcodeStr != "SUB" && opcodeStr != "MUL" && opcodeStr != "DIV" && opcodeStr != "JLT" && opcodeStr != "JGT" && opcodeStr != "JEQ") {
      instr.operand3 = -1; 
  } 
  
  if (opcodeStr == "R" || opcodeStr == "W" || opcodeStr == "JMP") {
      instr.operand2 = -1; 
  }

  MemoryCell cell;
  cell.type = MEM_INSTRUCTION;
  cell.instr = instr;

  if (opcodeStr == "JMP"  || opcodeStr == "W" || opcodeStr == "ST" || opcodeStr == "R") {
    cell.labelToLinker = op1;
    cell.operandToLinker = 1;
  } else if (opcodeStr == "MV") {
    cell.labelToLinker = op2;
    cell.operandToLinker = 2;
  } else if (opcodeStr == "JLT" || opcodeStr == "JGT" || opcodeStr == "JEQ") {
    cell.labelToLinker = op3;
    cell.operandToLinker = 3;
  } else {
    cell.labelToLinker = ""; 
    cell.operandToLinker = -1;
  }
  
  programs[currentIndex].memory[position] = cell;
}


void secondPass(vector<AssembledProgram>& programs, int currentIndex, ifstream& file, int initOfProgram) {
    string line;
    int lineNumber = 0;

    while (getline(file, line) && lineNumber < MEMORY_SIZE) {
      if (!line.empty()) {
        istringstream iss(trim(line));
        string opcode, op1, op2, op3;
        iss >> opcode >> op1 >> op2 >> op3;
        
        if (convertMnemonicToOpcode(opcode) != NOP) {
          dispatchInstruction(programs, currentIndex, opcode, op1, op2, op3, initOfProgram++);
        }
        lineNumber++;
      }

    }
}

bool variableExistsInCurrentProgram(const vector<AssembledProgram>& programs, int currentIndex, const string& varName) {
  const AssembledProgram& prog = programs[currentIndex];
  for (const auto& entry : prog.symbolTable) {
      if (entry.name == varName && entry.type == SYMBOL_VARIABLE) {
          return true;
      }
  }

  return false;
}

void dispatchVariable(vector<AssembledProgram>& programs, int currentIndex, const string& varName, int value, int lineNumber, int scopeType) {
  if (variableExistsInCurrentProgram(programs, currentIndex, varName)) {
    cout << "Error: Variable '" << varName << "' already declared in the current program." << endl;
    exit(EXIT_FAILURE);
  }

  AssembledProgram& prog = programs[currentIndex];

  int memoryPosition = prog.memory.size();

  ScopeType scope = static_cast<ScopeType>(scopeType);

  MemoryCell cell;
  cell.type = MEM_DATA;
  cell.value = value;
  cell.scope = scope;
  prog.memory.push_back(cell);

  SymbolTableEntry entry;
  entry.name = varName;
  entry.type = SYMBOL_VARIABLE;
  entry.memoryAddress = memoryPosition; 
  entry.scope = scope;
  prog.symbolTable.push_back(entry);
}


void dispatchLabel(vector<AssembledProgram>& programs, int currentIndex, const string& label, int lineNumber) {
  string labelName = label.substr(0, label.size() - 1);

  AssembledProgram& prog = programs[currentIndex];

  SymbolTableEntry entry;
  entry.name = labelName;
  entry.type = SYMBOL_LABEL;
  entry.memoryAddress = lineNumber;
  entry.scope = SCOPE_GLOBAL;
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
          string token, varName, value, scope;
          iss >> token >> varName >> value >> scope;
          cout << "Processing line " << lineNumber << ": " << line << endl;
          if (token.back() == ':' && token.size() > 1) {
              dispatchLabel(programs, currentIndex, token, instructionCount);
          } else if (token == "WORD") {
              if (varName.empty()) {
                  cout << "Error: Invalid WORD declaration at line " << lineNumber << endl;
                  exit(EXIT_FAILURE);
              }
              int scopeType = scope == "global" ? SCOPE_GLOBAL : SCOPE_LOCAL;
              dispatchVariable(programs, currentIndex, varName, stoi(value), wordCount++, scopeType);
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
  secondPass(programs, currentIndex, file, initOfProgram);  

  file.close();
}

void assembleManyPrograms(vector<AssembledProgram>& programs, const vector<string>& filePaths) {
  for (unsigned int i = 0; i < programs.size(); i++) {
      readProgram(programs, i, filePaths[i]);
  }
}