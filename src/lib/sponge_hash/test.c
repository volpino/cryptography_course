#include "../sponge_hash.h"
#include <stdio.h>
#include <assert.h>

#define BYTETOBINARYPATTERN "%d%d%d%d%d%d%d%d "
#define BYTETOBINARY(byte)  \
  (byte & 0x80 ? 1 : 0), \
  (byte & 0x40 ? 1 : 0), \
  (byte & 0x20 ? 1 : 0), \
  (byte & 0x10 ? 1 : 0), \
  (byte & 0x08 ? 1 : 0), \
  (byte & 0x04 ? 1 : 0), \
  (byte & 0x02 ? 1 : 0), \
  (byte & 0x01 ? 1 : 0)

void byte_print(uint8_t a) {
  printf(BYTETOBINARYPATTERN, BYTETOBINARY(a));
}


int main() {
  int i;
  uint8_t m[100];
  uint8_t o[20];
  /* 1011001100 */
  m[0] = 0xb3;
  sponge_hash(m, 1, o);
  printf("INPUT: %x\n", m[0]);

  printf("OUTPUT: \n");
  for (i=0; i<20; i++) {
    byte_print(o[i]);
  }
  printf("\n");

  assert(o[0] == 0x38);
  assert(o[1] == 0x3c);
  assert(o[2] == 0x4);
  assert(o[3] == 0xc4);
  assert(o[4] == 0xce);
  assert(o[5] == 0x47);
  assert(o[6] == 0x79);
  assert(o[7] == 0x45);
  assert(o[8] == 0xe7);
  assert(o[9] == 0x90);
  assert(o[10] == 0x89);
  assert(o[11] == 0xd);
  assert(o[12] == 0x8e);
  assert(o[13] == 0x72);
  assert(o[14] == 0x77);
  assert(o[15] == 0xfa);
  assert(o[16] == 0x68);
  assert(o[17] == 0xf1);
  assert(o[18] == 0x5b);
  assert(o[19] == 0xa3);

  m[0] = 0x47;
  m[1] = 0xc;
  m[2] = 0x39;
  m[3] = 0xcf;
  m[4] = 0x9a;
  m[5] = 0xfc;
  m[6] = 0xc0;

  sponge_hash(m, 7, o);

  printf("\n\nINPUT: \n");
  for (i=0; i<7; i++) {
    byte_print(m[i]);
  }
  printf("\n");

  printf("OUTPUT: \n");
  for (i=0; i<20; i++) {
    byte_print(o[i]);
  }
  printf("\n");

  assert(o[0] == 0x63);
  assert(o[1] == 0x87);
  assert(o[2] == 0xf1);
  assert(o[3] == 0xbe);
  assert(o[4] == 0xb5);
  assert(o[5] == 0xed);
  assert(o[6] == 0xb0);
  assert(o[7] == 0xd);
  assert(o[8] == 0x6);
  assert(o[9] == 0x48);
  assert(o[10] == 0x7e);
  assert(o[11] == 0x52);
  assert(o[12] == 0x43);
  assert(o[13] == 0x84);
  assert(o[14] == 0x34);
  assert(o[15] == 0x66);
  assert(o[16] == 0xb5);
  assert(o[17] == 0x60);
  assert(o[18] == 0x84);
  assert(o[19] == 0x64);

  return 0;
}
