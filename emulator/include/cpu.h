#ifndef CPU_H
#define CPU_H

#include <stdbool.h>
#include <stdint.h>
typedef uint16_t BYTE;
typedef BYTE PC;
typedef BYTE SP;
typedef uint8_t Flag ;

typedef struct {
  bool isrunning;

  uint16_t regs[8];

  PC Pc;
  SP sp;

  Flag Z;
  Flag C;
  Flag N;

} CPU;

typedef enum {
  I_NOP = 0xFE,
  I_HLT = 0xFF,
  I_LOAD = 0x08,
  I_STORE = 0x09,
  I_LOADI = 0x0A,
  I_ADD = 0x11,
  I_ADDI = 0x10,
  I_SUB = 0x12,
  I_MUL = 0x13,
  I_DIV = 0x14,
  I_INC = 0x15,
  I_DEC = 0x16,
  I_CMP = 0x20,
  I_JMP = 0x30,
  I_JE = 0x31,
  I_JNE = 0x32,
  I_JG = 0x33,
  I_JL = 0x34,
  I_PUSH = 0x40,
  I_POP = 0x41,
  I_CALL = 0x42,
  I_RET = 0x43,
} INSTRUCT;

CPU *_init_CPU();
BYTE fetch(CPU *cpu);
INSTRUCT decode(BYTE opcode);
void execute(CPU *cpu,INSTRUCT ins);
void CPU_CYCLE(CPU *cpu);

#endif // CPU_H
