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

void encrypt_internal(array m, const array k) {
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

void decrypt_internal(array m, const array k) {
  array rkeys[16];
  int round;

  key_schedule(k, rkeys);


  for (round=ROUND_NUM; round>0; round--) {
    array_inc(m, rkeys[round]);
    mixing_layer_inv(m);
    sboxes_dec(m);
  }

  array_inc(m, rkeys[0]);
}

void encrypt_cbc_internal(array* m, int n, const array k, const array iv) {
    int i;

    array_inc(m[0], iv);
    encrypt_internal(m[0], k);

    for (i=1; i<n; i++) {
        array_inc(m[i], m[i-1]);
        encrypt_internal(m[i], k);
    }
}

void decrypt_cbc_internal(array* m, int n, const array k, const array iv) {
    int i;

    for (i=n-1; i>0; i--) {
        decrypt_internal(m[i], k);
        array_inc(m[i], m[i-1]);
    }

    encrypt_internal(m[0], k);
    array_inc(m[0], iv);
}
