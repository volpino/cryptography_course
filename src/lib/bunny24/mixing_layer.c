#include "bunny_internals.h"

void array_cp(array src, array dst) {
  int i;
  for(i=0; i<ARRAY_LEN; i++) {
    dst[i] = src[i];
  }
}

uint8_t array_scal(array a, array b) {
  int i;
  uint8_t res = 0x0;
  for (i=0; i<ARRAY_LEN; i++) {
    res = bunny_sum(res, bunny_mul(a[i], b[i]));
  }
  return res;
}

/* apply mixing layer, in-place  */
void mixing_layer(array a) {
  int i;
  array in;
  array_cp(a, in);
  for (i=0; i<ARRAY_LEN; i++) {
    a[i] =  array_scal(in, lambdaT[i]);
  }
  return;
}
