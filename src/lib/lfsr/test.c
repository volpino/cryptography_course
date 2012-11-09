#include <stdio.h>
#include "../lfsr.h"

/*
extern uint64_t lfsr_prand_state;
extern uint64_t lfsr_prand_poly;
extern int lfsr_prand_poly_deg;
extern long long unsigned int lfsr_prand_countdown;
*/

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

#define G64_BYTETOBINARYPATTERN "(%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d)"
#define G64_BYTETOBINARY(byte)  \
  (byte & 0x8000000000000000 ? 1 : 0), \
  (byte & 0x4000000000000000 ? 1 : 0), \
  (byte & 0x2000000000000000 ? 1 : 0), \
  (byte & 0x1000000000000000 ? 1 : 0), \
  (byte & 0x800000000000000 ? 1 : 0), \
  (byte & 0x400000000000000 ? 1 : 0), \
  (byte & 0x200000000000000 ? 1 : 0), \
  (byte & 0x100000000000000 ? 1 : 0), \
  (byte & 0x80000000000000 ? 1 : 0), \
  (byte & 0x40000000000000 ? 1 : 0), \
  (byte & 0x20000000000000 ? 1 : 0), \
  (byte & 0x10000000000000 ? 1 : 0), \
  (byte & 0x8000000000000 ? 1 : 0), \
  (byte & 0x4000000000000 ? 1 : 0), \
  (byte & 0x2000000000000 ? 1 : 0), \
  (byte & 0x1000000000000 ? 1 : 0), \
  (byte & 0x800000000000 ? 1 : 0), \
  (byte & 0x400000000000 ? 1 : 0), \
  (byte & 0x200000000000 ? 1 : 0), \
  (byte & 0x100000000000 ? 1 : 0), \
  (byte & 0x80000000000 ? 1 : 0), \
  (byte & 0x40000000000 ? 1 : 0), \
  (byte & 0x20000000000 ? 1 : 0), \
  (byte & 0x10000000000 ? 1 : 0), \
  (byte & 0x8000000000 ? 1 : 0), \
  (byte & 0x4000000000 ? 1 : 0), \
  (byte & 0x2000000000 ? 1 : 0), \
  (byte & 0x1000000000 ? 1 : 0), \
  (byte & 0x800000000 ? 1 : 0), \
  (byte & 0x400000000 ? 1 : 0), \
  (byte & 0x200000000 ? 1 : 0), \
  (byte & 0x100000000 ? 1 : 0), \
  (byte & 0x80000000 ? 1 : 0), \
  (byte & 0x40000000 ? 1 : 0), \
  (byte & 0x20000000 ? 1 : 0), \
  (byte & 0x10000000 ? 1 : 0), \
  (byte & 0x8000000 ? 1 : 0), \
  (byte & 0x4000000 ? 1 : 0), \
  (byte & 0x2000000 ? 1 : 0), \
  (byte & 0x1000000 ? 1 : 0), \
  (byte & 0x800000 ? 1 : 0), \
  (byte & 0x400000 ? 1 : 0), \
  (byte & 0x200000 ? 1 : 0), \
  (byte & 0x100000 ? 1 : 0), \
  (byte & 0x80000 ? 1 : 0), \
  (byte & 0x40000 ? 1 : 0), \
  (byte & 0x20000 ? 1 : 0), \
  (byte & 0x10000 ? 1 : 0), \
  (byte & 0x8000 ? 1 : 0), \
  (byte & 0x4000 ? 1 : 0), \
  (byte & 0x2000 ? 1 : 0), \
  (byte & 0x1000 ? 1 : 0), \
  (byte & 0x800 ? 1 : 0), \
  (byte & 0x400 ? 1 : 0), \
  (byte & 0x200 ? 1 : 0), \
  (byte & 0x100 ? 1 : 0), \
  (byte & 0x80 ? 1 : 0), \
  (byte & 0x40 ? 1 : 0), \
  (byte & 0x20 ? 1 : 0), \
  (byte & 0x10 ? 1 : 0), \
  (byte & 0x8 ? 1 : 0), \
  (byte & 0x4 ? 1 : 0), \
  (byte & 0x2 ? 1 : 0), \
  (byte & 0x1 ? 1 : 0)

void g64_print(uint64_t a) {
    printf(G64_BYTETOBINARYPATTERN, G64_BYTETOBINARY(a));
}



void lfsr_test() {
  int i;
  lfsr reg;
  reg.poly = 0xd;
  reg.state = 0x5;
  reg.poly_deg = 3;
  uint8_t buffer[50];
  lfsr_rotate(&reg, buffer, 400);

  for (i=0; i<10; i++) {
    g8_print(buffer[i]); printf("\n");
  }
}

void lfsr_prandom_status() {
  /*
  printf("state: "); g64_print(lfsr_prand_state); printf("\n");
  printf("poly:  "); g64_print(lfsr_prand_poly); printf("\n");
  printf("deg: %i\n", lfsr_prand_poly_deg);
  printf("countdown: %lli\n", lfsr_prand_countdown);
  */
}

/*
void lfsr_prandom_test() {
  int i;
  lfsr_prandom_status();
  lfsr_pseudorand_init_8();
  lfsr_prandom_status();
  printf("Getting random...\n");
  lfsr_pseudorand_random_seed_8();
  lfsr_prandom_status();

  for(i=0; i<255; i++){
    printf("%i", lfsr_pseudorand_bit());
  }
  printf("\n");
  lfsr_prandom_status();
  for(i=0; i<255; i++){
    printf("%i", lfsr_pseudorand_bit());
  }
  printf("\n");
  lfsr_prandom_status();
}
*/

int main() {
  lfsr_test();

  return 0;
}
