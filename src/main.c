#include "chip8.h"


int main(int argc, char *argv[]) {
  // initialize config from the cli arguments
  config_t config = {0};
  if (!set_config(&config, argc, argv)) return EXIT_FAILURE;

  // initialize the SDL subsystems and create graphics components
  sdlc_t sdlc = {0};
  if (!sdl_init(&sdlc)) return EXIT_FAILURE;

  // initialize the emulator machine
  emulator_t emulator = {0};
  if (!emulator_init(&emulator, &config)) return EXIT_FAILURE;

  // emulator loop
  while (emulator.state != STOP) {
    // handle user inputs
    
    // entering debug mode
    if (emulator.state == PAUSED) continue;

    // emulate chip8 instructions

    // delay of 60FPS (value in milliseconds)
    SDL_Delay(16);

    // update the screen with back buffer changes
    update_screen(&sdlc);
  }

  sdl_free(&sdlc);     // free up the SDL components    

  return EXIT_SUCCESS;
}