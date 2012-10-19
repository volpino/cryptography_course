#ifndef _BUNNY_INTERNALS_H_
#define _BUNNY_INTERNALS_H_

#include "../field.h"

#define bunny_add(a, b) gf_add((a), (b))
#define bunny_mul(a, b) gf_mul((a), (b), 6, 0x1b)  /* x^4 + x^3 + x + 1 */
#define bunny_rotate(a, b) gf_rotate((a), (b), 6)

uint8_t lambda[4][4] = {
    {0x23, 0x3b, 0x38, 0x3d},
    {0x0d, 0x3c, 0x16, 0x18},
    {0x03, 0x20, 0x17, 0x37},
    {0x2c, 0x26, 0x38, 0x13}
};

#endif
