#include "../bunny24_prng.h"
#include <stdio.h>

//#define N_BIT 2097152
#define N 16777216*3
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
  uint8_t seed_zero[3] = {0x00, 0x00, 0x00};
  uint8_t iv[3];

  for (i=0; i<N; i++) {
    hash[i] = -1;
  }

  bunny24_prng(seed, 3, NULL, out, 100);
  for (i=0; i<100; i++) {
    printf("%x ", out[i]);
  }
  printf("\n");

  bunny24_prng(seed, 3, NULL, out, 30);
  for (i=0; i<30; i++) {
    printf("%x ", out[i]);
  }
  iv[0] = out[27];
  iv[1] = out[28];
  iv[2] = out[29];
  bunny24_prng(seed_zero, 3, iv, out, 70);
  for (i=0; i<70; i++) {
    printf("%x ", out[i]);
  }
  printf("\n");


  bunny24_prng(seed, 3, NULL, out, N);

  for (i=0; i<N; i+=3) {
    int tmp = bytes_to_int(&out[i]);
    if (hash[tmp] != -1) {
      printf("Loop!\nPrevious was at %d\n", hash[tmp]);
      printf("Current %d\n", i);
      return 0;
    }
    else {
      hash[tmp] = i;
    }
  }

  return 0;
}
