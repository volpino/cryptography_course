#include "bunny_internals.h"

void key_schedule(array key, uint8_t* result) {
    uint8_t w[20];
    w[0] = key[0];
    w[1] = key[1];
    w[2] = key[2];
    w[3] = key[3];

    /*Wi = SB(Wi-4) + Wi-3 */
    w[4] = bunny_add(SB1[w[0]], w[1]);
    w[5] = bunny_add(SB1[w[1]], w[2]);
    w[6] = bunny_add(SB1[w[2]], w[3]);
    w[7] = bunny_add(SB1[w[3]], w[0]);

    w[8] = bunny_add(bunny_add(w[0], SB2[bunny_rotate(w[7], -1)]), 40);
    w[9] = bunny_add(w[1], w[8]);
    w[10] = bunny_add(w[2], w[9]);
    w[11] = bunny_add(w[3], w[10]);
    w[12] = bunny_add(w[4], SB3[w[11]]);
    w[13] = bunny_add(w[5], w[12]);
    w[14] = bunny_add(w[6], w[13]);
    w[15] = bunny_add(w[7], w[14]);
    w[16] = bunny_add(bunny_add(w[8], SB2[bunny_rotate(w[15], -1)]), 40);
    w[17] = bunny_add(w[9], w[16]);
    w[18] = bunny_add(w[10], w[17]);
    w[19] = bunny_add(w[11], w[18]);
}
