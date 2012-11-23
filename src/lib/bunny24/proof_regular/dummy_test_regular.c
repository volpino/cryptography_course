#include "../bunny_internals.h"
#include <stdlib.h>
#include <stdio.h>

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
  b24_t message;
  b24_t key1, key2;
  b24_t enc1, enc2;

  message[0] = 1;
  message[1] = message[2] = message[3] = 0;

  key1[0] = 14;
  key1[1] = 25;
  key1[2] = 0;
  key1[3] = 0;

  key2[0] = 48;
  key2[1] = 16;
  key2[2] = 1;
  key2[3] = 0;

  b24_t_cp_(message, enc1);
  encrypt_internal(enc1, key1);
  b24_t_cp_(message, enc2);
  encrypt_internal(enc2, key2);

  printf("Message: ");
  b24_t_print_(message);
  printf("Key one: ");
  b24_t_print_(key1);
  printf("Key two: ");
  b24_t_print_(key2);
  printf("Encryption one: ");
  b24_t_print_(enc1);
  printf("Encryption two: ");
  b24_t_print_(enc2);

  exit(0);
}
