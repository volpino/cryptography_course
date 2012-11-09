#include "bunny_internals.h"
#include <stdlib.h>
#include <stdio.h>

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

    decrypt_internal(m[0], k);
    array_inc(m[0], iv);
}

void byte_to_g6(uint8_t* inp, int n, array* out) {
    int i;
    uint32_t tmp, x1, x2, x3;

    for (i=0; i<(n + (n % 3 == 0 ? 0 : 1)); i+=3) {
        x1 = inp[i] << 24;
        x2 = i+1 < n ? (inp[i+1] << 16) : 0;
        x3 = i+2 < n ? (inp[i+2] << 8) : 0;

        tmp = x1 + x2 + x3;
        out[i / 3][0] = (tmp & 0xFC000000) >> 26;
        out[i / 3][1] = (tmp & 0x03F00000) >> 20;
        out[i / 3][2] = (tmp & 0x000FC000) >> 14;
        out[i / 3][3] = (tmp & 0x00003F00) >> 8;
    }
}

/* Note: This functions assumes that n is a multiple of 3
 * the output is always padded
 */
void g6_to_byte(array* inp, uint8_t* out, int n) {
    int i;
    uint32_t tmp, x1, x2, x3, x4;

    for (i=0; i<(n + (n % 3 == 0 ? 0 : 1)); i+=3) {
        x1 = inp[i / 3][0] << 26;
        x2 = inp[i / 3][1] << 20;
        x3 = inp[i / 3][2] << 14;
        x4 = inp[i / 3][3] << 8;

        tmp = x1 + x2 + x3 + x4;
        out[i] = (tmp & 0xFF000000) >> 24;
        /*if (i+1 < n)*/ out[i+1] = (tmp & 0x00FF0000) >> 16;
        /*if (i+2 < n)*/ out[i+2] = (tmp & 0x0000FF00) >> 8;
    }
}


/* This is the public Bunny24 encrypt function
 * m is an uint8_t array
 * n is the length if m
 * k is an uint8_t array of length 3 (24 bits)
 * iv is an uint8_t array of length 3 (24 bits)
 *
 * Note: encryption is performed in-place
 */
void bunny24_encrypt_cbc(uint8_t* m, int n, uint8_t* k, uint8_t* iv) {
    int len_m_arr = n % 3 == 0 ? n / 3 : n / 3 + 1;
    array k_arr;
    array iv_arr;

    array* m_arr = (array*) malloc(len_m_arr * sizeof(array));
    byte_to_g6(m, n, m_arr);

    byte_to_g6(k, 3, &k_arr);

    byte_to_g6(iv, 3, &iv_arr);

    encrypt_cbc_internal(m_arr, len_m_arr, k_arr, iv_arr);

    g6_to_byte(m_arr, m, n);

    free(m_arr);
}


/* This is the public Bunny24 decrypt function
 * m is an uint8_t array
 * n is the length if m
 * k is an uint8_t array of length 3 (24 bits)
 * iv is an uint8_t array of length 3 (24 bits)
 *
 * Note: encryption is performed in-place
 */
void bunny24_decrypt_cbc(uint8_t* m, int n, uint8_t* k, uint8_t* iv) {
    int len_m_arr = n % 3 == 0 ? n / 3 : n / 3 + 1;
    array k_arr;
    array iv_arr;

    array* m_arr = (array*) malloc(len_m_arr * sizeof(array));
    byte_to_g6(m, n, m_arr);

    byte_to_g6(k, 3, &k_arr);

    byte_to_g6(iv, 3, &iv_arr);

    decrypt_cbc_internal(m_arr, len_m_arr, k_arr, iv_arr);

    g6_to_byte(m_arr, m, n);

    free(m_arr);
}
