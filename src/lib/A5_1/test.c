#include "../lfsr.h"
#include "A5_1_internals.h"
#include <stdlib.h>

#include <stdio.h>

#define G8_BYTETOBINARYPATTERN "%d,%d,%d,%d,%d,%d,%d,%d,"
#define G8_BYTETOBINARY(byte)  \
  (byte & 0x80 ? 1 : 0), \
  (byte & 0x40 ? 1 : 0), \
  (byte & 0x20 ? 1 : 0), \
  (byte & 0x10 ? 1 : 0), \
  (byte & 0x08 ? 1 : 0), \
  (byte & 0x04 ? 1 : 0), \
  (byte & 0x02 ? 1 : 0), \
  (byte & 0x01 ? 1 : 0)

void g8_print(uint8_t a) {
  printf(G8_BYTETOBINARYPATTERN, G8_BYTETOBINARY(a));
}

const int num_test = 10;

int main() {
  int i;
  uint8_t k[8] = {0x48, 0xc4, 0xa2, 0xe6, 0x91, 0xd5, 0xb3, 0xf7};
  uint8_t buff[num_test];

  /*a5_1(k, buff, num_test * 8);*/
  /*maj5(k, buff, num_test * 8);*/
  all5(k, buff, num_test * 8);

  for(i=0; i<num_test; i++) {
    g8_print(buff[i]);
  }
  printf("\b\n");
  return 0;
}
