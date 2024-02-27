#ifndef SDL_UTILS_H
#define SDL_UTILS_H

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;

}
sdl_t;


extern bool sdl_start(sdl_t *sdl);

#endif
