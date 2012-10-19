#ifndef _FIELD_H_
#define _FIELD_H_

#include <stdint.h>

uint8_t gf_add(uint8_t a, uint8_t b);
uint8_t gf_mul(uint8_t a, uint8_t b, uint8_t n_bits, uint8_t poly);
uint8_t gf_rotate(uint8_t a, int8_t d, uint8_t n_bits);

#endif
