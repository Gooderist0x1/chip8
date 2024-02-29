#include "chip8.h"
bool chip8_loop(chip8_e *chip8, sdl_e *sdl) {
  while (chip8->state != QUIT) {
    if (!key_input()) {
      sdl_cleanup(sdl);
      chip8->state = QUIT;
    }
    if (chip8->state == PAUSED) {
      continue;
    }
  }
  return true;
}

void chip8_init(chip8_e *chip8) {

}
