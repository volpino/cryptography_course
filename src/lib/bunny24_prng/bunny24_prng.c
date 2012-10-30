#include "../bunny24.h"

void bunny24_prng(const uint8_t* seed, int n, uint8_t* out, int m) {
    int i;
    uint8_t iv[3] = {0x00, 0x00, 0x00};
    uint8_t k[3] = {0x00, 0x00, 0x00};

    for (i=0; i<m; i++) {
        if (i < n) {
            out[i] = seed[i];
        }
        else {
            out[i] = 0;
        }
    }
    bunny24_encrypt_cbc(out, m, k, iv);
}
