#include "field.h"
#include <stdio.h>  /* only for debugging */

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
        a &= maks;
        if (hi_bit_set)
            a ^= poly; //0x1b; /* x^4 + x^3 + x + 1 */
        b >>= 1;
    }
    return p;
}

/* rotate to the right */
uint8_t gf_rotate(uint8_t a, int8_t d, uint8_t n_bits) {
    uint8_t tmp;
    uint8_t i;
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

int main() {
    printf("3 + 11 = %d\n", g6_add(3, 11));
    printf("27 + 12 = %d\n", g6_add(27, 12));
    printf("27 * 11 = %d\n", g6_mul(27, 11));
    printf("6 * 5 = %d\n", g6_mul(6, 5));
    printf("R(0x11, 4) = %d\n", g6_rotate(0x11, 4));
    printf("R(0x11, 2) = %d\n", g6_rotate(0x11, 2));
    printf("R(0x11, -4) = %d\n", g6_rotate(0x11, -4));
    printf("R(0x11, -2) = %d\n", g6_rotate(0x11, -2));
}
