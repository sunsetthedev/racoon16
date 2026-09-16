#include "../include/memory.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

Memory __memory; // Main memory

// 8bits
/* Write a data to memory 
   val -> value to add
   addr -> which address of memory to add the value
   no return void 
*/
void __write_mem8__(uint16_t val, uint16_t addr) { __memory[addr] = val; }
/*
  Read a data from memory
  addr -> which address of data return
  return 8 bit memory
  */
uint8_t __read_mem8__(uint16_t addr) { return __memory[addr]; }


// 16bits -- with little-endian
/* 
  Write a data to memory
 */
uint16_t __read_mem16__(uint16_t addr)
{
    return __memory[addr] |
           ((uint16_t)__memory[addr + 1] << 8);
}
// write a data to addr location
// addr -> address to write the data
// value -> value of that data
void __write_mem16__(uint16_t addr, uint16_t value)
{
    __memory[addr]     = value & 0xFF;
    __memory[addr + 1] = value >> 8;
}

void __load__program__(const char *binfile)
{
    FILE *file = fopen(binfile, "rb");
    if (!file) {
        perror("FILE cant open\n");
        exit(1);
    }
    size_t size = fread(__memory, 1, MAX_MEMORY, file);

    if (ferror(file)) {
        perror("Failed to read program");
        fclose(file);
        exit(1);
    }

    fclose(file);

    printf("Loaded %zu bytes\n", size);
}
