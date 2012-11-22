#include "../bunny_internals.h"
#include <stdlib.h>
#include <stdio.h>

#define SPACE 16777216

uint8_t matrix[SPACE];
array previous_key[SPACE];
array previous_encr[SPACE];

void array_print_(array a) {
  int index;
  for (index=0; index<ARRAY_LEN; index++) {
    g6_print(a[index]);
  }
  printf("\n");
}

void array_increment(array a) {
  int i;
  uint8_t carry = 1;
  for(i=0; i<ARRAY_LEN && carry == 1; i++) {
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
  int res = a[0] & 63;
  for (i=1; i<ARRAY_LEN; i++) {
    res <<= 6;
    res += (a[i] & 63);
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
  array_increment(message);
  for (i=0; i<SPACE; i++) {
    printf("Testing message: ");
    array_print_(message);
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
        printf("Message: ");
        array_print_(message);
        printf("Key one: ");
        array_print_(previous_key[hash]);
        printf("Key two: ");
        array_print_(key);
        printf("Encryption one: ");
        array_print_(previous_encr[hash]);
        printf("Encryption two: ");
        array_print_(m_tmp);
        printf("\n\n");
        //exit(1);
      }
      matrix[hash] = 1;
      array_cp_(key, previous_key[hash]);
      array_cp_(m_tmp, previous_encr[hash]);
      array_increment(key);
    }

    array_increment(message);
  }
  exit(0);
}
