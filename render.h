#ifndef RENDER_H
#define RENDER_H

#include <stdint.h>
#include <stdbool.h>
#include "chip8.h"

bool render_init(const char* title);

void render_update(const uint8_t* display_buffer);

void render_cleanup(void);

bool process_input_events(chip8_cpu_t *cpu);

#endif
