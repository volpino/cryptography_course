#include "../bunny_internals.h"
#include <stdlib.h>
#include <stdio.h>

#define SPACE 16777216

uint8_t matrix[SPACE];
b24_t previous_key[SPACE];
b24_t previous_encr[SPACE];

void b24_t_print_(b24_t a) {
  int index;
  for (index=0; index<B24_T_LEN; index++) {
    g6_print(a[index]);
  }
  printf("\n");
}

void b24_t_increment(b24_t a) {
  int i;
  uint8_t carry = 1;
  for(i=0; i<B24_T_LEN && carry == 1; i++) {
    if (a[i] != 63) {
      carry = 0;
    }
    a[i] = (a[i] + 1) % 64;
  }
}

void b24_t_cp_(const b24_t src, b24_t dst) {
  int i;
  for(i=0; i<B24_T_LEN; i++) {
    dst[i] = src[i];
  }
}

void b24_t_zero(b24_t a) {
  int i;
  for(i=0; i<B24_T_LEN; i++) {
    a[i] = 0;
  }
}

int b24_t_to_int(const b24_t a) {
  int i;
  int res = a[0] & 63;
  for (i=1; i<B24_T_LEN; i++) {
    res <<= 6;
    res += (a[i] & 63);
  }
  return res;
}

int main() {
  int i, j;
  b24_t message;
  b24_t m_tmp;
  b24_t key;
  int hash;
  b24_t_zero(message);
  b24_t_increment(message);
  for (i=0; i<SPACE; i++) {
    printf("Testing message: ");
    b24_t_print_(message);
    for (j=0; j<SPACE; j++) {
      matrix[j] = 0;
    }
    b24_t_zero(key);

    for (j=0; j<SPACE; j++) {
      b24_t_cp_(message, m_tmp);
      b24_encrypt(m_tmp, key);
      hash = b24_t_to_int(m_tmp);
      if (matrix[hash]) {
        printf("Noooooo...\n");
        printf("Message: ");
        b24_t_print_(message);
        printf("Key one: ");
        b24_t_print_(previous_key[hash]);
        printf("Key two: ");
        b24_t_print_(key);
        printf("Encryption one: ");
        b24_t_print_(previous_encr[hash]);
        printf("Encryption two: ");
        b24_t_print_(m_tmp);
        printf("\n\n");
        //exit(1);
      }
      matrix[hash] = 1;
      b24_t_cp_(key, previous_key[hash]);
      b24_t_cp_(m_tmp, previous_encr[hash]);
      b24_t_increment(key);
    }

    b24_t_increment(message);
  }
  exit(0);
}
