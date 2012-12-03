#ifndef _RSA_H_
#define _RSA_H_

#define KEY_LENGTH 512
#define N_PRIMES 2048

#include "../bunny24_prng.h"
#include <openssl/bn.h>

extern const int primes[N_PRIMES];

void rsa_encrypt(BIGNUM* m, const BIGNUM* e, const BIGNUM* n);
void rsa_decrypt(BIGNUM* m, const BIGNUM* e, const BIGNUM* n);

#endif
