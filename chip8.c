#include "chip8.h"
#include <SDL2/SDL_timer.h>

void chip8_instructions(chip8_e *chip8) {
  uint16_t instruction =
      (chip8->ram[chip8->PC] << 8 | chip8->ram[chip8->PC + 1]);
  uint8_t opcode = (instruction >> 12) & 0x000F;
  // printf("instruction: 0x%4X, opcocde: 0x%1X\n", instruction, opcode);
  chip8->PC += 2;
  chip8->NNN = instruction & 0x0FFF;
  chip8->NN = instruction & 0x00FF;
  chip8->N = instruction & 0x000F;
  chip8->X = (instruction >> 8) & 0x000F;
  chip8->Y = (instruction >> 4) & 0x000F;
  switch (opcode) {
  case 0x00:
    if (chip8->NN == 0xE0) {
      memset(&chip8->display, 0, sizeof chip8->display);
    } else if (chip8->NN == 0xEE) {
      chip8->PC = *--chip8->stack_ptr;
    } else {
      printf("ERROR: MALFORMED OPCODE\n");
    }
    break;
  case 0x1:
    chip8->PC = chip8->NNN;
    break;
  case 0x2:
    *chip8->stack_ptr++ = chip8->PC;
    chip8->PC = chip8->NNN;
    break;
  case 0x3:
    if (chip8->V[chip8->X] == chip8->NN) {
      chip8->PC += 2;
    }
    break;
  case 0x4:
    if (chip8->V[chip8->X] != chip8->NN) {
      chip8->PC += 2;
    }
    break;
  case 0x5:
    if (chip8->V[chip8->X] == chip8->V[chip8->Y]) {
      chip8->PC += 2;
    }
    break;
  case 0x6:
    chip8->V[chip8->X] = chip8->NN;
    break;
  case 0x7:
    chip8->V[chip8->X] += chip8->NN;
    break;
  case 0x8:
    if (chip8->N == 0x0) {
      chip8->V[chip8->X] = chip8->V[chip8->Y];
    } else if (chip8->N == 0x1) {
      chip8->V[chip8->X] |= chip8->V[chip8->Y];
    } else if (chip8->N == 0x2) {
      chip8->V[chip8->X] &= chip8->V[chip8->Y];
    } else if (chip8->N == 0x3) {
      chip8->V[chip8->X] ^= chip8->V[chip8->Y];
    } else if (chip8->N == 0x4) {
      if ((int)chip8->V[chip8->X] + (int)chip8->V[chip8->Y] >= 255) {
        chip8->V[0xF] = 1;
      } else {
        chip8->V[0xF] = 0;
      }
      chip8->V[chip8->X] += chip8->V[chip8->Y];
    } else if (chip8->N == 0x5) {
      if (chip8->V[chip8->X] >= chip8->V[chip8->Y]) {
        chip8->V[0xF] = 1;
      } else {
        chip8->V[0xF] = 0;
      }
      chip8->V[chip8->X] = chip8->V[chip8->X] - chip8->V[chip8->Y];
    } else if (chip8->N == 0x6) {
      chip8->V[0xF] = chip8->V[chip8->X] & 0x1;
      chip8->V[chip8->X] >>= 1;
    } else if (chip8->N == 0x7) {
      if (chip8->V[chip8->Y] >= chip8->V[chip8->X]) {
        chip8->V[0xF] = 1;
      } else {
        chip8->V[0xF] = 0;
      }
      chip8->V[chip8->X] = chip8->V[chip8->Y] - chip8->V[chip8->X];
    } else if (chip8->N == 0xE) {
      chip8->V[0xF] = (chip8->V[chip8->X] >> 7) & 0x1;
      chip8->V[chip8->X] <<= 1;
    } else {
      printf("ERROR: MALFORMED INSTRUCTION\n");
    }
    break;
  case 0x9:
    if (chip8->V[chip8->X] != chip8->V[chip8->Y]) {
      chip8->PC += 2;
    }
    break;
  case 0xA:
    chip8->I = chip8->NNN;
    break;
  case 0xB:
    chip8->PC = chip8->V[0] + chip8->NNN;
    break;
  case 0xC:
    srand(time(NULL));
    chip8->V[chip8->X] = rand() % 256 & chip8->NN;
    break;
    case 0xD: {
    uint8_t x_coord = chip8->V[chip8->X] % 64;
    uint8_t y_coord = chip8->V[chip8->Y] % 32;
    uint8_t safe_x = x_coord;
    chip8->V[0xF] = 0;
    for (uint8_t i = 0; i < chip8->N; i++) {
      uint8_t sprite_data = chip8->ram[chip8->I + i];
      x_coord = safe_x;
      for (int j = 7; j >= 0; j--) {
        bool *pixel = &chip8->display[y_coord * 64 + x_coord];
        bool sprite_bit = (sprite_data & (1 << j));
        if (*pixel && sprite_bit) {
          chip8->V[0xF] = 1;
        }
        *pixel ^= sprite_bit;
        if (++x_coord >= 64) {
          break;
        }
      }
      if (++y_coord >= 32) {
        break;
      }
    }
    break;
  }
  case 0xE:
    if (chip8->N == 0xE) {
      if (chip8->keypad[chip8->V[chip8->X]]) {
        chip8->PC += 2;
      }
    } else if (chip8->N == 0x1) {
      if (!chip8->keypad[chip8->V[chip8->X]]) {
        chip8->PC += 2;
      }
    } else {
      printf("ERROR: MALFORMED INSTRUCTION\n");
    }
    break;
  case 0xF:
    if (chip8->NN == 0x07) {
      chip8->V[chip8->X] = chip8->delay;
    } else if (chip8->NN == 0x0A) {
      bool keypressed = false;
      for (uint8_t i = 0; i < sizeof chip8->keypad; i++) {
        if (chip8->keypad[i]) {
          chip8->V[chip8->X] = i;
          keypressed = true;
          break;
        }
      }
      if (!keypressed) {
        chip8->PC -= 2;
      }
    } else if (chip8->NN == 0x15) {
      chip8->delay = chip8->V[chip8->X];
    } else if (chip8->NN == 0x18) {
      chip8->sound = chip8->V[chip8->X];
    } else if (chip8->NN == 0x1E) {
      chip8->I += chip8->V[chip8->X];
    } else if (chip8->NN == 0x29) {
      chip8->I = chip8->V[chip8->X] * 5;
    } else if (chip8->NN == 0x33) {
      uint8_t num = chip8->V[chip8->X];
      uint8_t num_100 = (num % 1000) / 100;
      uint8_t num_10 = (num % 100) / 10;
      uint8_t num_1 = num % 10;
      chip8->ram[chip8->I] = num_100;
      chip8->ram[chip8->I + 1] = num_10;
      chip8->ram[chip8->I + 2] = num_1;
    } else if (chip8->NN == 0x55) {
      for (int i = 0; i <= chip8->X; i++) {
        chip8->ram[chip8->I + i] = chip8->V[i];
      }
    } else if (chip8->NN == 0x65) {
      for (int i = 0; i <= chip8->X; i++) {
        chip8->V[i] = chip8->ram[chip8->I + i];
      }
    } else {
      printf("ERROR: MALFORMED INSTRUCTION\n");
    }
    break;
  default:
    SDL_Log("Unimplemented opcode");
    break;
  }
}
void update_timers(chip8_e *chip8) {
  if (chip8->sound > 0) {
    chip8->sound--;
  }
  if (chip8->delay > 0) {
    chip8->delay--;
  }
}

