#include "bunny_internals.h"
#include <assert.h>

#define W_LEN 88
#define T_NUM 4
#define ROW_NUM 5
#define COL_NUM 4

/* result must be capable of holding 16 keys.*/
void key_schedule(const b24_t key, b24_t* result) {
  uint8_t w[W_LEN];
  uint8_t tb[T_NUM][ROW_NUM][COL_NUM];
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

  for (k=8; k<W_LEN; k++) {
    i = k+1;
    if ((i % 4) != 1) {
      w[k] = bunny_add(w[k-8], w[k-1]);
    }
    else if ((i % 8) == 1) {
      w[k] = bunny_add(bunny_add(w[k-8], SB2[bunny_rotate(w[k-1], -1)]), 0x2A);
    }
    else if ((i % 8) == 5) {
      w[k] = bunny_add(w[k-8], SB3[w[k-1]]);
    }
    else
      assert(0);
  }

  /*
  for (k=0; k<W_LEN; k++) {
    printf("W_%i: ", k+1);
    g6_print(w[k]);
    printf("\n");
  }
  */


  /* Rearrange words in the proper tables */
  k = 8;
  for (t=0; t<T_NUM; t++) {
    for (i=0; i<ROW_NUM; i++) {
      for (j=0; j<COL_NUM; j++){
        tb[t][i][j] = w[k];
        k++;
      }
    }
  }

  /*
  for (t=0; t<T_NUM; t++) {
    for (i=0; i<ROW_NUM; i++) {
      for (j=0; j<COL_NUM; j++){
        g6_print(tb[t][i][j]);
      }
      printf("\n");
    }
    printf("\n");
  }
  printf("\n\n\n");
  */

  /* Read tables in diagonal, in order to get the keys */
  k = 0;
  for (t=0; t<T_NUM && k<16; t++) {
    for (i=0; i<ROW_NUM && k<16; i++) {
      for (j=0; j<COL_NUM; j++){
        result[k][j] = tb[t][j+(j+i)/COL_NUM][(i+j)%COL_NUM];
      }
      k++;
    }
  }
  return;
}

