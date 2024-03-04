#include "chip8.h"
#include "sdl_utils.h"

typedef struct {
  uint8_t scale_factor;
  uint32_t fg_color;
  uint32_t bg_Color;
} config_e;

void create_config(config_e *config) { config->scale_factor = 20; }

void apply_config(config_e *config, sdl_e *sdl) {
  // set the default config
  config->scale_factor = 20;
  config->fg_color = 0xFFFFFFFF;
  config->bg_Color = 0x00000000;

  // set the custom config
  // from argv later
  // when the config is complete

  // set the config
  sdl->scale_factor = config->scale_factor;
  sdl->fg_color = config->fg_color;
  sdl->bg_color = config->bg_Color;
}

int main(int argc, char *argv[]) {
  if (!(argc > 1)) {
    printf("Usage: %s <ROM name>\n", argv[0]);
    return 1;
  }

  // create variables
  sdl_e sdl = {0};
  config_e config = {0};
  chip8_e chip8 = {0};

  // use created variables to

  create_config(&config);
  apply_config(&config, &sdl);
  sdl_start(&sdl);
  if (!chip8_init(&chip8, argv[1])) {
    sdl_cleanup(&sdl);
    return 1;
  }
  chip8_loop(&chip8, &sdl);
  return 0;
}
