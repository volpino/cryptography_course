#include "rsa_internals.h"
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
