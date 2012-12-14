#include "../rsa.h"
#include <assert.h>

/*
  Operates in-place, the result is stored over m
 */
void rsa_encrypt(BIGNUM* m, const BIGNUM* e, const BIGNUM* n) {
  BIGNUM* tmp;
  BN_CTX *ctx;

  assert(BN_cmp(m, n) < 0); /* assert m < n */

  ctx = BN_CTX_new();
  tmp = BN_new();
  BN_copy(tmp, m);  /* tmp = m */

  BN_mod_exp(m, tmp, e, n, ctx); /* m = tmp ^ e (mod n) */

  BN_free(tmp);
  BN_CTX_free(ctx);
}

void rsa_decrypt(BIGNUM* m, const BIGNUM* e, const BIGNUM* n) {
  rsa_encrypt(m, e, n);
}

/* encrypts (or decrypts) with private key, not sensitive to
   timing attacks
*/
void rsa_encrypt_secure(BIGNUM* m, const BIGNUM* d,
                        const BIGNUM* e, const BIGNUM* n,
                        const unsigned char * r_bin, int r_len) {
  BN_CTX *ctx;
  BIGNUM *tmp = BN_new();
  BIGNUM *r = BN_new();
  BIGNUM *r_inv = BN_new();

  ctx = BN_CTX_new();
  BN_bin2bn(r_bin, r_len, r);
  BN_mod(r, r, n, ctx); /* r = r % n */

  BN_mod(tmp, r, n, ctx);
  while (! BN_is_one(r)) {
    BN_mod_add(r, r, BN_value_one(), n, ctx);
    BN_mod(tmp, r, n, ctx);
  }

  BN_mod_inverse(r_inv, r, n, ctx);

  printf(" r = ");BN_print_fp(stdout, r);
  printf(" r_inv = ");BN_print_fp(stdout, r_inv);
  printf(" n = ");BN_print_fp(stdout, n);
  printf("\n");

  BN_mod_exp(r, r, e, n, ctx);  /* r = r^e % n */
  BN_mod_mul(m, m, r, n, ctx);  /* m = m * r % n */

  rsa_encrypt(m, d, n);

  BN_mod_mul(m, m, r_inv, n, ctx);

  BN_free(r);
  BN_free(r_inv);
  BN_free(tmp);
  BN_CTX_free(ctx);
}
