#ifndef _RSA_H_
#define _RSA_H_

#define N_PRIMES 2048

#include <openssl/bn.h>

extern const int primes[N_PRIMES];

void generate_random_prime(BIGNUM *p);
void rsa_encrypt(BIGNUM* m, const BIGNUM* e, const BIGNUM* n);
void rsa_decrypt(BIGNUM* m, const BIGNUM* e, const BIGNUM* n);

#endif
