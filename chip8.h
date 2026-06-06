#include <stdint.h>
#include <stdlib.h>

enum {
    MEM_SIZE          = 4096,
    ADDR_PROG_START   = 0x0200,
    ADDR_FONT_START   = 0x0050
};

typedef struct {
    uint8_t memory[4096];
    uint8_t registers[16];
    uint16_t I;
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint16_t pc;
    uint8_t sp;
    uint16_t stack[16];
    uint16_t keypad;
} chip8_cpu_t;

int init_cpu(chip8_cpu_t *cpu);
void cpu_step(chip8_cpu_t *cpu);
