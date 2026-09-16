#include "../include/debugger.h"
#include "../include/cpu.h"
#include "../include/memory.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Debug : To print all registers and its values
void printREG(CPU *cpu) {
  printf("Registers\n");
  printf("-------------------------\n");
  printf("| Regs\t|\tValue\t|\n");
  printf("-------------------------\n");
  for (int i = 0x0000; i < 0x0008; i++) {
    printf("| s%d\t|\t0x%04X\t|\n", i, cpu->regs[i]);
  }

  printf("-------------------------\n");
}
void printMEM(BYTE startmem, BYTE endmem) {
  PC i = startmem;
  printf("\n-------------------------\n");
  while (i <= endmem) {
    printf("| 0x%04X : ", i);
    PC temp = i;
    while (temp != (i + 4)) {
      uint8_t val = __read_mem16__(temp++);
      printf("%02X ", val);
    }

    printf("\t|\n");

    i += 4;
  }
  printf("-------------------------\n\n");
}

void printCPUSpec(CPU *cpu) {
  printf("\t====CPU Spec====\t\n");
  printf("-------------------------\n");
  printf("| PC = 0x%04X\t\t|\n", cpu->Pc);
  printf("| SP = 0x%04X\t\t|\n", cpu->sp);
  printf("-------------------------\n");
  printf("Flags\n");
  printf("-------------------------\n");
  printf("| Z = %d\t\t\t|\n", cpu->Z);
  printf("| N = %d\t\t\t|\n", cpu->N);
  printf("| C = %d\t\t\t|\n", cpu->C);
  printf("-------------------------\n\n");
  printREG(cpu);
}

void debugShell(CPU *cpu) {
  bool finished = false;
  bool stillprint;
  while (!finished && !stillprint) {
    printf("(dbg)> ");
    char buff[1024];
    fgets(buff, sizeof(buff), stdin);
    if (strcmp(buff, "step\n") == 0) {
      printCPUSpec(cpu);
      finished = true;
    }
    if (strcmp(buff, "mem\n") == 0) {
      printMEM(0x0000, 0x0100);
    }
    if (strcmp(buff, "regs\n") == 0) {
      printREG(cpu);
    }
    if (strcmp(buff, "quit\n") == 0) {
      finished = true;
      stillprint = true;
    }
  }
}
