#include "rsa_internals.h"
#include <stdio.h>


int main() {
  char n_s[] = "33";
  char d_s[] = "3";
  char m_s[] = "27";
  BIGNUM *n = NULL, *d = NULL, *m = NULL;
  BN_dec2bn(&n, n_s);
  BN_dec2bn(&d, d_s);
  BN_dec2bn(&m, m_s);

  rsa_encrypt(m, d, n);

  BN_print_fp(stdout, m);
  printf("\n");

  BN_free(n);
  BN_free(m);
  BN_free(d);

  return 0;
}
