#include "../include/cpu.h"
#include "../include/memory.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

CPU *_init_CPU() {
  CPU *cpu = malloc(sizeof(CPU));
  if (!cpu) {
    perror("CPU iC");
    exit(1);
  }

  cpu->regs[0x00] = 0x0000;
  cpu->regs[0x01] = 0x0000;
  cpu->regs[0x002] = 0x0000;
  cpu->regs[0x003] = 0x0000;
  cpu->regs[0x004] = 0x0000;
  cpu->regs[0x005] = 0x0000;
  cpu->regs[0x006] = 0x0000;
  cpu->regs[0x007] = 0x0000;

  cpu->Pc = 0x0000;
  cpu->sp = 0xEFFF;

  cpu->Z = 0;
  cpu->C = 0;
  cpu->N = 0;

  cpu->isrunning = true;

  return cpu;
}

void CPU_CYCLE(CPU *cpu) {
    uint16_t opcode = fetch(cpu);
    INSTRUCT ins = decode(opcode);
    execute(cpu, ins);
}

// Helper function
// CPU Stages

// Get the program counter aka next instruction
BYTE fetch(CPU *cpu) { return __read_mem8__(cpu->Pc++); }

// Decode the instruction
INSTRUCT decode(BYTE opcode) {
  switch (opcode) {
  case I_LOAD:
    return I_LOAD;
  case I_STORE:
    return I_STORE;
  case I_LOADI:
    return I_LOADI;
  case I_ADDI:
    return I_ADDI;
  case I_ADD:
    return I_ADD;
  case I_SUB:
    return I_SUB;
  case I_MUL:
    return I_MUL;
  case I_DIV:
    return I_DIV;
  case I_INC:
    return I_INC;
  case I_DEC:
    return I_DEC;
  case I_CMP:
    return I_CMP;
  case I_JMP:
    return I_JMP;
  case I_JE:
    return I_JE;
  case I_JNE:
    return I_JNE;
  case I_JG:
    return I_JG;
  case I_JL:
    return I_JL;
  case I_PUSH:
    return I_PUSH;
  case I_POP:
    return I_POP;
  case I_CALL:
    return I_CALL;
  case I_RET:
    return I_RET;
  case I_NOP:
    return I_NOP;
  case I_HLT:
    return I_HLT;
  default:
    return I_NOP;
  }
}

