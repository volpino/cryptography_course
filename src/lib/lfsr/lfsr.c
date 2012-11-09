#include "../lfsr.h"
#include <stdlib.h>


/* - poly is a polynome, of degree poly_deg
     least significant bit is coeff. of x^0
   - state is the initial state, only matter least significant poly_deg bits
   - output is a out_bits/8 array of bytes, where output will be stored,
     from index 0 to out_len-1, each byte from most to least significant bit
     (i.e. if out_len is not multiple of 8, least significant bits in the
     last byte of output are just zeroes)
     if output is NULL, no output will be given.
   returns the new state where the lfsr arrived after outputting the bits
 */
void lfsr_rotate(lfsr* reg, uint8_t* output, int out_bits) {
  uint64_t poly = reg->poly >> 1;
  int i, j, out_len;
  uint64_t temp, bit;

  if (output != NULL) {
    out_len = (out_bits/8) + ((out_bits%8!=0) ? 1 : 0);
    for (i=0; i<out_len; i++) {
      output[i] = 0;
    }
  }

  for (i=0; i<out_bits; i++) {
    /* compute output bit */
    temp = poly & reg->state;
    bit = 0;
    for (j=0; j<reg->poly_deg; j++) {
      bit ^= temp;
      temp >>= 1;
    }
    bit &= 1;
    /* save output bit in output buffer */
    if (output != NULL) {
      output[i/8] ^= (bit << (7 - i % 8));
    }
    /* update reg->state */
    reg->state <<= 1;
    reg->state |= bit;
  }
  /* should we reg->state &= mask;  ?? */
}
