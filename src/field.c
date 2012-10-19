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
    g6_print(g6_add(a, b));
    printf("\nA * B: ");
    g6_print(g6_mul(a, b));
    printf("\n");
}

int main() {
    field_test();
}
