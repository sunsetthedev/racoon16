#include <stdint.h>
#include <stdio.h>

int main(int argc, char **argv) {
  printf("Hello From Lab\n");
  uint16_t reg1 = 65535;
  printf("int : %d\nhex : %X\n", reg1, reg1);
  uint8_t mem[1024];
  mem[0x00] = 12;
  printf("%X\n", mem[0x00]);
  int nums[1223];
  nums[0x00] = 12;
  printf("%d\n", nums[0x00]);
  uint16_t testing = 0x0000;
  printf("testing first : %04X",testing);
  testing++;
  printf("testing later : %04X",testing);
}
