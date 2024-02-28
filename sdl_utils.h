#ifndef SDL_UTILS_H
#define SDL_UTILS_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>

#define WINDOW_HEIGHT 32
#define WINDOW_WIDTH 64
#define SCALE_FACTOR 20

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
  uint8_t scale_factor;
} sdl_t;

extern bool sdl_start(sdl_t *sdl);

#endif
