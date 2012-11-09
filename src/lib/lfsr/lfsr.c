#include "../lfsr.h"


/* - poly is a polynome, of degree poly_deg
     least significant bit is coeff. of x^0
   - state is the initial state, only matter least significant poly_deg bits
   - output is a out_bits/8 array of bytes, where output will be stored,
     from index 0 to out_len-1, each byte from least to most significant bit
   returns the new state where the lfsr arrived after outputting the bits
 */
void lfsr_rotate(lfsr* reg, uint8_t* output, int out_bits) {
  uint64_t poly = reg->poly;
  uint64_t state = reg->state;
  int poly_deg = reg->poly_deg;

  int i, j, out_len;
  uint64_t temp, bit;

  poly >>= 1;
  out_len = (out_bits/8) + ((out_bits%8!=0) ? 1 : 0);
  for (i=0; i<out_len; i++) {
    output[i] = 0;
  }

  for (i=0; i<out_bits; i++) {
    /* compute output bit */
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
  /* should we state &= mask;  ?? */
  return state;
}

uint8_t lfsr_get_bit(lfsr* reg, int index) {
}

void lfsr_set_bit(lfsr* reg, int index, uint8_t value) {
}
