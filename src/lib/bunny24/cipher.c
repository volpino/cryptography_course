#include "bunny_internals.h"

/* sums a and b, stores result into a */
void array_inc(array a, const array b) {
  int i;
  for (i=0; i<ARRAY_LEN; i++) {
    a[i] = bunny_add(a[i], b[i]);
  }
}

void sboxes_enc(array c){
  c[0] = SB1[c[0]];
  c[1] = SB2[c[1]];
  c[2] = SB3[c[2]];
  c[3] = SB4[c[3]];
}

void sboxes_dec(array c){
  c[0] = SB1i[c[0]];
  c[1] = SB2i[c[1]];
  c[2] = SB3i[c[2]];
  c[3] = SB4i[c[3]];
}


void encrypt_internal(array m, array k) {
  array rkeys[16];
  int round;

  key_schedule(k, rkeys);

  array_inc(m, rkeys[0]);

  for (round=1; round<=ROUND_NUM; round++) {
    sboxes_enc(m);
    mixing_layer(m);
    array_inc(m, rkeys[round]);
  }
}
