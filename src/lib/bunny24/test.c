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
  array t;  /* XXX: this is only for checking consistency */
  m[0] = 9; m[1] = 53; m[2] = 32; m[3] = 60;
  t[0] = 9; t[1] = 53; t[2] = 32; t[3] = 60;
  k[0] = 61; k[1] = 13; k[2] = 19; k[3] = 16;
  c[0] = 62; c[1] = 16; c[2] = 17; c[3] = 38;
  g6_print(m[0]); g6_print(c[0]); printf("\n");
  encrypt_internal(m, k);
  g6_print(m[0]); g6_print(c[0]); printf("\n");
  decrypt_internal(m, k);
  g6_print(m[0]); printf("\n");
  fflush(stdout);

  assert(m[0] == t[0]);
  assert(m[1] == t[1]);
  assert(m[2] == t[2]);
  assert(m[3] == t[3]);

  m[0] = 9; m[1] = 53; m[2] = 32; m[3] = 60;
  k[0] = 61; k[1] = 13; k[2] = 19; k[3] = 16;
  c[0] = 62; c[1] = 16; c[2] = 17; c[3] = 38;
  encrypt_internal(m, k);
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
  array_print(k);
  mixing_layer(k);
  array_print(k);
  mixing_layer_inv(k);
  array_print(k);
}

