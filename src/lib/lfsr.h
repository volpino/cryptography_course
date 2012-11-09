#ifndef _LFSR_H_
#define _LFSR_H_

#include <stdint.h>

/*
   - poly is a polynome, of degree poly_deg
     least significant bit is coeff. of x^0
   - state is the initial state, only matter least significant poly_deg bits
*/
typedef struct {
  uint64_t poly;
  uint64_t state;
  int poly_deg;
} lfsr;

/*
   - output is a out_bits/8 array of bytes, where output will be stored,
     from index 0 to out_len-1, each byte from most to least significant bit
     (i.e. if out_len is not multiple of 8, least significant bits in the
     last byte of output are just noise)
   updates the state properly
*/
void lfsr_rotate(lfsr* reg, uint8_t* output, int out_bits);

#endif
