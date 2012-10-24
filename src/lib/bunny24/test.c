#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "bunny_internals.h"

void test_key_schedule () {
  array key;
  array rkeys[16];
  int i;
  int index; /* reqiored for array_print()...
                perhaps it should be NOT a macro */

  /* key = (1 0 0 1 0 0 | 0 1 1 0 1 1 | 1 0 1 0 0 1 | 1 0 0 0 0 0) */
  key[0] = 0x24; key[1] = 0x1b; key[2] = 0x29; key[3] = 0x20;

  key_schedule(key, rkeys);

  for (i=0; i<16; i++) {
    array_print(rkeys[i]);
  }
}

int main () {
  test_key_schedule();
  exit(0);
}
