#include "../rsa.h"


int main() {
  BIGNUM *p, *q, *n, *e, *d, *phi, *pm1, *qm1;
  BN_CTX *ctx = BN_CTX_new();

  p = BN_new();
  q = BN_new();
  n = BN_new();
  e = BN_new();
  d = BN_new();
  phi = BN_new();
  pm1 = BN_new();
  qm1 = BN_new();

  generate_random_prime(p);
  generate_random_prime(q);

  BN_mul(n, p, q, ctx);

  BN_sub(pm1, p, BN_value_one());
  BN_sub(qm1, q, BN_value_one());

  BN_zero(e);
  BN_add_word(e, (BN_ULONG) 65539);

  BN_mul(phi, pm1, qm1, ctx);

  BN_mod_inverse(d, e, phi, ctx);

  printf("N: ");
  BN_print_fp(stdout, n);
  printf("\n");

  printf("P: ");
  BN_print_fp(stdout, p);
  printf("\n");

  printf("Q: ");
  BN_print_fp(stdout, q);
  printf("\n");

  printf("E: ");
  BN_print_fp(stdout, e);
  printf("\n");

  printf("D: ");
  BN_print_fp(stdout, d);
  printf("\n");

  BN_free(p);
  BN_free(q);
  BN_free(n);
  BN_free(e);
  BN_free(d);
  BN_free(phi);
  BN_free(pm1);
  BN_free(qm1);

  BN_CTX_free(ctx);

  return 0;
}