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
uint64_t lfsr(uint64_t poly, int poly_deg, uint64_t state,
              uint8_t* output, int out_bits);



void lfsr_pseudorand_init(uint64_t poly, int poly_deg);
void lfsr_pseudorand_seed(uint64_t state);
void lfsr_pseudorand_init_8();
void lfsr_pseudorand_random_seed_8();
void lfsr_prand_countdown_check();
uint8_t lfsr_pseudorand_bit();
uint8_t lfsr_pseudorand_byte();




#endif
