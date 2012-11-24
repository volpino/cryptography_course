#include <stdlib.h>
#include "../bunny24.h"


/* every m element is 20 bits, the last 4 bits are 0 */
void sponge_hash_internal(b24_t* m, int len_m, b24_t key, uint8_t* out) {
  uint8_t state_byte[3];
  b24_t state;
  int i, j, k;

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
    k = i * 5;

    b24_to_byte(&state, state_byte, 3);

    out[k] = state_byte[0];
    out[k+1] = state_byte[1];
    out[k+2] = (state_byte[2] & 0xf0);

    b24_encrypt(state, key);
    b24_to_byte(&state, state_byte, 3);

    out[k+2] = out[k+2] ^ (state_byte[0] >> 4);

    out[k+3] = state_byte[0] << 4;
    out[k+3] = out[k+3] ^ (state_byte[1] >> 4);

    out[k+4] = state_byte[1] << 4;
    out[k+4] = out[k+4] ^ (state_byte[2] >> 4);

    b24_encrypt(state, key);
  }
}


/* This function converts an array of uint8_t to an array of b64_t
 * where for every b24_t the first 20 bits are filled and the remaining 4
 * are set to 0.
 */
void byte_to_b20(uint8_t* inp, int n, b24_t* out) {
  int i, j;
  uint32_t tmp, x1, x2, x3;

  for (i=0, j=0; i<(n + (n % 5 == 0 ? 0 : 1)); i+=5, j+=2) {
    /* Set 8 + 8 + 4 = 20 bits */
    x1 = inp[i] << 24;
    x2 = i+1 < n ? (inp[i+1] << 16) : 0;
    x3 = i+2 < n ? ((inp[i+2] & 0xF0) << 8) : 0;  /* Take the first 4 bits */

    tmp = x1 + x2 + x3;
    out[j][0] = (tmp & 0xFC000000) >> 26;
    out[j][1] = (tmp & 0x03F00000) >> 20;
    out[j][2] = (tmp & 0x000FC000) >> 14;
    out[j][3] = (tmp & 0x00003F00) >> 8;

    /* Set the remaining 4 + 8 + 8 = 20 bits */
    x1 = i+2 < n ? ((inp[i+2] & 0x0F) << 28) : 0;  /* And then the last 4 */
    x2 = i+3 < n ? (inp[i+3] << 20) : 0;
    x3 = i+4 < n ? (inp[i+4] << 12) : 0;

    tmp = x1 + x2 + x3;
    if (tmp != 0) {
      out[j + 1][0] = (tmp & 0xFC000000) >> 26;
      out[j + 1][1] = (tmp & 0x03F00000) >> 20;
      out[j + 1][2] = (tmp & 0x000FC000) >> 14;
      out[j + 1][3] = (tmp & 0x00003F00) >> 8;
    }
  }
}


void sponge_hash(uint8_t* input, int len, uint8_t* out) {
  int m_len = (len * 8) / 20 + ((len * 8) % 5 == 0 ? 0 : 1);
  b24_t* m = (b24_t*) malloc(len * sizeof(b24_t));
  b24_t key;
  int i;

  byte_to_b20(input, len, m);

  /* initialize the key to all 1s */
  for (i=0; i<B24_T_LEN; i++) {
    key[i] = 0x3F;
  }

  sponge_hash_internal(m, m_len, key, out);
  free(m);
}
