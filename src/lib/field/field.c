#include "field.h"
#include <stdio.h>  /* only for debugging */


#define G6_BYTETOBINARYPATTERN "(%d,%d,%d,%d,%d,%d)"
#define G6_BYTETOBINARY(byte)  \
  (byte & 0x20 ? 1 : 0), \
  (byte & 0x10 ? 1 : 0), \
  (byte & 0x08 ? 1 : 0), \
  (byte & 0x04 ? 1 : 0), \
  (byte & 0x02 ? 1 : 0), \
  (byte & 0x01 ? 1 : 0)

/* given the number of bits, return the proper mask */
uint8_t size2mask(uint8_t n_bits){
  uint8_t res = 0;
  for (; n_bits > 0; n_bits --)
    res = (res << 1) + 1;
  return res;
}

/* given the number of bits, return the mask for most-significative bit */
uint8_t size2msb_mask(uint8_t n_bits){
  // cast required for this to work properly with negative shifts
  return ((uint8_t)1) << (n_bits-1);
}

/* Add two numbers in a GF(2^6) finite field */
uint8_t gf_add(uint8_t a, uint8_t b) {
  return (a ^ b);
}

/* Multiply two numbers in the GF(2^6) finite field defined
 *  * by the polynomial x^6 + x^4 + x^3 + x + 1 */
uint8_t gf_mul(uint8_t a, uint8_t b, uint8_t n_bits, uint8_t poly) {
    uint8_t p = 0;
    uint8_t counter;
    uint8_t hi_bit_set;
    uint8_t mask = size2mask(n_bits);
    uint8_t msb_mask = size2msb_mask(n_bits);
    for (counter = 0; counter < 6; counter++) {
        if (b & 1)
            p ^= a;
        hi_bit_set = (a & msb_mask);
        a <<= 1;
        a &= mask;
        if (hi_bit_set)
            a ^= poly; //0x1b; /* x^4 + x^3 + x + 1 */
        b >>= 1;
    }
    return p;
}

/* rotate to the right */
uint8_t gf_rotate(uint8_t a, int8_t d, uint8_t n_bits) {
    uint8_t tmp;
    a &= (size2mask(n_bits));
    tmp = a;
    d %= n_bits;
    if (d < 0) {
        d += n_bits;
    }
    a >>= d;
    tmp <<= n_bits - d;
    return a ^ (tmp & size2mask(n_bits));
}

uint8_t g6_input() {
    uint8_t a0, a1, a2, a3, a4, a5;
    scanf("(%c,%c,%c,%c,%c,%c)", &a0, &a1, &a2, &a3, &a4, &a5);
    return (a0 - '0')*0x20 + (a1 - '0')*0x10 + (a2 - '0')*0x08 + \
           (a3 - '0')*0x04 + (a4 - '0')*0x02 + (a5 - '0')*0x01;
}

void g6_print(uint8_t a) {
    printf(G6_BYTETOBINARYPATTERN, G6_BYTETOBINARY(a));
}

void field_test() {
    uint8_t a, b;

    printf("Gimme the A vector: ");
    a = g6_input();
    scanf("\n");
    printf("Gimme the B vector: ");
    scanf("\n");
    b = g6_input();

    printf("\nA + B: ");
    g6_print(gf_add(a, b));
    printf("\nA * B: ");
    g6_print(gf_mul(a, b, 6, 0x1b));
    printf("\n");
}

