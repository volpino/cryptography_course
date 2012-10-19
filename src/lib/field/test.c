#include <assert.h>
#include <stdio.h>
#include "../field.h"

#define g6poly 0x1b

int main() {
  /*
    uint8_t a, b;
    printf("Gimme the A vector: ");
    a = g6_input();
    scanf("\n");
    printf("Gimme the B vector: ");
    scanf("\n");
    b = g6_input();

    printf("\nA + B: ");
    g6_print(gf_add(a, b));
    printf("\nA * B: ");
    g6_print(gf_mul(a, b, 6, 0x1b));
    printf("\n");
  */

  assert(gf_mul(0x00, 0x00, 6, g6poly) == 0x00);
  assert(gf_mul(0x01, 0x01, 6, g6poly) == 0x01);
  assert(gf_mul(0x1b, 0x01, 6, g6poly) == 0x1b);
  assert(gf_mul(0x1b, 0x0b, 6, g6poly) == 0x18);

  assert(gf_rotate(0x01, -1, 6) == 0x02);
  assert(gf_rotate(0x02,  1, 6) == 0x01);
  assert(gf_rotate(0x10, -1, 6) == 0x20);
  assert(gf_rotate(0x01,  1, 6) == 0x20);
  assert(gf_rotate(0x01,  6, 6) == 0x01);
  assert(gf_rotate(0x3f,  2, 6) == 0x3f);

  assert(gf_rotate(0x4,  2, 4) == 0x1);
  assert(gf_rotate(0x8,  3, 4) == 0x1);

  printf("OK\n");

  return 0;
}

#undef g6poly
