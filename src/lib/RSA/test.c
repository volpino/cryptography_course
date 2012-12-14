#include "../rsa.h"
#include <stdio.h>


int main() {
  char n_s[] = "33";
  char d_s[] = "3";
  char e_s[] = "7";
  char m_s[] = "27";
  char m2_s[] = "27";
  BIGNUM *n = NULL, *d = NULL, *m = NULL, *e = NULL, *m2 = NULL;
  unsigned char r = 5;
  BN_dec2bn(&n, n_s);
  BN_dec2bn(&d, d_s);
  BN_dec2bn(&m, m_s);
  BN_dec2bn(&m2, m2_s);
  BN_dec2bn(&e, e_s);

  rsa_encrypt(m, d, n);
  rsa_encrypt_secure(m2, d, e, n, &r, 1);

  BN_print_fp(stdout, m);
  printf("\n");
  BN_print_fp(stdout, m2);
  printf("\n\n");

  /* Try to generate a prime */
  /*generate_random_prime(m, 64);
  BN_print_fp(stdout, m);
  printf("\n");*/

  BN_free(n);
  BN_free(m);
  BN_free(d);
  BN_free(e);
  BN_free(m2);

  return 0;
}
