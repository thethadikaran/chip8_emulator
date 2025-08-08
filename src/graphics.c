#include "chip8.h"


bool sdl_init(sdlc_t *sdlc) {
  // I. initialize SDL subsystems along with audio and video
  if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO) != 0) {
    SDL_Log("SDL subsystem initialization failure - %s\n", SDL_GetError());
    return false;
  }

  // II. initialize SDL window
  sdlc->win = SDL_CreateWindow(
                "CHIP8 Emulator",                  // window title
                SDL_WINDOWPOS_CENTERED,            // x positon of window
                SDL_WINDOWPOS_CENTERED,            // y position of window
                300,  // width of window
                300, // height of window
                0                                  // flags
              );
  if (!sdlc->win) {
    SDL_Log("Window creation failure - %s\n", SDL_GetError());
    return false;
  }

  // III. initialize a renderer
  sdlc->render = SDL_CreateRenderer(
                    sdlc->win,                 // window to display rendering
                    -1,                        // index of rendering driver (-1 to initilize with first one)
                    SDL_RENDERER_ACCELERATED   // flag
                );
  if (!sdlc->render) {
    SDL_Log("Renderer initialization failure - %s\n", SDL_GetError());
    return false;
  }

  return true;    // initialization was successful
}



void update_screen(const sdlc_t *sdlc) {
  // after composing the entire scene/drawing in the backbuffer then
  // call this function once per frame to present the backbuffer to the screen
  SDL_RenderPresent(sdlc->render);
}



void sdl_free(sdlc_t *sdlc) {
  SDL_DestroyRenderer(sdlc->render); // destroy the renderer
  SDL_DestroyWindow(sdlc->win);      // destroy the SDL Window
  SDL_Quit();                        // shutdown all SDL subsystems
}