#ifndef SDL_UTILS_H
#define SDL_UTILS_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>

#define WINDOW_HEIGHT 32
#define WINDOW_WIDTH 64

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
  uint32_t fg_color;
  uint32_t bg_color;
  uint8_t scale_factor;
} sdl_e;

typedef enum { RUNNING, PAUSED, QUIT } state_e;

bool sdl_start(sdl_e *sdl);
bool clear_screen(sdl_e *sdl);
bool key_input(state_e *state, bool *keypad);
void update_screen(sdl_e *sdl, bool display[64 * 32]);
void sdl_cleanup(sdl_e *sdl);

#endif
