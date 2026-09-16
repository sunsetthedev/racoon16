#ifndef MEMORY_H
#define MEMORY_H

/* Memory Fixed position
0x0000 - 0x7FFF   Program / ROM
0x8000 - 0xDFFF   RAM
0xE000 - 0xEFFF   Stack
0xF000 - 0xFFFF   I/O / Reserved
*/



#include "../include/cpu.h"
#include <stdint.h>

#define MAX_MEMORY 65535


typedef uint8_t Memory[MAX_MEMORY];

// 8bits
uint8_t __read_mem8__(uint16_t addr);
void __write_mem8__(uint16_t val, uint16_t addr);
// 16bits
uint16_t __read_mem16__(uint16_t addr);
void __write_mem16__(uint16_t addr, uint16_t value);


void __clear_mem_all(CPU *cpu);
void __load__program__(const char *binfile);

#endif // MEMORY_H
