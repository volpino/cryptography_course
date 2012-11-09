#include "../lfsr.h"
#include <stdio.h>



void lfsr_pseudorand_init(uint64_t poly, int poly_deg);
void lfsr_pseudorand_seed(uint64_t state);
void lfsr_pseudorand_init_8();
void lfsr_pseudorand_random_seed_8();
void lfsr_prand_countdown_check();
uint8_t lfsr_pseudorand_bit();
uint8_t lfsr_pseudorand_byte();




static uint64_t lfsr_prand_state;
static uint64_t lfsr_prand_poly;
static int lfsr_prand_poly_deg;
static long long unsigned int lfsr_prand_countdown;

/*
uint64_t lfsr_prand_state;
uint64_t lfsr_prand_poly;
int lfsr_prand_poly_deg;
long long int lfsr_prand_countdown;
*/
/* return 2^exp */
static long long int lint_exp2(int exp) { 
  int i;
  long long unsigned int res = 1;

  if (exp < 0) {
    fprintf(stderr, "ERROR: function lint_exp2 in lfsr.c: negative exponent.\n");
  }

  for(i=0; i<exp; i++) {
    res *= ((long long unsigned int) 2);
  }
  return res;
}

void lfsr_pseudorand_init(uint64_t poly, int poly_deg) {
  lfsr_prand_poly = poly;
  lfsr_prand_poly_deg = poly_deg;
}

void lfsr_pseudorand_seed(uint64_t state) {
  lfsr_prand_state = state;

  lfsr_prand_countdown = lint_exp2(lfsr_prand_poly_deg) - 1;
}

void lfsr_pseudorand_init_8() {
  /* x^8 + x^4 + x^3 + x^2 + 1 */
  lfsr_pseudorand_init(0x11d, 8);
}

void lfsr_pseudorand_random_seed_8() {
  uint8_t byte = 0;
  FILE* f = fopen("/dev/random", "r");
  if (f != NULL) {
    while (byte == 0) {
      fscanf(f, "%c", &byte);
    }
    lfsr_pseudorand_seed((uint64_t) byte);
    fclose(f);
  }
}

void lfsr_prand_countdown_check() {
  if (lfsr_prand_countdown < 0) {
    fprintf(stderr, "WARNING: lfsr pseudorandom overflow.\n");
  }
}

uint8_t lfsr_pseudorand_bit() {
  uint8_t bit;
  lfsr_prand_state = \
    lfsr(lfsr_prand_poly, lfsr_prand_poly_deg, lfsr_prand_state, &bit, 1);
  lfsr_prand_countdown -= 1;
  lfsr_prand_countdown_check();
  return bit;
}

uint8_t lfsr_pseudorand_byte() {
  uint8_t byte;
  lfsr_prand_state = \
    lfsr(lfsr_prand_poly, lfsr_prand_poly_deg, lfsr_prand_state, &byte, 8);
  lfsr_prand_countdown -= 8;
  lfsr_prand_countdown_check();
  return byte;
}
