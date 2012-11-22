#ifndef _A5_1_INTERNALS_H
#define _A5_1_INTERNALS_H

#include "../lfsr.h"

/* This struct represents a set of registers */
typedef struct {
  int num;  /* length of registers and taps arrays */
  lfsr *registers;  /* array of registers */
  uint64_t* taps; /* array of one-bit masks, one for each register */
} reg_set_t;

#endif
