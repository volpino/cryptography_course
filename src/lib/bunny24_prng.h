#ifndef _BUNNY24_PRNG_H_
#define _BUNNY24_PRNG_H_

#include <stdint.h>

void bunny24_prng(const uint8_t* seed, int n, uint8_t* out, int m);

#endif
