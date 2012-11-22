#include "../lfsr.h"
#include "A5_1_internals.h"
#include <stdlib.h>

#define KEY_LEN 64
#define FRAME_LEN 22

void init(reg_set_t* reg_set) {
  int i;
  for (i=0; i<reg_set->num; i++) {
    (reg_set->registers[i]).state = 0;
  }
}

void keyloading(reg_set_t* reg_set,
                uint8_t* key, int key_len) {
  int i, j;
  uint64_t bit;
  for (i=0; i<key_len; i++) {
    bit = key[i/8] & 0x80;
    key[i/8] <<= 1;  /* This has side effects on the key! */
    for (j=0; j<reg_set->num; j++) {
      lfsr_rotate(&(reg_set->registers[j]), NULL, 0);
      (reg_set->registers[j]).state ^= (bit ? 1 : 0);
    }
  }
}

/* runs 1 shift operation, returns the bit (either all zeroes, or not) */
uint8_t shift_majority_1(reg_set_t* reg_set) {
  int i, count_0 = 0, count_1 = 0;
  uint8_t out_bit, temp, tap, majority;
  for (i=0; i<reg_set->num; i++) {
    if (reg_set->registers[i].state & reg_set->taps[i])  /* if the tap bit is one */
      count_1++;
    else
      count_0++;
  }
  majority = (count_1 > count_0) ? 1 : 0;
  out_bit = 0;
  for (i=0; i<reg_set->num; i++) {
    /* take the value of the tap */
    tap = (reg_set->registers[i].state & reg_set->taps[i]) ? 1 : 0;

    if (tap == majority) { /* if tap agrees with majority */
      lfsr_rotate(&(reg_set->registers[i]), &temp, 1);
      out_bit ^= (temp ? 1 : 0);
    }
  }
  return out_bit;
}


void a5_1(uint64_t key, int out_len) {

}
