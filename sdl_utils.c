#include "sdl_utils.h"
#include <SDL2/SDL_render.h>

extern bool sdl_start(sdl_t *sdl) {
  // Initialize the subsystems
  // TIMER, AUDIO, VIDEO, EVENTS
  if (!(SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO |
                 SDL_INIT_EVENTS) == 0)) {
    SDL_Log("SDL failed to initialize subsystems. SDL_ERROR: %s",
            SDL_GetError());
    return false;
  }

  // Initialize the window and error handle

  sdl->window = SDL_CreateWindow(
      "Chip 8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      WINDOW_WIDTH * SCALE_FACTOR, WINDOW_HEIGHT * SCALE_FACTOR, 0);

  if (!sdl->window) {
    SDL_Log("SDL failed to initialize the window. SDL_ERROR: %s",
            SDL_GetError());
    return false;
  }

  // Initialize the renderer and error handle

  sdl->renderer = SDL_CreateRenderer(sdl->window, -1, 0);

  if (!sdl->renderer) {
    SDL_Log("SDL failed to initialize the renderer. SDL_ERROR %s",
            SDL_GetError());
    return false;
  }

  return true;
}
