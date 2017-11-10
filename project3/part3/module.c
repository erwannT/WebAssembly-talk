#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <emscripten/emscripten.h>

int main(int argc, char ** argv) {
  printf("Hello World\n");
}


int EMSCRIPTEN_KEEPALIVE logArrayOfInt(uint8_t *intTab, int size) {
  int i = 0;
  while( i < size){
    printf("Element %d : %u\n", i, intTab[i]);
    i++;
  }
  return 1;
}

int EMSCRIPTEN_KEEPALIVE logArrayOfInt32(int32_t *intTab, int size) {
  int i = 0;
  while( i < size){
    printf("Element %d : %d\n", i, intTab[i]);
    i++;
  }
  return 1;  
}