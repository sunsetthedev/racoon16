#include "../include/cpu.h"
#include "../include/emu.h"
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "No input file.<sunflower> <output.bin>");
    exit(1);
  }

  CPU *cpu = _init_CPU();

  if (!cpu) {
    perror("CPU");
    return 1;
  }
  Running(cpu, argv[1],true);
  return 0;
}
