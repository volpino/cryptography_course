#include "../field.h"
#include <stdio.h>

#define E 0x2;  /* (0,0,0,0,1,0) */

void SB1() {
    uint8_t i, j, result;

    printf("uint8_t SB1 = {");

    for (i=0; i<0x40; i++) {
        result = i;
        for (j=0; j<62; j++) {
            result = gf_mul(result, i);
        }
        printf("%d, ", result);
    }
    printf("}\n");
}


void SB2() {
    uint8_t i, j, result;

    printf("uint8_t SB2 = {");

    for (i=0; i<0x40; i++) {
        result = i;
        for (j=0; j<62; j++) {
            result = gf_mul(result, i);
        }
        printf("%d, ", gf_add(result, gf_mul(E, E)));
    }
    printf("}\n");
}


void SB3() {
    uint8_t i, j, result;

    printf("uint8_t SB1 = {");

    for (i=0; i<0x40; i++) {
        result = i;
        for (j=0; j<5; j++) {
            result = gf_mul(result, i);
        }
        printf("%d, ", result);
    }
    printf("}\n");
}


void SB4() {
    uint8_t i, j, result;

    printf("uint8_t SB1 = {");

    for (i=0; i<0x40; i++) {
        result = i;
        for (j=0; j<17; j++) {
            result = gf_mul(result, i);
        }
        printf("%d, ", result);
    }
    printf("}\n");
}
