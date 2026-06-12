#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>
#include <stdlib.h>

enum {
    MEM_SIZE          = 4096,
    DISPLAY_WIDTH     = 64,
    DISPLAY_HEIGHT    = 32,
    ADDR_PROG_START   = 0x0200,
    ADDR_FONT_START   = 0x0050,
    MAX_ROM_SIZE      = 0x0e00, // 3584
    MAX_DISPLAY_SIZE  = DISPLAY_HEIGHT * DISPLAY_WIDTH
};

typedef struct {
    uint8_t memory[MEM_SIZE];
    uint8_t display_buffer[MAX_DISPLAY_SIZE]; // display dimentions
    uint8_t registers[16];
    uint16_t I;
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint16_t pc;
    uint8_t sp;
    uint16_t stack[16];
    uint16_t keypad;
    uint8_t is_halted;
} chip8_cpu_t;

int init_cpu(chip8_cpu_t *cpu);
void cpu_step(chip8_cpu_t *cpu);
size_t load_rom(chip8_cpu_t *cpu, const uint8_t *rom_buffer, size_t rom_size);

#endif
