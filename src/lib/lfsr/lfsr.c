#include "../lfsr.h"

/* - poly is a polynome, of degree poly_deg
     least significant bit is coeff. of x^0
   - state is the initial state, only matter least significant poly_deg bits
   - output is a out_bits/8 array of bytes, where output will be stored,
     from index 0 to out_len-1, each byte from least to most significant bit
   returns the new state where the lfsr arrived after outputting the bits
 */
uint64_t lfsr(uint64_t poly, int poly_deg, uint64_t state,
              uint8_t* output, int out_bits)
{
  int i, j, out_len;
  uint64_t temp, bit;

  poly >>= 1;
  out_len = (out_bits/8) + ((out_bits%8!=0) ? 1 : 0);
  for (i=0; i<out_len; i++) {
    output[i] = 0;
  }

  for (i=0; i<out_bits; i++) {
    /* compute ot bit */
    temp = poly & state;
    bit = 0;
    for (j=0; j<poly_deg; j++) {
      bit ^= temp;
      temp >>= 1;
    }
    bit &= 1;
    /* save output bit in output buffer */
    output[i/8] ^= (bit << (i%8));
    /* update state */
    state <<= 1;
    state |= bit;
  }
  return state;
}
