#include "../bunny24_prng.h"
#include <stdio.h>

//#define N_BIT 2097152
#define N 16777216

uint8_t out[N];
int hash[N];


int bytes_to_int(uint8_t* bytes) {
  int i;
  int res = bytes[0];
  for (i=1; i<3; i++) {
    res <<= 8;
    res += bytes[i];
  }
  return res;
}

int main() {
  int i;
  uint8_t seed[3] = {0x01, 0x02, 0x03};

  for (i=0; i<N; i++) {
    hash[i] = -1;
  }

  printf("It is working, or is it!\n");
  bunny24_prng(seed, 3, out, N);

  for (i=0; i<N; i+=3) {
    int tmp = bytes_to_int(&out[i]);
    if (hash[tmp] != -1) {
      printf("DLF hash[tmp]=%d\n", hash[tmp]);
      printf("DLF i=%d\n", i);
    }
    else {
      hash[tmp] = i;
    }
  }

  return 0;
}
