#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "bunny_internals.h"

void test_key_schedule() {
  array key;
  array rkeys[16];
  int i;
  int index; /* required for array_print()...
                perhaps it should be NOT a macro */

  /* key = (1 0 0 1 0 0 | 0 1 1 0 1 1 | 1 0 1 0 0 1 | 1 0 0 0 0 0) */
  key[0] = 0x24; key[1] = 0x1b; key[2] = 0x29; key[3] = 0x20;

  key_schedule(key, rkeys);

  for (i=0; i<16; i++) {
    array_print(rkeys[i]);
  }
}

void test_encrypt() {
  array k, c, m;
  m[0] = 9; m[1] = 53; m[2] = 32; m[3] = 60;
  k[0] = 61; k[1] = 13; k[2] = 19; k[3] = 16;
  c[0] = 62; c[1] = 16; c[2] = 17; c[3] = 38;
  g6_print(m[0]); g6_print(c[0]); printf("\n");
  encrypt_internal(m, k);
  g6_print(m[0]); g6_print(c[0]);
  assert(m[0] == c[0]);
  assert(m[1] == c[1]);
  assert(m[2] == c[2]);
  assert(m[3] == c[3]);

  m[0] = 28; m[1] = 45; m[2] = 43; m[3] = 28;
  k[0] = 34; k[1] = 35; k[2] = 14; k[3] = 30;
  c[0] = 47; c[1] = 48; c[2] = 48; c[3] = 43;
  encrypt_internal(m, k);
  assert(m[0] == c[0]);
  assert(m[1] == c[1]);
  assert(m[2] == c[2]);
  assert(m[3] == c[3]);

  m[0] = 2; m[1] = 1; m[2] = 28; m[3] = 54;
  k[0] = 35; k[1] = 5; k[2] = 45; k[3] = 28;
  c[0] = 23; c[1] = 34; c[2] = 10; c[3] = 23;
  encrypt_internal(m, k);
  assert(m[0] == c[0]);
  assert(m[1] == c[1]);
  assert(m[2] == c[2]);
  assert(m[3] == c[3]);

  m[0] = 15; m[1] = 21; m[2] = 41; m[3] = 49;
  k[0] = 34; k[1] = 58; k[2] = 54; k[3] = 30;
  c[0] = 38; c[1] = 18; c[2] = 46; c[3] = 31;
  encrypt_internal(m, k);
  assert(m[0] == c[0]);
  assert(m[1] == c[1]);
  assert(m[2] == c[2]);
  assert(m[3] == c[3]);

  m[0] = 23; m[1] = 59; m[2] = 41; m[3] = 9;
  k[0] = 26; k[1] = 17; k[2] = 25; k[3] = 50;
  c[0] = 29; c[1] = 17; c[2] = 33; c[3] = 44;
  encrypt_internal(m, k);
  assert(m[0] == c[0]);
  assert(m[1] == c[1]);
  assert(m[2] == c[2]);
  assert(m[3] == c[3]);

  m[0] = 37; m[1] = 18; m[2] = 30; m[3] = 21;
  k[0] = 21; k[1] = 17; k[2] = 17; k[3] = 24;
  c[0] = 47; c[1] = 27; c[2] = 18; c[3] = 30;
  encrypt_internal(m, k);
  assert(m[0] == c[0]);
  assert(m[1] == c[1]);
  assert(m[2] == c[2]);
  assert(m[3] == c[3]);

  m[0] = 34; m[1] = 55; m[2] = 33; m[3] = 6;
  k[0] = 62; k[1] = 55; k[2] = 61; k[3] = 16;
  c[0] = 29; c[1] = 44; c[2] = 54; c[3] = 30;
  encrypt_internal(m, k);
  assert(m[0] == c[0]);
  assert(m[1] == c[1]);
  assert(m[2] == c[2]);
  assert(m[3] == c[3]);

  m[0] = 32; m[1] = 57; m[2] = 9; m[3] = 62;
  k[0] = 4; k[1] = 36; k[2] = 57; k[3] = 62;
  c[0] = 15; c[1] = 34; c[2] = 56; c[3] = 25;
  encrypt_internal(m, k);
  assert(m[0] == c[0]);
  assert(m[1] == c[1]);
  assert(m[2] == c[2]);
  assert(m[3] == c[3]);

  m[0] = 2; m[1] = 41; m[2] = 24; m[3] = 36;
  k[0] = 53; k[1] = 37; k[2] = 12; k[3] = 41;
  c[0] = 33; c[1] = 2; c[2] = 30; c[3] = 36;
  encrypt_internal(m, k);
  assert(m[0] == c[0]);
  assert(m[1] == c[1]);
  assert(m[2] == c[2]);
  assert(m[3] == c[3]);

  m[0] = 13; m[1] = 25; m[2] = 35; m[3] = 14;
  k[0] = 4; k[1] = 52; k[2] = 21; k[3] = 37;
  c[0] = 47; c[1] = 37; c[2] = 46; c[3] = 57;
  encrypt_internal(m, k);
  assert(m[0] == c[0]);
  assert(m[1] == c[1]);
  assert(m[2] == c[2]);
  assert(m[3] == c[3]);

  m[0] = 47; m[1] = 37; m[2] = 0; m[3] = 44;
  k[0] = 36; k[1] = 26; k[2] = 59; k[3] = 53;
  c[0] = 14; c[1] = 3; c[2] = 32; c[3] = 48;
  encrypt_internal(m, k);
  assert(m[0] == c[0]);
  assert(m[1] == c[1]);
  assert(m[2] == c[2]);
  assert(m[3] == c[3]);

  m[0] = 26; m[1] = 4; m[2] = 15; m[3] = 1;
  k[0] = 60; k[1] = 43; k[2] = 37; k[3] = 59;
  c[0] = 51; c[1] = 8; c[2] = 47; c[3] = 34;
  encrypt_internal(m, k);
  assert(m[0] == c[0]);
  assert(m[1] == c[1]);
  assert(m[2] == c[2]);
  assert(m[3] == c[3]);

  m[0] = 17; m[1] = 61; m[2] = 55; m[3] = 62;
  k[0] = 54; k[1] = 58; k[2] = 57; k[3] = 24;
  c[0] = 20; c[1] = 48; c[2] = 13; c[3] = 17;
  encrypt_internal(m, k);
  assert(m[0] == c[0]);
  assert(m[1] == c[1]);
  assert(m[2] == c[2]);
  assert(m[3] == c[3]);

  m[0] = 49; m[1] = 7; m[2] = 37; m[3] = 9;
  k[0] = 43; k[1] = 6; k[2] = 8; k[3] = 41;
  c[0] = 27; c[1] = 49; c[2] = 47; c[3] = 37;
  encrypt_internal(m, k);
  assert(m[0] == c[0]);
  assert(m[1] == c[1]);
  assert(m[2] == c[2]);
  assert(m[3] == c[3]);

  m[0] = 42; m[1] = 37; m[2] = 42; m[3] = 1;
  k[0] = 40; k[1] = 22; k[2] = 57; k[3] = 41;
  c[0] = 44; c[1] = 56; c[2] = 47; c[3] = 31;
  encrypt_internal(m, k);
  assert(m[0] == c[0]);
  assert(m[1] == c[1]);
  assert(m[2] == c[2]);
  assert(m[3] == c[3]);

  m[0] = 38; m[1] = 52; m[2] = 24; m[3] = 44;
  k[0] = 30; k[1] = 41; k[2] = 4; k[3] = 9;
  c[0] = 40; c[1] = 38; c[2] = 20; c[3] = 34;
  encrypt_internal(m, k);
  assert(m[0] == c[0]);
  assert(m[1] == c[1]);
  assert(m[2] == c[2]);
  assert(m[3] == c[3]);

  m[0] = 27; m[1] = 58; m[2] = 52; m[3] = 61;
  k[0] = 14; k[1] = 43; k[2] = 28; k[3] = 39;
  c[0] = 21; c[1] = 36; c[2] = 13; c[3] = 25;
  encrypt_internal(m, k);
  assert(m[0] == c[0]);
  assert(m[1] == c[1]);
  assert(m[2] == c[2]);
  assert(m[3] == c[3]);

  m[0] = 51; m[1] = 27; m[2] = 22; m[3] = 39;
  k[0] = 49; k[1] = 13; k[2] = 56; k[3] = 31;
  c[0] = 47; c[1] = 53; c[2] = 7; c[3] = 0;
  encrypt_internal(m, k);
  assert(m[0] == c[0]);
  assert(m[1] == c[1]);
  assert(m[2] == c[2]);
  assert(m[3] == c[3]);

  m[0] = 20; m[1] = 20; m[2] = 57; m[3] = 21;
  k[0] = 32; k[1] = 52; k[2] = 11; k[3] = 34;
  c[0] = 41; c[1] = 1; c[2] = 58; c[3] = 38;
  encrypt_internal(m, k);
  assert(m[0] == c[0]);
  assert(m[1] == c[1]);
  assert(m[2] == c[2]);
  assert(m[3] == c[3]);

  m[0] = 61; m[1] = 27; m[2] = 4; m[3] = 23;
  k[0] = 36; k[1] = 27; k[2] = 41; k[3] = 32;
  c[0] = 50; c[1] = 18; c[2] = 35; c[3] = 51;
  encrypt_internal(m, k);
  assert(m[0] == c[0]);
  assert(m[1] == c[1]);
  assert(m[2] == c[2]);
  assert(m[3] == c[3]);
}

void test_mixing_layer() {
  //1 1 0 0 1 1   1 0 0 1 0 0   0 0 0 1 0 1   1 0 0 0 0 0
  array k;
  int index;
  k[0] = 0x33; k[1] = 0x24; k[2] = 0x05; k[3] = 0x20;
  mixing_layer(k);
  array_print(k);
}

int main() {
  //test_key_schedule();

  //test_mixing_layer();

  test_encrypt();

  exit(0);
}
