#include "../bunny_internals.h"
#include <stdlib.h>
#include <stdio.h>

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
  array message;
  array key1, key2;
  array enc1, enc2;

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

  array_cp_(message, enc1);
  encrypt_internal(enc1, key1);
  array_cp_(message, enc2);
  encrypt_internal(enc2, key2);

  printf("Message: ");
  array_print_(message);
  printf("Key one: ");
  array_print_(key1);
  printf("Key two: ");
  array_print_(key2);
  printf("Encryption one: ");
  array_print_(enc1);
  printf("Encryption two: ");
  array_print_(enc2);

  exit(0);
}
