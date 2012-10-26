#include <stdio.h>
#include "../lfsr.h"

#define G8_BYTETOBINARYPATTERN "(%d,%d,%d,%d,%d,%d,%d,%d)"
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

int main() {
  int i;
  uint64_t poly = 0xd;
  uint8_t buffer[50];
  lfsr(poly, 3, 0x1, buffer, 400);

  for (i=0; i<50; i++) {
    g8_print(buffer[i]); printf("\n");
  }
  return 0;
}
