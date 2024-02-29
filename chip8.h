#ifndef CHIP8_H
#define CHIP8_H

#include "sdl_utils.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdint.h>

typedef enum { RUNNING, PAUSED, QUIT } state_e;

typedef struct {
  state_e state;
  bool display[64 * 32];
  uint16_t ram[4096];
  uint16_t stack[16];
  uint16_t PC;
  uint16_t I;
  uint16_t NNN;
  uint8_t NN;
  uint8_t N;
  uint8_t X;
  uint8_t Y;
  uint8_t V[0xF];
  uint8_t delay;
  uint8_t timer;
  uint8_t *stack_ptr;
} chip8_e;
bool chip8_loop(chip8_e *chip8, sdl_e *sdl);
void chip8_init(chip8_e *chip8);
#endif
