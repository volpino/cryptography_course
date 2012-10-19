#include "bunny_internals.h"
#include <stdio.h>

#define E 0x2  /* (0,0,0,0,1,0) */


void SB1() {
    uint8_t i, j, result;

    printf("uint8_t SB1[] = {");

    for (i=0; i<64; i++) {
        result = i;
        for (j=0; j<62; j++) {
            result = bunny_mul(result, i);
        }
        printf("%d%s", result, i < 63 ? ", " : "");
    }
    printf("};\n");
}


void SB2() {
    uint8_t i, j, result;

    printf("uint8_t SB2[] = {");

    for (i=0; i<64; i++) {
        result = i;
        for (j=0; j<5; j++) {
            result = bunny_mul(result, i);
        }
        printf("%d%s", result, i < 63 ? ", " : "");
    }
    printf("};\n");
}


void SB3() {
    uint8_t i, j, result;

    printf("uint8_t SB3[] = {");

    for (i=0; i<64; i++) {
        result = i;
        for (j=0; j<17; j++) {
            result = bunny_mul(result, i);
        }
        printf("%d%s", result, i < 63 ? ", " : "");
    }
    printf("};\n");
}


void SB4() {
    uint8_t i, j, result;
    uint8_t e2 = bunny_mul(E, E);

    printf("uint8_t SB4[] = {");

    for (i=0; i<64; i++) {
        result = i;
        for (j=0; j<62; j++) {
            result = bunny_mul(result, i);
        }
        printf("%d%s", bunny_add(result, e2), i < 63 ? ", " : "");
    }
    printf("};\n");
}


int main() {
    printf("%d\n", bunny_mul(2,2));

    SB1();
    SB2();
    SB3();
    SB4();
}
