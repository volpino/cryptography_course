#include "field.h"
#include <stdio.h>  /* only for debugging */

/* Add two numbers in a GF(2^6) finite field */
uint8_t g6_add(uint8_t a, uint8_t b) {
    return (a ^ b) & 0x3f;
}

/* Subtract two numbers in a GF(2^6) finite field */
uint8_t g6_sub(uint8_t a, uint8_t b) {
    return (a ^ b) & 0x3f;
}

/* Multiply two numbers in the GF(2^6) finite field defined
 *  * by the polynomial x^6 + x^4 + x^3 + x + 1 */
uint8_t g6_mul(uint8_t a, uint8_t b) {
    uint8_t p = 0;
    uint8_t counter;
    uint8_t hi_bit_set;
    for (counter = 0; counter < 6; counter++) {
        if (b & 1)
            p ^= a;
        hi_bit_set = (a & 0x20);
        a <<= 1;
        if (hi_bit_set)
            a ^= 0x1b; /* x^4 + x^3 + x + 1 */
        b >>= 1;
    }
    return p;
}

uint8_t g6_rotate(uint8_t a, int8_t d) {
    uint8_t res = a;
    uint8_t tmp = a;
    uint8_t i;
    d = d % 6;
    if (d < 0) {
        d = 6 + d;
    }
    res >>= d;
    tmp <<= 6 - d;
    return res ^ (tmp % 64);
}

int main() {
    printf("3 + 11 = %d\n", g6_add(3, 11));
    printf("27 + 12 = %d\n", g6_add(27, 12));
    printf("3 * 11 = %d\n", g6_mul(3, 11));
    printf("6 * 5 = %d\n", g6_mul(6, 5));
    printf("R(0x11, 4) = %d\n", g6_rotate(0x11, 4));
    printf("R(0x11, 2) = %d\n", g6_rotate(0x11, 2));
    printf("R(0x11, -4) = %d\n", g6_rotate(0x11, -4));
    printf("R(0x11, -2) = %d\n", g6_rotate(0x11, -2));
}
