#include "bunny_internals.h"

void b24_cp(const b24_t src, b24_t dst) {
  int i;
  for(i=0; i<B24_T_LEN; i++) {
    dst[i] = src[i];
  }
}

uint8_t b24_scal(b24_t a, const b24_t b) {
  int i;
  uint8_t res = 0x0;
  for (i=0; i<B24_T_LEN; i++) {
    res = bunny_add(res, bunny_mul(a[i], b[i]));
  }
  return res;
}

/* apply mixing layer, in-place  */
void mixing_layer(b24_t a) {
  int i;
  b24_t in;
  b24_cp(a, in);
  for (i=0; i<B24_T_LEN; i++) {
    a[i] =  b24_scal(in, lambdaT[i]);
  }
}

void mixing_layer_inv(b24_t a) {
  int i;
  b24_t in;
  b24_cp(a, in);
  for (i=0; i<B24_T_LEN; i++) {
    a[i] =  b24_scal(in, lambdaT_i[i]);
  }
}
