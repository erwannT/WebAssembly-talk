#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <emscripten/emscripten.h>

int main(int argc, char ** argv) {
  printf("Hello World\n");
}


const char *   EMSCRIPTEN_KEEPALIVE info(const char * myLog) {
  printf("%s\n", myLog);
  return "Bon retour";
} 