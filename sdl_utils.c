#include "sdl_utils.h"

// builds the subsystems, window and renderer for the
// program to use
// INPUT: main sdl struct
// OUT: boolean, true if it worked false if it failed
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

  sdl->window =
      SDL_CreateWindow("Chip 8 Emulator", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH * sdl->scale_factor,
                       WINDOW_HEIGHT * sdl->scale_factor, SDL_WINDOW_VULKAN);

  if (!sdl->window) {
    SDL_Log("SDL failed to initialize the window. SDL_ERROR: %s",
            SDL_GetError());
    return false;
  }

  // Initialize the renderer and error handle

  sdl->renderer = SDL_CreateRenderer(sdl->window, -1, SDL_RENDERER_ACCELERATED);

  if (!sdl->renderer) {
    SDL_Log("SDL failed to initialize the renderer. SDL_ERROR %s",
            SDL_GetError());
    return false;
  }

  return true;
}
// Clears the screen
// IN: the main sdl object
// OUT: boolean if it functioned

bool clear_screen(sdl_e *sdl) {
  uint8_t bg_color_r = (sdl->bg_color >> 24) & 0xFF;
  uint8_t bg_color_g = (sdl->bg_color >> 16) & 0xFF;
  uint8_t bg_color_b = (sdl->bg_color >> 8) & 0xFF;
  uint8_t bg_color_a = (sdl->bg_color >> 0) & 0xFF;

  SDL_RenderClear(sdl->renderer);
  SDL_SetRenderDrawColor(sdl->renderer, bg_color_r, bg_color_g, bg_color_b,
                         bg_color_a);

  return true;
}

void update_screen(sdl_e *sdl, bool display[2048]) {
  SDL_RenderClear(sdl->renderer);
  uint8_t bg_color_r = (sdl->bg_color >> 24) & 0xFF;
  uint8_t bg_color_g = (sdl->bg_color >> 16) & 0xFF;
  uint8_t bg_color_b = (sdl->bg_color >> 8) & 0xFF;
  uint8_t bg_color_a = (sdl->bg_color >> 0) & 0xFF;

  uint8_t fg_color_r = (sdl->fg_color >> 24) & 0xFF;
  uint8_t fg_color_g = (sdl->fg_color >> 16) & 0xFF;
  uint8_t fg_color_b = (sdl->fg_color >> 8) & 0xFF;
  uint8_t fg_color_a = (sdl->fg_color >> 0) & 0xFF;
  SDL_Rect rect = {
      .w = sdl->scale_factor, .h = sdl->scale_factor, .x = 0, .y = 0};

  for (int i = 0; i < 2048; i++) {
    rect.x = (i % WINDOW_WIDTH) * sdl->scale_factor;
    rect.y = (i / WINDOW_WIDTH) * sdl->scale_factor;
    if (display[i]) {
      SDL_SetRenderDrawColor(sdl->renderer, fg_color_r, fg_color_g, fg_color_b,
                             fg_color_a);
    } else {
      SDL_SetRenderDrawColor(sdl->renderer, bg_color_r, bg_color_g, bg_color_b,
                             bg_color_a);
    }
    SDL_RenderFillRect(sdl->renderer, &rect);
  }
  SDL_RenderPresent(sdl->renderer);
}

// manage all key input through sdl2
// IN:
// OUT: boolean if true then the program
// continues, if false, then the program stops
bool key_input(state_e *state, bool *keypad) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case (SDL_QUIT):
      return false;
    case (SDL_KEYDOWN):
      switch (event.key.keysym.sym) {
      case (SDLK_SPACE):
        if (*state == 0) {
          *state = 1;
        } else {
          *state = 0;
        }
        break;
      case (SDLK_1):
        keypad[0x1] = true;
        break;
      case (SDLK_2):
        keypad[0x2] = true;
        break;
      case (SDLK_3):
        keypad[0x3] = true;
        break;
      case (SDLK_4):
        keypad[0xC] = true;
        break;
      case (SDLK_q):
        keypad[0x4] = true;
        break;
      case (SDLK_w):
        keypad[0x5] = true;
        break;
      case (SDLK_e):
        keypad[0x6] = true;
        break;
      case (SDLK_r):
        keypad[0xD] = true;
        break;
      case (SDLK_a):
        keypad[0x7] = true;
        break;
      case (SDLK_s):
        keypad[0x8] = true;
        break;
      case (SDLK_d):
        keypad[0x9] = true;
        break;
      case (SDLK_f):
        keypad[0xE] = true;
        break;
      case (SDLK_z):
        keypad[0xA] = true;
        break;
      case (SDLK_x):
        keypad[0x0] = true;
        break;
      case (SDLK_c):
        keypad[0xB] = true;
        break;
      case (SDLK_v):
        keypad[0xF] = true;
        break;
      }
      break;
    case (SDL_KEYUP):
      switch (event.key.keysym.sym) {
      case (SDLK_1):
        keypad[0x1] = false;
        break;
      case (SDLK_2):
        keypad[0x2] = false;
        break;
      case (SDLK_3):
        keypad[0x3] = false;
        break;
      case (SDLK_4):
        keypad[0xC] = false;
        break;
      case (SDLK_q):
        keypad[0x4] = false;
        break;
      case (SDLK_w):
        keypad[0x5] = false;
        break;
      case (SDLK_e):
        keypad[0x6] = false;
        break;
      case (SDLK_r):
        keypad[0xD] = false;
        break;
      case (SDLK_a):
        keypad[0x7] = false;
        break;
      case (SDLK_s):
        keypad[0x8] = false;
        break;
      case (SDLK_d):
        keypad[0x9] = false;
        break;
      case (SDLK_f):
        keypad[0xE] = false;
        break;
      case (SDLK_z):
        keypad[0xA] = false;
        break;
      case (SDLK_x):
        keypad[0x0] = false;
        break;
      case (SDLK_c):
        keypad[0xB] = false;
        break;
      case (SDLK_v):
        keypad[0xF] = false;
        break;
      }
    }
  }
  return true;
}
void sdl_cleanup(sdl_e *sdl) {
  SDL_DestroyRenderer(sdl->renderer);
  SDL_DestroyWindow(sdl->window);
  SDL_Quit();
}
