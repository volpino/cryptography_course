#ifndef _A5_1_INTERNALS_H
#define _A5_1_INTERNALS_H

#include "../lfsr.h"

typedef struct {
  int num;
  lfsr *registers;
  uint64_t taps; // one one-bit mask for each register
} reg_set;

#endif
