#include "../bunny24_prng.h"
#include <stdio.h>

int main() {
  int i;
  uint8_t seed[3] = {0x01, 0x02, 0x03};
  uint8_t out[100];

  bunny24_prng(seed, 3, out, 100);

  for (i=0; i<100; i++) {
    printf("%x ", out[i]);
  }
  printf("\n");

  return 0;
}
