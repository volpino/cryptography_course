#include "../lfsr.h"
#include "A5_1_internals.h"
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

/* runs 1 shift operation, returns the bit (either all zeroes, or not) */
uint8_t shift_majority_1(reg_set* regs) {
  int i, count_0 = 0, count_1 = 0;
  uint8_t out_bit, temp, tap, majority;
  for(i=0; i<regs->num; i++) {
    if (regs->registers[i].state & regs->taps[i])
      count_1 ++;
    else
      count_0 ++;
  }
  majority = (count_1 > count_0) ? 1 : 0;
  out_bit = 0x0;
  for(i=0; i<regs->num; i++) {
    tap = (regs->registers[i].state & regs->taps[i]) ? 1 : 0;
    if (!(tap ^ majority)) {/* if tap agrees with majority */
      lfsr_rotate(&(regs->registers[i]), &temp, 1);
      out_bit ^= (temp ? 1 : 0);
    }
  }
  return out_bit;
}
