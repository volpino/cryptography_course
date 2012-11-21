#include "../bunny_internals.h"
#include <stdlib.h>
#include <stdio.h>

#define SPACE 16777216

uint8_t matrix[SPACE];

void array_increment(array a) {
  int i;
  uint8_t carry = 1;
  for(i=0; i<ARRAY_LEN && carry == 0; i++) {
    if (a[i] != 63) {
      carry = 0;
    }
    a[i] = (a[i] + 1) % 64;
  }
}

void array_cp_(const array src, array dst) {
  int i;
  for(i=0; i<ARRAY_LEN; i++) {
    dst[i] = src[i];
  }
}

void array_zero(array a) {
  int i;
  for(i=0; i<ARRAY_LEN; i++) {
    a[i] = 0;
  }
}

int array_to_int(const array a) {
  int i;
  int res = 0;
  for (i=0; i<ARRAY_LEN; i++) {
    res += (a[i] & 63);
    res <<= 6;
  }
  return res;
}

int main() {
  int i, j;
  array message;
  array m_tmp;
  array key;
  int hash;
  array_zero(message);
  for (i=0; i<SPACE; i++) {
    for (j=0; j<SPACE; j++) {
      matrix[j] = 0;
    }
    array_zero(key);

    for (j=0; j<SPACE; j++) {
      array_cp_(message, m_tmp);
      encrypt_internal(m_tmp, key);
      hash = array_to_int(m_tmp);
      if (matrix[hash]) {
        printf("Noooooo...\n");
        exit(1);
      }
      array_increment(key);
    }

    array_increment(message);
  }
  exit(0);
}
