#ifndef _BUNNY24_H_
#define _BUNNY24_H_

#include <stdint.h>

#define B24_T_LEN 4

typedef uint8_t b24_t[B24_T_LEN];

void bunny24_encrypt_cbc(uint8_t* m, int n, uint8_t* k, uint8_t* iv);
void bunny24_decrypt_cbc(uint8_t* m, int n, uint8_t* k, uint8_t* iv);

void b24_encrypt(b24_t m, const b24_t k);
void b24_decrypt(b24_t m, const b24_t k);

#endif
