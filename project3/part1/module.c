#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <emscripten/emscripten.h>

int main(int argc, char ** argv) {
  printf("Hello World\n");
}

// bien penser a compiler avec NO_EXIT_RUNTIME
int EMSCRIPTEN_KEEPALIVE add(int p1, int p2) {
  printf("Add %d and %d\n", p1, p2);// ne pas oublier le saut de ligne
  return p1 + p2;
}

float EMSCRIPTEN_KEEPALIVE addFloat(float p1, float p2) {
  printf("Add %0.1f and %0.1f\n", p1, p2);// ne pas oublier le saut de ligne
  return p1 + p2;
}