bool chip8_loop(chip8_e *chip8, sdl_e *sdl) {
  while (chip8->state != QUIT) {
    if (!key_input(&chip8->state, chip8->keypad)) {
      sdl_cleanup(sdl);
      chip8->state = QUIT;
    }
    if (chip8->state == PAUSED) {
      continue;
    }
    const double start_time = SDL_GetPerformanceCounter();
    for (int i = 0; i < (700 / 60); i++) {
      chip8_instructions(chip8);

      // printf("delay: %d\n", chip8->delay);
    }
    const double end_time = SDL_GetPerformanceCounter();
    const double total_time = (double)((end_time - start_time) * 1000) /
                              SDL_GetPerformanceFrequency();
    if (total_time < 16.67f) {
      SDL_Delay(16.67f - total_time);
    }
    update_screen(sdl, chip8->display);
    update_timers(chip8);
  }

  return true;
}

bool chip8_init(chip8_e *chip8, char *name) {
  uint32_t user_space_start = 0x200;
  uint8_t font[] = {
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
  memcpy(&chip8->ram[0], font, sizeof font);

  FILE *rom = fopen(name, "rb");
  if (!rom) {
    SDL_Log("Rom %s does not exist", name);
    return false;
  }
  fseek(rom, 0, SEEK_END);

  size_t rom_size = ftell(rom);
  size_t max_size = sizeof chip8->ram - user_space_start;
  rewind(rom);

  if (rom_size > max_size) {
    SDL_Log("Rom size exceeds maximum allowed size. rom_size: %zu, max_size: "
            "%zu\n",
            rom_size, max_size);
    return false;
  }
  if (fread(&chip8->ram[user_space_start], rom_size, 1, rom) != 1) {
    SDL_Log("Unable to properly read rom, are you sure its a .ch8 file");
    return false;
  }

  fclose(rom);

  chip8->state = RUNNING;
  chip8->PC = user_space_start;
  chip8->stack_ptr = &chip8->stack[0];
  memset(chip8->display, 0, 2048);

#ifdef DEBUG
  SDL_Log("chip8 state: %d\n", chip8->state);
  SDL_Log("chip8 PC: 0x%x\n", chip8->PC);
#endif
  return true;
}
