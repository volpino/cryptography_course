#include "rsa_internals.h"
#include "../bunny24_prng.h"

#define KEY_LENGTH 64  /* bytes */
#define KEY_LENGTH_3 66  /* bytes */
#define SEED_LENGTH 64  /* bytes */

void generate_random_prime(BIGNUM *p) {
  int done, found;
  int i, j;
  int not_prime;
  int checks = BN_prime_checks_for_size(KEY_LENGTH * 8);
  uint8_t random_bits[KEY_LENGTH_3];
  uint8_t seed[SEED_LENGTH];
  uint8_t iv[3] = {0x00, 0x00, 0x00};
  FILE *fp;

  BIGNUM *q, *pm1, *pp1;
  BN_CTX *ctx;

  ctx = BN_CTX_new();
  BN_CTX_init(ctx);

  q = BN_new();
  pm1 = BN_new();
  pp1 = BN_new();

  found = 0;
  while (!found) {  /* This is needed because sometimes the PRNG "loops" */
    /* Set the seed only once */
    fp = fopen("/dev/random", "r");
    for (i=0; i<SEED_LENGTH; i++) {
      fscanf(fp, "%c", &(seed[i]));
    }
    fclose(fp);

    for (i=0; i<SEED_LENGTH; i++) {
      printf("%x ", seed[i]);
    }
    printf(" ");

    j = 0;
    done = 0;
    /* while a suitable prime is not found */
    while (!done) {
      j++;

      /* If you do more than <magic_number> iterations change seed */
      if (j >= 2*1e6) {
        printf("LOOP INFINITO\n");
        done = 1;
      }

      /* pick a random q with first bit as 0, second and last as 1 */
      bunny24_prng(seed, SEED_LENGTH, iv, random_bits, KEY_LENGTH_3);

      iv[0] = random_bits[KEY_LENGTH_3-3];  /* Set the IV for the next loop */
      iv[1] = random_bits[KEY_LENGTH_3-2];
      iv[2] = random_bits[KEY_LENGTH_3-1];

      /* Set the seed for the next loop */
      /*for (i=0; i<SEED_LENGTH; i++) {
        seed[i] = 0x00;
      }*/

      random_bits[0] &= 0x7F;  /* First bit as 0 0x7F=01111111 */
      random_bits[0] |= 0x40;  /* Second bit as 1 0x40=01000000 */
      random_bits[KEY_LENGTH-1] |= 0x01;  /* Last bit as 1 */

      /* convert the random bytes to a bignum */
      BN_bin2bn(random_bits, KEY_LENGTH, q);

      /* generate p = 2 * q + 1*/
      BN_zero(p);
      BN_lshift1(p, q);
      BN_copy(pm1, p);  /* p-1 */
      BN_add_word(p, 1);  /* p */
      BN_copy(pp1, p);  /* p+1 */
      BN_add_word(pp1, 1);
      BN_sub_word(q, 1); /* q-1 */

      /* check that both p-1, p+1, q-1 don't have small factors
       * Note: exclude 2 from primes
       */
      not_prime = 0;
      for (i=1; i<N_PRIMES; i++) {
        if ((BN_mod_word(pm1, (BN_ULONG)primes[i]) == 0) ||
            //(BN_mod_word(pp1, (BN_ULONG)primes[i]) == 0) ||
            (BN_mod_word(q, (BN_ULONG)primes[i]) == 0)) {
          not_prime = 1;
          break;
        }
      }
      if (not_prime) {
        continue;
      }

      /* now pm1 is (p-1)/2 */
      BN_rshift1(pm1, p);

      /*BN_print_fp(stdout, p);
      printf("\n");*/

      /* check that (p-1)/2 is prime (it's ok to do just p/2, integer division)
       * and that p is prime
       */
      if (BN_is_prime(pm1, checks, NULL, NULL, NULL) &&
          BN_is_prime(p, checks, NULL, NULL, NULL)) {
        done = 1;
        found = 1;
      }
    }
  }

  BN_free(q);
  BN_free(pm1);
  BN_free(pp1);
}
