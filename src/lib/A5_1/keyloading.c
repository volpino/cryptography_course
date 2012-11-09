#include "../lfsr.h"
#include <stdlib.h>

#define KEY_LEN 64
#define FRAME_LEN 22

void init(lfsr* registers, int len) {
  int i;
  for (i=0; i<len; i++) {
    registers[i].state = 0;
  }
}

void keyloading(lfsr* registers, int len,
                uint8_t* key, int key_len) {
  int i, j;
  uint64_t bit;
  for (i=0; i<key_len; i++) {
    bit = key[i/8] & 0x80;
    key[i/8] <<= 1;
    for (j=0; j<len; j++) {
      lfsr_rotate( &(registers[j]), NULL, 0 );
      registers[j].state ^= bit;
    }
  }
}
