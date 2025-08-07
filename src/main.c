#include "chip8.h"


int main(int argc, char *argv[]) {
  // initialize the SDL subsystems and create graphics components
  sdlc_t sdlc = {0};
  if (!sdl_init(&sdlc)) return EXIT_FAILURE;

  return EXIT_SUCCESS;
}