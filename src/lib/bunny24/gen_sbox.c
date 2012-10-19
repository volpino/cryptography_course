#include "bunny_internals.h"
#include <stdio.h>

#define E 0x02  /* (0,0,0,0,1,0) */


void gen_SB1() {
    uint8_t i, j, result;

    printf("uint8_t SB1[] = {");

    for (i=0; i<64; i++) {
        result = 1;
        for (j=0; j<62; j++) {
            result = bunny_mul(result, i);
        }
        printf("%d%s", result, i < 63 ? ", " : "");
    }
    printf("};\n");
}


void gen_SB2() {
    uint8_t i, j, result;

    printf("uint8_t SB2[] = {");

    for (i=0; i<64; i++) {
        result = 1;
        for (j=0; j<5; j++) {
            result = bunny_mul(result, i);
        }
        printf("%d%s", result, i < 63 ? ", " : "");
    }
    printf("};\n");
}


void gen_SB3() {
    uint8_t i, j, result;

    printf("uint8_t SB3[] = {");

    for (i=0; i<64; i++) {
        result = 1;
        for (j=0; j<17; j++) {
            result = bunny_mul(result, i);
        }
        printf("%d%s", result, i < 63 ? ", " : "");
    }
    printf("};\n");
}


void gen_SB4() {
    uint8_t i, j, result;
    uint8_t e2 = bunny_mul(E, E);

    printf("uint8_t SB4[] = {0, ");

    for (i=1; i<64; i++) {
        result = 1;
        for (j=0; j<62; j++) {
            result = bunny_mul(result, i);
        }
        printf("%d%s", bunny_add(result, e2), i < 63 ? ", " : "");
    }
    printf("};\n");
}


void gen_SB1i() {
    uint8_t i, j, result;

    printf("uint8_t SB1i[] = {");

    for (i=0; i<64; i++) {
        result = 1;
        for (j=0; j<2; j++) {
            result = bunny_mul(result, i);
        }
        printf("%d%s", result, i < 63 ? ", " : "");
    }
    printf("};\n");
}


void gen_SB2i() {
    uint8_t i, j, result;

    printf("uint8_t SB2i[] = {");

    for (i=0; i<64; i++) {
        result = 1;
        for (j=0; j<59; j++) {
            result = bunny_mul(result, i);
        }
        printf("%d%s", result, i < 63 ? ", " : "");
    }
    printf("};\n");
}


void gen_SB3i() {
    uint8_t i, j, result;

    printf("uint8_t SB3i[] = {");

    for (i=0; i<64; i++) {
        result = 1;
        for (j=0; j<47; j++) {
            result = bunny_mul(result, i);
        }
        printf("%d%s", result, i < 63 ? ", " : "");
    }
    printf("};\n");
}


void gen_SB4i() {
    uint8_t i, j, result;
    uint8_t e2 = bunny_mul(E, E);

    printf("uint8_t SB4i[] = {0, ");

    for (i=1; i<64; i++) {
        result = i;
        result = bunny_add(result, e2);
        result = bunny_mul(result, result);
        printf("%d%s", result, i < 63 ? ", " : "");
    }
    printf("};\n");
}


int main() {
    gen_SB1();
    gen_SB2();
    gen_SB3();
    gen_SB4();

    gen_SB1i();
    gen_SB2i();
    gen_SB3i();
    gen_SB4i();
}
