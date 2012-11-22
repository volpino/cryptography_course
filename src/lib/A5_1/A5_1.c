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

/* note: to get A5/1 poly values I ran, for each poly:
   hex(eval('x^22 + x^21 + 1'.replace('x', '2').replace('^', '**')))
*/

void a5_1(uint8_t* key, uint8_t* output, int out_bits) {
  int i, out_len;
  uint8_t out_bit;

  /* uint64_t f = 0xb2000;  frame vector */
  uint8_t f[3];
  f[0] = 0x2c;
  f[1] = 0x80;
  f[3] = 0x0;

  lfsr registers[3];
  uint64_t taps[3];
  reg_set_t reg_set;

  registers[0].poly_deg = 19;
  registers[0].poly = 0xe4001;
  registers[0].state = 0;
  registers[1].poly_deg = 22;
  registers[1].poly = 0x600001;
  registers[1].state = 0;
  registers[2].poly_deg = 23;
  registers[2].poly = 0xe00101;
  registers[2].state = 0;

  taps[0] = 256;
  taps[1] = 1024;
  taps[2] = 1024;

  reg_set.num = 3;
  reg_set.registers = registers;
  reg_set.taps = taps;

  out_len = (out_bits/8) + ((out_bits%8!=0) ? 1 : 0);
  for (i=0; i<out_len; i++) {
    output[i] = 0;
  }

  /* key loading */
  keyloading(&reg_set, key, 64);

  /* initialization vector loading*/
  keyloading(&reg_set, f, 22);

  /* 100 updates with no output */
  for (i=0; i<100; i++) {
    shift_majority_1(&reg_set);
  }

  /* stream generation */
  for (i=0; i<out_bits; i++) {
    out_bit = shift_majority_1(&reg_set);
    out_bit = (out_bit ? 1 : 0);
    output[i/8] ^= (out_bit << (7 - i % 8));
  }

}
