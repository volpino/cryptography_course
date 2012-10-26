#include "bunny_internals.h"
#include <stdio.h>
#include <stdlib.h>
/*
[e^46 e^56 e^53 e^31]
[e^35 e^48 e^38 e^29]
[e^20 e^18 e^11 e^58]
[e^50 e^47 e^25 e^12]
*/


int tab [4][4] = {{46,56,53,31},
                  {35,48,38,29},
                  {20,18,11,58},
                  {50,47,25,12}};

uint8_t e = 0x02;

uint8_t pow(uint8_t a, int b){
  uint8_t result = 1, j;
  for (j=0; j<b; j++) {
    result = bunny_mul(result, a);
  }
  return result;
}

int main() {
  int i, j;

  printf("{");
  for (i=0; i<4; i++){
    printf("{");
    for (j=0; j<3; j++){
      printf("0x%x, ", pow(e, tab[i][j]));
    }
    printf("0x%x}\n", pow(e, tab[i][j]));
  }
  printf("}\n");
  return 0;
}
