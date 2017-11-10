#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <emscripten/emscripten.h>

int main(int argc, char ** argv) {
  printf("Hello World\n");
}


int  EMSCRIPTEN_KEEPALIVE alert(){
  emscripten_run_script("alert('Alert world')");
  return  1;
}

int  EMSCRIPTEN_KEEPALIVE alert2(){
  EM_ASM_({
    alert($0)
  }, "test");
    return  1;
}