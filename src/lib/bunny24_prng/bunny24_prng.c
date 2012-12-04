#include "../bunny24.h"
#include "../bunny24_prng.h"
#include <stdlib.h>


void bunny24_prng(const uint8_t* seed, int n, const uint8_t* iv,
                  uint8_t* out, int m) {
  int i;
  uint8_t iv_zero[3] = {0x00, 0x00, 0x00};
  uint8_t k[3] = {0x00, 0x00, 0x00};

  for (i=0; i<m; i++) {
    if (i < n) {
      out[i] = seed[i];
    }
    else {
      out[i] = 0;
    }
  }
  if (iv == NULL) {
    bunny24_encrypt_cbc(out, m, k, iv_zero);
  }
  else {
    bunny24_encrypt_cbc(out, m, k, iv);
  }
}
