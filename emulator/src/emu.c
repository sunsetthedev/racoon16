#include "../include/emu.h"
#include "../include/debugger.h"
#include "../include/memory.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// TODO: Add TUI Later

// Main Emulator Loop TODO: Change the fucking name later
void Running(CPU *cpu, const char *binfile, bool debugmode) {
  if (!cpu) {
    fprintf(stderr, "CPU error in Running\n");
    exit(1);
  }
  __load__program__(binfile);
  while (cpu->isrunning) {
    CPU_CYCLE(cpu);
    if (debugmode) {
      debugShell(cpu);
    }
  }
}
