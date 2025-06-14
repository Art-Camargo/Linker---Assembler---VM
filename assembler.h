#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "instructions.h"

#include <string>
#include <vector>
#include <fstream>

using namespace std;

int convertMnemonicToOpcode(const string& mnemonic);
bool symbolExistsGlobally(const vector<AssembledProgram>& programs, int currentIndex, const string& name, int symbolType);
void restartFile(ifstream& file);
string trim_left(const string& str);
string trim_right(const string& str);
string trim(const string& str);
ifstream getFile(const string& filePath);

void dispatchVariable(vector<AssembledProgram>& programs, int currentIndex, const string& varName, int value, int lineNumber);
void dispatchLabel(vector<AssembledProgram>& programs, int currentIndex, const string& label, int lineNumber);

void firstPass(vector<AssembledProgram>& programs, int currentIndex, ifstream& file, int *initOfProgram);
void secondPass(AssembledProgram& prog, ifstream& file, int initOfProgram);
void readProgram(vector<AssembledProgram>& programs, int currentIndex, const string& filePath);
void assembleManyPrograms(vector<AssembledProgram>& programs, const vector<string>& filePaths);

#endif // ASSEMBLER_H
