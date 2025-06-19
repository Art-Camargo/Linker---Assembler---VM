#ifndef LINKER_H
#define LINKER_H

#include <vector>
#include <string>
#include <iostream>
#include "instructions.h"

using namespace std;

void linkerFirstPass(const vector<AssembledProgram>& programs, LinkedProgram& linkedProgram);
void printSymbolTable(const LinkedProgram& linkedProgram);
void linkerSecondPass(const vector<AssembledProgram>& programs, LinkedProgram& linkedProgram);
void printDataMemory(const LinkedProgram& linkedProgram);
void printInstructionMemory(const LinkedProgram& linkedProgram);

#endif