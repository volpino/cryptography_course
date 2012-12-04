#include "rsa_internals.h"


BIGNUM* generate_random_prime() {
  int done = 0;
  int i;
  int not_prime;
  int checks = BN_prime_checks_for_size(KEY_LENGTH);
  BIGNUM *p, *q, *pm1, *pp1;
  BN_CTX *ctx;

  ctx = BN_CTX_new();
  BN_CTX_init(ctx);

  p = BN_new();
  q = BN_new();
  pm1 = BN_new();
  pp1 = BN_new();

  /* while a suitable prime is not found */
  while (!done) {
    /* pick a random q with first and last bits as 1, 511 bits */

    // WRITEME

    /* generate p = 2 * q + 1*/
    BN_zero(p);
    BN_lshift1(p, q);
    BN_copy(pm1, p);  /* p-1 */
    BN_add_word(p, 1);
    BN_copy(pp1, p);  /* p+1 */
    BN_add_word(pp1, 1);
    BN_sub_word(q, 1); /* q-1 */

    /* check that both p-1 and q-1 don't have small factors
     * Note: exclude 2 from primes
     */
    not_prime = 0;
    for (i=1; i<N_PRIMES; i++) {
      if ((BN_mod_word(pm1, (BN_ULONG)primes[i]) == 0) ||
          (BN_mod_word(pp1, (BN_ULONG)primes[i]) == 0) ||
          (BN_mod_word(q, (BN_ULONG)primes[i]) == 0)) {
        not_prime = 1;
        break;
      }
    }
    if (not_prime) {
      continue;
    }

    /* check that (p-1)/2 is prime (it's ok to do just p/2, integer division)*/
    BN_rshift1(q, p);

    if (BN_is_prime(p, checks, NULL, NULL, NULL)) {
      done = 1;
    }
  }

  BN_free(q);
  BN_free(pm1);
  BN_free(pp1);
  return p;
}
