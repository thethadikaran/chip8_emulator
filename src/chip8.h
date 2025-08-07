#ifndef __CHIP8_EMULATOR_HEADER__
#define __CHIP8_EMULATOR_HEADER__

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_video.h"


/* emulator parameters */
#define DATA_REGISTER   16        // 16 x 8 bit register -> V0 to VF
#define RAM_SIZE        4096      // 4096 bytes of memory
#define STACK_SIZE      16        // function stack, support 16 level of nesting
#define KEY_COUNT       16        // keys 0 to F

#define WIDTH           64        // 64 x 32 - native CHIP8 emulator resolution
#define HEIGHT          32        // which totals to 2048 pixels


/* enum to represent the emulator state */
typedef enum { STOP, RUNNING, PAUSED } emulator_state_t;


/* struct to describe the emulator configurations */
typedef struct {
  uint8_t V[DATA_REGISTER];     // data register - V0 to VF
  uint16_t I;                   // index/address register
  uint16_t PC;                  // program counter

  uint8_t ram[RAM_SIZE];        // RAM 

  uint16_t stack[STACK_SIZE];   // function stack

  uint8_t delay_timer;          // count at 60Hz, until reaches 0
  uint8_t sound_timer;          // makes beep sound, if value is non-zero

  bool keypad[KEY_COUNT];       // hexadecimal keypad

  bool display[WIDTH * HEIGHT];// each bit represent state of single pixel
                               // array act as a display buffer for storing graphics

  char *rom;                   // ROM file - instructions
  emulator_state_t state;      // state of the emulator
} emulator_t;



/* struct to hold the SDL components */
typedef struct {
  SDL_Window *win;        // main window
  SDL_Renderer *render;   // 2D renderer
} sdlc_t;




/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/* ---------- FUNCTION PROTOTYPES ---------- */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/* ----- CHIP8 ------------------------------*/

/* ----- GRAPHICS ---------------------------*/
bool sdl_init(sdlc_t *);

/* ----- UTILS ------------------------------*/




#endif   // __CHIP8_EMULATOR_HEADER__