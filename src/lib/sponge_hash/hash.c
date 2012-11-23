#include <stdlib.h>
#include "../bunny24.h"

/* every m element is 20 bits, the last 4 bits are 0 */
void sponge_hash_internal(b24_t* m, int len_m, b24_t key, uint8_t* out) {
  uint8_t state_byte[3];
  b24_t state;
  int i, j;

  for (i=0; i<B24_T_LEN; i++) {
    state[i] = 0;
  }

  /* absorbing phase */
  for (i=0; i<len_m; i++) {
    for (j=0; j<B24_T_LEN; j++) {
      state[j] ^= m[i][j];
    }
    b24_encrypt(state, key);
  }


  /* squeezing phase */
  /* we create the output 40 bits at a time,
     so we need 4 loops to get to 160 bits */
  for (i=0; i<4; i++) {
    b24_to_byte(&state, state_byte, 3);
    out[i] = state_byte[0];
    out[i+1] = state_byte[1];
    out[i+2] = (state_byte[2] & 0xf0);

    b24_encrypt(state, key);
    b24_to_byte(&state, state_byte, 3);

    out[i+2] = out[i+2] ^ (state_byte[0] >> 4);
    out[i+3] = (state_byte[0] << 4);

    out[i+3] = out[i+3] ^ (state_byte[1] >> 4);
    out[i+4] = (state_byte[1] << 4);
    out[i+5] = (state_byte[2] & 0xf0);
  }
}

void sponge_hash(uint8_t* input, int len, uint8_t* out) {
  b24_t* m = (b24_t*) malloc(len * sizeof(b24_t));
  b24_t key;
  int i;

  byte_to_b24(input, len, m);

  /* initialize the key to all 1s */
  for (i=0; i<B24_T_LEN; i++) {
    key[i] = 1;
  }

  sponge_hash_internal(m, len, key, out);
  free(m);
}

