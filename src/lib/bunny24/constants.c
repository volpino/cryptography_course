#include "bunny_internals.h"

/* Mixing layer matrix */
const uint8_t lambda[ARRAY_LEN][ARRAY_LEN] = {
    {0x23, 0x3b, 0x38, 0x3d},
    {0x0d, 0x3c, 0x16, 0x18},
    {0x03, 0x20, 0x17, 0x37},
    {0x2c, 0x26, 0x38, 0x13}
};

/* Transposal of mix layer matrix (useful for matrix multiplication) */
const uint8_t lambdaT[ARRAY_LEN][ARRAY_LEN] = {
    {0x23, 0x0d, 0x03, 0x2c},
    {0x3b, 0x3c, 0x20, 0x26},
    {0x38, 0x16, 0x17, 0x38},
    {0x3d, 0x18, 0x37, 0x13}
};

/* Mix layer inverse matrix (useful for matrix multiplication) */
const uint8_t lambda_i[ARRAY_LEN][ARRAY_LEN] = {
  {0x1d, 0x3, 0xb, 0x19},
  {0x11, 0x2f, 0x3e, 0x3d},
  {0x7, 0x17, 0x39, 0xc},
  {0xa, 0x3a, 0xd, 0x29}
};

/* Transposal of mix layer inverse matrix (useful for matrix multiplication) */
const uint8_t lambdaT_i[ARRAY_LEN][ARRAY_LEN] = {
  {0x1d, 0x11, 0x7, 0xa},
  {0x3, 0x2f, 0x17, 0x3a},
  {0xb, 0x3e, 0x39, 0xd},
  {0x19, 0x3d, 0xc, 0x29}
};

/* S-Boxes */
const uint8_t SB1[] = {0, 1, 45, 54, 59, 18, 27, 30, 48, 10, 9, 49, 32, 62, 15, 14, 24, 51, 5, 58, 41, 56, 53, 35, 16, 50, 31, 6, 42, 38, 7, 26, 12, 63, 52, 23, 47, 61, 29, 43, 57, 20, 28, 39, 55, 2, 60, 36, 8, 11, 25, 17, 34, 22, 3, 44, 21, 40, 19, 4, 46, 37, 13, 33};
const uint8_t SB2[] = {0, 1, 32, 51, 49, 3, 63, 31, 36, 4, 59, 9, 62, 45, 15, 14, 7, 5, 54, 38, 8, 57, 23, 52, 30, 61, 16, 33, 58, 42, 26, 24, 13, 43, 22, 34, 41, 60, 28, 27, 55, 48, 19, 6, 56, 12, 50, 20, 47, 10, 37, 18, 53, 35, 17, 21, 40, 44, 29, 11, 25, 46, 2, 39};
const uint8_t SB3[] = {0, 1, 38, 54, 37, 18, 43, 13, 20, 50, 25, 46, 42, 58, 15, 14, 32, 51, 5, 7, 47, 10, 34, 22, 12, 56, 2, 39, 24, 26, 62, 45, 28, 27, 35, 53, 8, 57, 31, 63, 4, 36, 16, 33, 11, 29, 55, 48, 41, 60, 21, 17, 23, 52, 3, 49, 9, 59, 30, 61, 44, 40, 19, 6};
const uint8_t SB4[] = {4, 5, 41, 50, 63, 22, 31, 26, 52, 14, 13, 53, 36, 58, 11, 10, 28, 55, 1, 62, 45, 60, 49, 39, 20, 54, 27, 2, 46, 34, 3, 30, 8, 59, 48, 19, 43, 57, 25, 47, 61, 16, 24, 35, 51, 6, 56, 32, 12, 15, 29, 21, 38, 18, 7, 40, 17, 44, 23, 0, 42, 33, 9, 37};

const uint8_t SB1i[] = {0, 1, 45, 54, 59, 18, 27, 30, 48, 10, 9, 49, 32, 62, 15, 14, 24, 51, 5, 58, 41, 56, 53, 35, 16, 50, 31, 6, 42, 38, 7, 26, 12, 63, 52, 23, 47, 61, 29, 43, 57, 20, 28, 39, 55, 2, 60, 36, 8, 11, 25, 17, 34, 22, 3, 44, 21, 40, 19, 4, 46, 37, 13, 33};
const uint8_t SB2i[] = {0, 1, 62, 5, 9, 17, 43, 16, 20, 11, 49, 59, 45, 32, 15, 14, 26, 54, 51, 42, 47, 55, 34, 22, 31, 60, 30, 39, 38, 58, 24, 7, 2, 27, 35, 53, 8, 50, 19, 63, 56, 36, 29, 33, 57, 13, 61, 48, 41, 4, 46, 3, 23, 52, 18, 40, 44, 21, 28, 10, 37, 25, 12, 6};
const uint8_t SB3i[] = {0, 1, 26, 54, 40, 18, 63, 19, 36, 56, 21, 44, 24, 7, 15, 14, 42, 51, 5, 62, 8, 50, 23, 52, 28, 10, 29, 33, 32, 45, 58, 38, 16, 43, 22, 34, 41, 4, 2, 27, 61, 48, 12, 6, 60, 31, 11, 20, 47, 55, 9, 17, 53, 35, 3, 46, 25, 37, 13, 57, 49, 59, 30, 39};
const uint8_t SB4i[] = {59, 18, 27, 30, 0, 1, 45, 54, 32, 62, 15, 14, 48, 10, 9, 49, 41, 56, 53, 35, 24, 51, 5, 58, 42, 38, 7, 26, 16, 50, 31, 6, 47, 61, 29, 43, 12, 63, 52, 23, 55, 2, 60, 36, 57, 20, 28, 39, 34, 22, 3, 44, 8, 11, 25, 17, 46, 37, 13, 33, 21, 40, 19, 4};
