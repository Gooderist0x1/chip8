#include "sdl_utils.h"
#include <SDL2/SDL_events.h>

//builds the subsystems, window and renderer for the 
//program to use
//INPUT: main sdl struct
//OUT: boolean, true if it worked false if it failed
bool sdl_start(sdl_e *sdl) {
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
      WINDOW_WIDTH * sdl->scale_factor, WINDOW_HEIGHT * sdl->scale_factor, 0);

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
//Clears the screen
//IN: the main sdl object
//OUT: boolean if it functioned

bool clear_screen(sdl_e *sdl) {
  uint8_t bg_color_r = (sdl->bg_color << 24) & 0xFF;
  uint8_t bg_color_g = (sdl->bg_color << 16) & 0xFF;
  uint8_t bg_color_b = (sdl->bg_color << 8) & 0xFF;
  uint8_t bg_color_a = (sdl->bg_color << 0) & 0xFF;

  SDL_RenderClear(sdl->renderer);
  SDL_SetRenderDrawColor(sdl->renderer, bg_color_r, bg_color_g, bg_color_b,
                         bg_color_a);

  return true;
}


//manage all key input through sdl2
//IN:
//OUT: boolean if true then the program
//continues, if false, then the program stops
bool key_input(void) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case (SDL_QUIT):
      return false;
    }
  }
  return true;
}

void sdl_cleanup(sdl_e *sdl) {
  SDL_DestroyRenderer(sdl->renderer);
  SDL_DestroyWindow(sdl->window);
  SDL_Quit();
}
