#ifndef _BUNNY_INTERNALS_H_
#define _BUNNY_INTERNALS_H_

#include "../field.h"

#define B24_T_LEN 4
#define ROUND_NUM 15

typedef uint8_t b24_t[B24_T_LEN];

#define bunny_add(a, b) gf_add((a), (b))
#define bunny_mul(a, b) gf_mul((a), (b), 6, 0x1b) /* (x^6+) x^4 + x^3 + x + 1 */
#define bunny_rotate(a, b) gf_rotate((a), (b), 6)

void key_schedule(const b24_t key, b24_t* result);
void mixing_layer(b24_t c);
void mixing_layer_inv(b24_t c);

void encrypt_internal(b24_t m, const b24_t k);
void decrypt_internal(b24_t m, const b24_t k);

/* Mixing layer matrix */
extern const uint8_t lambda[B24_T_LEN][B24_T_LEN];
/* Transposal of mix layer matrix (useful for matrix multiplication) */
extern const uint8_t lambdaT[B24_T_LEN][B24_T_LEN];

extern const uint8_t lambdaT_i[B24_T_LEN][B24_T_LEN];

/* S-Boxes */
extern const uint8_t SB1[];
extern const uint8_t SB2[];
extern const uint8_t SB3[];
extern const uint8_t SB4[];
extern const uint8_t SB1i[];
extern const uint8_t SB2i[];
extern const uint8_t SB3i[];
extern const uint8_t SB4i[];

/* Conversions */
void byte_to_g6(uint8_t* inp, int n, b24_t* out);
void g6_to_byte(b24_t* inp, uint8_t* out, int n);

void bunny24_encrypt_cbc(uint8_t* m, int n, uint8_t* k, uint8_t* iv);
void bunny24_decrypt_cbc(uint8_t* m, int n, uint8_t* k, uint8_t* iv);

#endif
