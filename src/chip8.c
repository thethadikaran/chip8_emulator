#include "chip8.h"

/** 
 * Build-in font (characters from 0 to F)
 * each font character are 4 pixel wide x 5 pixel tall
 * 
 * consider character - 0
 * 0xF0  1 1 1 1 0 0 0 0 
 * 0x90  1 0 0 1 0 0 0 0
 * 0x90  1 0 0 1 0 0 0 0
 * 0x90  1 0 0 1 0 0 0 0
 * 0xF0  1 1 1 1 0 0 0 0
 * 
 * index register is set to specific character's memeory location and
 * it will be drawn to the screen. 
 */
const uint8_t fontset[] = {
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};



bool set_config(config_t *config, int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <rom_name>.ch8\n", argv[0]);
    return false;
  }

  config->rom_path = argv[1];
  return true;
}



bool emulator_init(emulator_t *emulator, config_t *config) {
  // clear RAM, stack, display, data register, keypad
  memset(emulator->V, 0, DATA_REGISTER * sizeof(uint8_t));
  memset(emulator->ram, 0, RAM_SIZE * sizeof(uint8_t));
  memset(emulator->stack, 0, STACK_SIZE * sizeof(uint16_t));
  memset(emulator->keypad, 0, KEY_COUNT);
  memset(emulator->display, 0, WIDTH * HEIGHT);

  // load the fontset into RAM (at starting location of RAM)
  memcpy(emulator->ram, fontset, sizeof(fontset));

  emulator->rom_path = config->rom_path;

  emulator->PC = 0x200;   // from 0x00 to 0x1FF (511) reserved by emulator
                          // reading of instruction starts from 0x200 (512)

  // load the ROM instructions into the RAM (starting from address 0x200)
  if (!load_rom(emulator)) return false;

  emulator->state = RUNNING;  // finally kick start the emulator

  return true;
}



void handle_inputs(emulator_t *emulator) {
  // SDL_PollEvent() pops the event from the event queue and load it into the
  // passed in SDL_Event struct. Returns 0 if no events in event queue else 1
  SDL_Event key_event;

  while (SDL_PollEvent(&key_event)) {
    // identify the type of the event :- like quit, keypress, mouse scroll
    // after knowing the type, use another switch to know the action
    switch (key_event.type) {
      case SDL_QUIT:        // quit requested event
        emulator->state = STOP;
        return;

      case SDL_KEYDOWN:     // keypad key is pressed
        handle_keydown_events(emulator, &key_event);
        break;

      case SDL_KEYUP:       // keypad key is released
        break;
    }

  }

}


/**
 * @brief Handle the key press in the keyboard
 * SDL_KeyboardEvent -> https://wiki.libsdl.org/SDL2/SDL_KeyboardEvent
 * SDL_Keysym -> https://wiki.libsdl.org/SDL2/SDL_Keysym
 * SDL_Keycode -> https://github.com/libsdl-org/SDL/blob/SDL2/include/SDL_keycode.h
 * 
 * @param emulator - emulator_t struct
 * @param key_event - SDL_Event struct, where key events are placed
 */
void handle_keydown_events(emulator_t *emulator, SDL_Event *key_event) {
  switch (key_event->key.keysym.sym) {
    case SDLK_ESCAPE:     // ESC key pressed
      emulator->state = STOP;
      return;

    case SDLK_SPACE:     // SPACE key pressed
      emulator->state = emulator->state == RUNNING ? PAUSED : RUNNING;

      if (emulator->state == PAUSED)
        puts("=== ENTERING DEBUG MODE ===");
      else 
        puts("=== EXIT DEBUG MODE ===");
      return;

  }

}



bool load_rom(emulator_t *emulator) {
  // open the ROM
  FILE *rom = fopen(emulator->rom_path, "rb");    // reading the binary ROM file
  if (!rom) {
    SDL_Log("ROM file \"%s\" is invalid or does not exist", emulator->rom_path);
    return false;
  }

  // calcuate the size of the loaded rom
  size_t rsize = rom_size(rom);

  // verify if the rom instructions can fit in RAM
  if (rsize > (sizeof(emulator->ram) - emulator->PC) ) {
    SDL_Log("Loaded ROM can't fit in to the RAM");
    fclose(rom);
    return false;
  }

  // load the ROM into RAM at the entry point
  bool status = fread(&emulator->ram[emulator->PC], rsize, 1, rom);
  if (!status)  {
    SDL_Log("Cannot load ROM on to the RAM");
    fclose(rom);
    return false;
  }

  // finally close the file
  fclose(rom);
  return true;
}



size_t rom_size(FILE *rom) {
  // move the file pointer to the end of the file
  fseek(rom, 0, SEEK_END);

  // tis position represent the offset in bytes from begining of the file
  size_t size = ftell(rom);

  // reset the function pointer to the beginning of the file
  rewind(rom);

  return size;
}