void test_sbox() {
  assert(SB1[1] == 1);
  assert(SB2[1] == 1);
  assert(SB3[1] == 1);
  assert(SB4[1] == 5);
  assert(SB1[2] == 45);
  assert(SB2[2] == 32);
  assert(SB3[2] == 38);
  assert(SB4[2] == 41);
  assert(SB1[4] == 59);
  assert(SB2[4] == 49);
  assert(SB3[4] == 37);
  assert(SB4[4] == 63);
  assert(SB1[8] == 48);
  assert(SB2[8] == 36);
  assert(SB3[8] == 20);
  assert(SB4[8] == 52);
  assert(SB1[16] == 24);
  assert(SB2[16] == 7);
  assert(SB3[16] == 32);
  assert(SB4[16] == 28);
  assert(SB1[32] == 12);
  assert(SB2[32] == 13);
  assert(SB3[32] == 28);
  assert(SB4[32] == 8);
  assert(SB1[27] == 6);
  assert(SB2[27] == 33);
  assert(SB3[27] == 39);
  assert(SB4[27] == 2);
  assert(SB1[54] == 3);
  assert(SB2[54] == 17);
  assert(SB3[54] == 3);
  assert(SB4[54] == 7);
  assert(SB1[55] == 44);
  assert(SB2[55] == 21);
  assert(SB3[55] == 49);
  assert(SB4[55] == 40);
  assert(SB1[53] == 22);
  assert(SB2[53] == 35);
  assert(SB3[53] == 52);
  assert(SB4[53] == 18);
  assert(SB1[49] == 11);
  assert(SB2[49] == 10);
  assert(SB3[49] == 60);
  assert(SB4[49] == 15);
  assert(SB1[57] == 40);
  assert(SB2[57] == 44);
  assert(SB3[57] == 59);
  assert(SB4[57] == 44);
  assert(SB1[41] == 20);
  assert(SB2[41] == 48);
  assert(SB3[41] == 36);
  assert(SB4[41] == 16);
  assert(SB1[9] == 10);
  assert(SB2[9] == 4);
  assert(SB3[9] == 50);
  assert(SB4[9] == 14);
  assert(SB1[18] == 5);
  assert(SB2[18] == 54);
  assert(SB3[18] == 5);
  assert(SB4[18] == 1);
  assert(SB1[36] == 47);
  assert(SB2[36] == 41);
  assert(SB3[36] == 8);
  assert(SB4[36] == 43);
  assert(SB1[19] == 58);
  assert(SB2[19] == 38);
  assert(SB3[19] == 7);
  assert(SB4[19] == 62);
  assert(SB1[38] == 29);
  assert(SB2[38] == 28);
  assert(SB3[38] == 31);
  assert(SB4[38] == 25);
  assert(SB1[23] == 35);
  assert(SB2[23] == 52);
  assert(SB3[23] == 22);
  assert(SB4[23] == 39);
  assert(SB1[46] == 60);
  assert(SB2[46] == 50);
  assert(SB3[46] == 55);
  assert(SB4[46] == 56);
  assert(SB1[7] == 30);
  assert(SB2[7] == 31);
  assert(SB3[7] == 13);
  assert(SB4[7] == 26);
  assert(SB1[14] == 15);
  assert(SB2[14] == 15);
  assert(SB3[14] == 15);
  assert(SB4[14] == 11);
  assert(SB1[28] == 42);
  assert(SB2[28] == 58);
  assert(SB3[28] == 24);
  assert(SB4[28] == 46);
  assert(SB1[56] == 21);
  assert(SB2[56] == 40);
  assert(SB3[56] == 9);
  assert(SB4[56] == 17);
  assert(SB1[43] == 39);
  assert(SB2[43] == 6);
  assert(SB3[43] == 33);
  assert(SB4[43] == 35);
  assert(SB1[13] == 62);
  assert(SB2[13] == 45);
  assert(SB3[13] == 58);
  assert(SB4[13] == 58);
  assert(SB1[26] == 31);
  assert(SB2[26] == 16);
  assert(SB3[26] == 2);
  assert(SB4[26] == 27);
  assert(SB1[52] == 34);
  assert(SB2[52] == 53);
  assert(SB3[52] == 23);
  assert(SB4[52] == 38);
  assert(SB1[51] == 17);
  assert(SB2[51] == 18);
  assert(SB3[51] == 17);
  assert(SB4[51] == 21);
  assert(SB1[61] == 37);
  assert(SB2[61] == 46);
  assert(SB3[61] == 40);
  assert(SB4[61] == 33);
  assert(SB1[33] == 63);
  assert(SB2[33] == 43);
  assert(SB3[33] == 27);
  assert(SB4[33] == 59);
  assert(SB1[25] == 50);
  assert(SB2[25] == 61);
  assert(SB3[25] == 56);
  assert(SB4[25] == 54);
  assert(SB1[50] == 25);
  assert(SB2[50] == 37);
  assert(SB3[50] == 21);
  assert(SB4[50] == 29);
  assert(SB1[63] == 33);
  assert(SB2[63] == 39);
  assert(SB3[63] == 6);
  assert(SB4[63] == 37);
  assert(SB1[37] == 61);
  assert(SB2[37] == 60);
  assert(SB3[37] == 57);
  assert(SB4[37] == 57);
  assert(SB1[17] == 51);
  assert(SB2[17] == 5);
  assert(SB3[17] == 51);
  assert(SB4[17] == 55);
  assert(SB1[34] == 52);
  assert(SB2[34] == 22);
  assert(SB3[34] == 35);
  assert(SB4[34] == 48);
  assert(SB1[31] == 26);
  assert(SB2[31] == 24);
  assert(SB3[31] == 45);
  assert(SB4[31] == 30);
  assert(SB1[62] == 13);
  assert(SB2[62] == 2);
  assert(SB3[62] == 19);
  assert(SB4[62] == 9);
  assert(SB1[39] == 43);
  assert(SB2[39] == 27);
  assert(SB3[39] == 63);
  assert(SB4[39] == 47);
  assert(SB1[21] == 56);
  assert(SB2[21] == 57);
  assert(SB3[21] == 10);
  assert(SB4[21] == 60);
  assert(SB1[42] == 28);
  assert(SB2[42] == 19);
  assert(SB3[42] == 16);
  assert(SB4[42] == 24);
  assert(SB1[15] == 14);
  assert(SB2[15] == 14);
  assert(SB3[15] == 14);
  assert(SB4[15] == 10);
  assert(SB1[30] == 7);
  assert(SB2[30] == 26);
  assert(SB3[30] == 62);
  assert(SB4[30] == 3);
  assert(SB1[60] == 46);
  assert(SB2[60] == 25);
  assert(SB3[60] == 44);
  assert(SB4[60] == 42);
  assert(SB1[35] == 23);
  assert(SB2[35] == 34);
  assert(SB3[35] == 53);
  assert(SB4[35] == 19);
  assert(SB1[29] == 38);
  assert(SB2[29] == 42);
  assert(SB3[29] == 26);
  assert(SB4[29] == 34);
  assert(SB1[58] == 19);
  assert(SB2[58] == 29);
  assert(SB3[58] == 30);
  assert(SB4[58] == 23);
  assert(SB1[47] == 36);
  assert(SB2[47] == 20);
  assert(SB3[47] == 48);
  assert(SB4[47] == 32);
  assert(SB1[5] == 18);
  assert(SB2[5] == 3);
  assert(SB3[5] == 18);
  assert(SB4[5] == 22);
  assert(SB1[10] == 9);
  assert(SB2[10] == 59);
  assert(SB3[10] == 25);
  assert(SB4[10] == 13);
  assert(SB1[20] == 41);
  assert(SB2[20] == 8);
  assert(SB3[20] == 47);
  assert(SB4[20] == 45);
  assert(SB1[40] == 57);
  assert(SB2[40] == 55);
  assert(SB3[40] == 4);
  assert(SB4[40] == 61);
  assert(SB1[11] == 49);
  assert(SB2[11] == 9);
  assert(SB3[11] == 46);
  assert(SB4[11] == 53);
  assert(SB1[22] == 53);
  assert(SB2[22] == 23);
  assert(SB3[22] == 34);
  assert(SB4[22] == 49);
  assert(SB1[44] == 55);
  assert(SB2[44] == 56);
  assert(SB3[44] == 11);
  assert(SB4[44] == 51);
  assert(SB1[3] == 54);
  assert(SB2[3] == 51);
  assert(SB3[3] == 54);
  assert(SB4[3] == 50);
  assert(SB1[6] == 27);
  assert(SB2[6] == 63);
  assert(SB3[6] == 43);
  assert(SB4[6] == 31);
  assert(SB1[12] == 32);
  assert(SB2[12] == 62);
  assert(SB3[12] == 42);
  assert(SB4[12] == 36);
  assert(SB1[24] == 16);
  assert(SB2[24] == 30);
  assert(SB3[24] == 12);
  assert(SB4[24] == 20);
  assert(SB1[48] == 8);
  assert(SB2[48] == 47);
  assert(SB3[48] == 41);
  assert(SB4[48] == 12);
  assert(SB1[59] == 4);
  assert(SB2[59] == 11);
  assert(SB3[59] == 61);
  assert(SB4[59] == 0);
  assert(SB1[45] == 2);
  assert(SB2[45] == 12);
  assert(SB3[45] == 29);
  assert(SB4[45] == 6);
  assert(SB1[0] == 0);
  assert(SB2[0] == 0);
  assert(SB3[0] == 0);
  assert(SB4[0] == 4);
}

int main() {
  //test_key_schedule();

  //test_mixing_layer();

  test_sbox();

  test_encrypt();

  exit(0);
}
