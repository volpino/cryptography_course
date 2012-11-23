#ifndef _BUNNY_INTERNALS_H_
#define _BUNNY_INTERNALS_H_

#include "../field.h"
#include "../bunny24.h"

#define ROUND_NUM 15

#define bunny_add(a, b) gf_add((a), (b))
#define bunny_mul(a, b) gf_mul((a), (b), 6, 0x1b) /* (x^6+) x^4 + x^3 + x + 1 */
#define bunny_rotate(a, b) gf_rotate((a), (b), 6)

void key_schedule(const b24_t key, b24_t* result);
void mixing_layer(b24_t c);
void mixing_layer_inv(b24_t c);

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


#endif
