#ifndef _LFSR_H_
#define _LFSR_H_

#include <stdint.h>

/* - poly is a polynome, of degree poly_deg
     least significant bit is coeff. of x^0
   - state is the initial state, only matter least significant poly_deg bits
   - output is a out_bits/8 array of bytes, where output will be stored,
     from index 0 to out_len-1, each byte from least to most significant bit
   returns the new state where the lfsr arrived after outputting the bits
 */

void lfsr_pseudorand_init(uint64_t poly, int poly_deg);
void lfsr_pseudorand_seed(uint64_t state);
void lfsr_pseudorand_init_8();
void lfsr_pseudorand_random_seed_8();
void lfsr_prand_countdown_check();
uint8_t lfsr_pseudorand_bit();
uint8_t lfsr_pseudorand_byte();

typedef struct {
  uint64_t poly;
  uint64_t state;
  int poly_deg;
} lfsr;

void lfsr_rotate(lfsr* reg, uint8_t* output, int out_bits);
uint8_t lfsr_get_bit(lfsr* reg, int index);
void lfsr_set_bit(lfsr* reg, int index, uint8_t value);


#endif
