#ifndef _BUNNY24_H_
#define _BUNNY24_H_

#include <stdint.h>

void bunny24_encrypt_cbc(uint8_t* m, int n, uint8_t* k, uint8_t* iv);
void bunny24_decrypt_cbc(uint8_t* m, int n, uint8_t* k, uint8_t* iv);

#endif