// Execute the instruction
void execute(CPU *cpu, INSTRUCT ins) {
  switch (ins) {

  case I_LOADI: { // Example : LOAD
    uint8_t reg = __read_mem8__(cpu->Pc++);
    uint16_t value = ((uint16_t)__read_mem8__(cpu->Pc++) << 8) |
                     (uint16_t)__read_mem8__(cpu->Pc++);

    cpu->regs[reg] = value;
  } break;

  case I_STORE: {
    uint8_t reg = __read_mem8__(cpu->Pc++);
    uint16_t value = cpu->regs[reg];
    uint16_t addr = ((uint16_t)__read_mem8__(cpu->Pc++) << 8) |
                    (uint16_t)__read_mem8__(cpu->Pc++);

    __write_mem16__(addr, value);
  } break;

  case I_LOAD: {
    uint8_t reg = __read_mem8__(cpu->Pc++);
    uint16_t addr = ((uint16_t)__read_mem8__(cpu->Pc++) << 8) |
                    (uint16_t)__read_mem8__(cpu->Pc++);

    cpu->regs[reg] = __read_mem16__(addr);

  } break;

  case I_ADDI: {
    uint8_t reg = __read_mem8__(cpu->Pc++);
    cpu->Pc++;
    // uint16_t value = ((uint16_t)__read_mem8__(cpu->Pc++) << 8) |
    // (uint16_t)__read_mem8__(cpu->Pc++);
    uint16_t value = (uint16_t)__read_mem8__(cpu->Pc++);

    cpu->regs[reg] += value;

  } break;

  case I_ADD: {
    uint8_t reg1 = __read_mem8__(cpu->Pc++);
    cpu->Pc++;
    uint8_t reg2 = __read_mem8__(cpu->Pc++);

    cpu->regs[reg1] += cpu->regs[reg2];

  } break;

  case I_SUB: {
    uint8_t reg1 = __read_mem8__(cpu->Pc++);
    cpu->Pc++;
    uint8_t reg2 = __read_mem8__(cpu->Pc++);

    cpu->regs[reg1] -= cpu->regs[reg2];
  } break;

  case I_MUL: {
    uint8_t reg1 = __read_mem8__(cpu->Pc++);
    cpu->Pc++;
    uint8_t reg2 = __read_mem8__(cpu->Pc++);

    cpu->regs[reg1] *= cpu->regs[reg2];
  } break;

  case I_DIV: {
    uint8_t reg1 = __read_mem8__(cpu->Pc++);
    cpu->Pc++;
    uint8_t reg2 = __read_mem8__(cpu->Pc++);

    cpu->regs[reg1] /= cpu->regs[reg2];
  } break;

  case I_INC: {
    cpu->Pc++;
    cpu->Pc++;
    uint8_t reg = __read_mem8__(cpu->Pc++);
    cpu->regs[reg]++;

  } break;

  case I_DEC: {
    cpu->Pc++;
    cpu->Pc++;
    uint8_t reg = __read_mem8__(cpu->Pc++);
    cpu->regs[reg]--;
  } break;

  case I_CMP: {

    uint8_t reg1 = __read_mem8__(cpu->Pc++);
    cpu->Pc++;
    uint8_t reg2 = __read_mem8__(cpu->Pc++);
    uint32_t res = (uint32_t)cpu->regs[reg1] - (uint32_t)cpu->regs[reg2];
    cpu->Z = (res == 0);
    cpu->N = (res < 0);

  } break;

  case I_JMP: {
    cpu->Pc++;
    uint16_t addr = ((uint16_t)__read_mem8__(cpu->Pc++) << 8) |
                    (uint16_t)__read_mem8__(cpu->Pc++);
    cpu->Pc = addr;

  } break;

  case I_JE: {
    cpu->Pc++;
    uint16_t addr = ((uint16_t)__read_mem8__(cpu->Pc++) << 8) |
                    (uint16_t)__read_mem8__(cpu->Pc++);
    if (cpu->Z == 1) {
      cpu->Pc = addr;
    }
  } break;

  case I_JNE: {
    cpu->Pc++;
    uint16_t addr = ((uint16_t)__read_mem8__(cpu->Pc++) << 8) |
                    (uint16_t)__read_mem8__(cpu->Pc++);
    if (cpu->Z == 0) {
      cpu->Pc = addr;
    }
  } break;
  case I_JL: {
    cpu->Pc++;
    uint16_t addr = ((uint16_t)__read_mem8__(cpu->Pc++) << 8) |
                    (uint16_t)__read_mem8__(cpu->Pc++);
    if (cpu->N == 0) {
      cpu->Pc = addr;
    }
  } break;
  case I_JG: {
    cpu->Pc++;
    uint16_t addr = ((uint16_t)__read_mem8__(cpu->Pc++) << 8) |
                    (uint16_t)__read_mem8__(cpu->Pc++);
    if (cpu->N == 1) {
      cpu->Pc = addr;
    }
  } break;
  case I_PUSH: {
    uint8_t reg = __read_mem8__(cpu->Pc++);
    uint16_t value = cpu->regs[reg];

    __write_mem8__(value & 0xFF, cpu->sp--);
    __write_mem8__(value >> 8, cpu->sp--);

    // cpu->Pc++;cpu->Pc++;
  } break;
  case I_POP: {
    uint8_t reg = __read_mem8__(cpu->Pc++);
    uint8_t high = __read_mem8__(++cpu->sp);
    uint8_t low = __read_mem8__(++cpu->sp);

    cpu->regs[reg] = (high << 8) | low;
    cpu->Pc += 2;
  } break;
  case I_CALL: {
    cpu->Pc++;
    uint16_t addr = ((uint16_t)__read_mem8__(cpu->Pc++) << 8) |
                    (uint16_t)__read_mem8__(cpu->Pc++);
    uint16_t pcaddr = cpu->Pc;
    __write_mem8__(pcaddr & 0xFF, cpu->sp--);
    __write_mem8__(pcaddr >> 8, cpu->sp--);
    cpu->Pc = addr;

  } break;
  case I_RET: {
    uint8_t high = __read_mem8__(++cpu->sp);
    uint8_t low = __read_mem8__(++cpu->sp);
    cpu->Pc = (high << 8) | low;

  } break;

  case I_HLT: {
    cpu->Pc++;
    cpu->Pc++;
    cpu->Pc++;
    cpu->isrunning = false;
  } break;
  case I_NOP: {

  } break;

  default:
    fprintf(stderr, "Unknown instruction: %d\n", ins);
    exit(1);
    break;
  }
}
