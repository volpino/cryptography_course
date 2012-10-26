#ifndef _BUNNY_INTERNALS_H_
#define _BUNNY_INTERNALS_H_

#include "../field.h"

#define ARRAY_LEN 4
#define ROUND_NUM 15

typedef uint8_t array[ARRAY_LEN];

#define bunny_add(a, b) gf_add((a), (b))
#define bunny_mul(a, b) gf_mul((a), (b), 6, 0x1b) /* (x^6+) x^4 + x^3 + x + 1 */
#define bunny_rotate(a, b) gf_rotate((a), (b), 6)

#define array_print(a)                          \
  for (index=0; index<ARRAY_LEN; index++) {     \
    g6_print(a[index]);                         \
  }                                             \
  printf("\n");

void key_schedule(const array key, array* result);
void mixing_layer(array c);
void mixing_layer_inv(array c);

void encrypt_internal(array m, const array k);
void decrypt_internal(array m, const array k);

/* Mixing layer matrix */
extern const uint8_t lambda[ARRAY_LEN][ARRAY_LEN];
/* Transposal of mix layer matrix (useful for matrix multiplication) */
extern const uint8_t lambdaT[ARRAY_LEN][ARRAY_LEN];

extern const uint8_t lambdaT_i[ARRAY_LEN][ARRAY_LEN];

/* S-Boxes */
extern const uint8_t SB1[];
extern const uint8_t SB2[];
extern const uint8_t SB3[];
extern const uint8_t SB4[];
extern const uint8_t SB1i[];
extern const uint8_t SB2i[];
extern const uint8_t SB3i[];
extern const uint8_t SB4i[];

#endif
