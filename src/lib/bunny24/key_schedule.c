#include "bunny_internals.h"
#include <stdio.h>
/*
void key_schedule(array key, uint8_t* result) {
    uint8_t w[20];
    w[0] = key[0];
    w[1] = key[1];
    w[2] = key[2];
    w[3] = key[3];

    / Wi = SB(Wi-4) + Wi-3 /
    w[4] = bunny_add(SB1[w[0]], w[1]);
    w[5] = bunny_add(SB1[w[1]], w[2]);
    w[6] = bunny_add(SB1[w[2]], w[3]);
    w[7] = bunny_add(SB1[w[3]], w[0]);

    w[8] = bunny_add(bunny_add(w[0], SB2[bunny_rotate(w[7], -1)]), 40);
    w[9] = bunny_add(w[1], w[8]);
    w[10] = bunny_add(w[2], w[9]);
    w[11] = bunny_add(w[3], w[10]);
    w[12] = bunny_add(w[4], SB3[w[11]]);
    w[13] = bunny_add(w[5], w[12]);
    w[14] = bunny_add(w[6], w[13]);
    w[15] = bunny_add(w[7], w[14]);
    w[16] = bunny_add(bunny_add(w[8], SB2[bunny_rotate(w[15], -1)]), 40);
    w[17] = bunny_add(w[9], w[16]);
    w[18] = bunny_add(w[10], w[17]);
    w[19] = bunny_add(w[11], w[18]);
}
*/

/* result must be capable of holding 16 keys.
*/
void key_schedule(array key, array* result) {
  uint8_t w[88];
  uint8_t tb[4][5][4];
  int t, i, j, k;

  /* Generate sequence of 88 6-bit words */
  w[0] = key[0];
  w[1] = key[1];
  w[2] = key[2];
  w[3] = key[3];

  /* Wi = SB(Wi-4) + Wi-3 */
  w[4] = bunny_add(SB1[w[0]], w[1]);
  w[5] = bunny_add(SB2[w[1]], w[2]);
  w[6] = bunny_add(SB3[w[2]], w[3]);
  w[7] = bunny_add(SB4[w[3]], w[0]);

  for (k=8; k<88; k++) {
    i = k+1;
    if ((i % 4) != 1) {
      w[k] = bunny_add(w[k-8], w[k-1]);
    }
    else if ((i % 8) == 1) {
      w[k] = bunny_add(bunny_add(w[k-8], SB2[bunny_rotate(w[k-1], -1)]), 0x2A);
    }
    else if ((i % 8) == 5) {
      bunny_add(w[k-8], SB3[w[k-1]]);
    }
    else
      printf("ERRORRORRRRR!\n");
  }


  for (k=0; k<88; k++) {
    printf("W_%i: ", k+1);
    g6_print(w[k]);
    printf("\n");
  }


  /* Rearrange words in the proper tables */
  k = 8;
  for (t=0; t<4; t++) {
    for (i=0; i<5; i++) {
      for (j=0; j<4; j++){
        tb[t][i][j] = w[k];
        k++;
      }
    }
  }

  for (t=0; t<4; t++) {
    for (i=0; i<5; i++) {
      for (j=0; j<4; j++){
        g6_print(tb[t][i][j]);
      }
      printf("\n");
    }
    printf("\n");
  }
  printf("\n\n\n");

  /* Read tables in diagonal, in order to get the keys */
  k = 0;
  for (t=0; t<3 && k<16; t++) {
    for (i=0; i<5 && k<16; i++) {
      for (j=0; j<4; j++){
        result[k][j] = tb[t][(i+j)%5][j];
      }
      k++;
    }
  }

  return;
}
