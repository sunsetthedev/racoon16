#ifndef DEBUGGER_H
#define DEBUGGER_H



#include <stdio.h>
#include <stdint.h>
#include "../include/cpu.h"

void printREG(CPU *cpu);
void printMEM(BYTE startmem,BYTE endmem);
void printCPUSpec(CPU *cpu);

void debugShell(CPU *cpu);
#endif // DEBUGGER_H
