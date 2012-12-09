#include "../rsa.h"


int main(int argc, char **argv) {
  BIGNUM *p, *q, *pmq, *n, *e, *d, *phi, *pm1, *qm1;
  int key_len;

  BN_CTX *ctx = BN_CTX_new();

  p = BN_new();
  q = BN_new();
  pmq =  BN_new();
  n = BN_new();
  e = BN_new();
  d = BN_new();
  phi = BN_new();
  pm1 = BN_new();
  qm1 = BN_new();

  if (argc != 2) {
    printf("./gen_key [key_len]\n");
    return 1;
  }

  key_len = atoi(argv[1]) / 8;

  printf("[+] Key length: %d bytes\n", key_len);

  generate_random_prime(p, key_len / 2);
  printf("\n[+] Generated p...\n");

  do {
    generate_random_prime(q, key_len / 2);
    printf("\n[+] Generated q...\n");
    if (BN_cmp(p, q) < 0) {
      BN_sub(pmq, p, q);
    }
    else {
      BN_sub(pmq, q, p);
    }
  } while (BN_num_bits(pmq) < (key_len*8) / 4);
  printf("[+] |p - q| = %d bits\n", BN_num_bits(pmq));

  BN_mul(n, p, q, ctx);

  BN_sub(pm1, p, BN_value_one());
  BN_sub(qm1, q, BN_value_one());

  BN_zero(e);
  BN_add_word(e, (BN_ULONG) (65539));

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
  BN_free(pmq);

  BN_CTX_free(ctx);

  return 0;
}
