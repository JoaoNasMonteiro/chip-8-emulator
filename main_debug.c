#include "chip8.h"
#include <stdint.h>

#define DEBUG

// enum {
//     ADDR_PROG_START   = 0x0200,
//     ADDR_FONT_START   = 0x0050
// };
//
// typedef struct {
//     uint8_t memory[4096];
//     uint8_t registers[16];
//     uint16_t I;
//     uint8_t delay_timer;
//     uint8_t sound_timer;
//     uint16_t pc;
//     uint8_t sp;
//     uint16_t stack[16];
// } chip8_cpu_t;

void *init_cpu(chip8_cpu_t *cpu) {
  // write zero to everything but the memory
}

void cpu_step(chip8_cpu_t *cpu) {
  // fetch
  // decode & axecute
}

uint16_t get_keyboad_input()

    int load_rom()

        void cpu_update_timers()

            uint8_t
    read_memory(chip8_cpu_t *cpu,
                uint16_t memaddr) int write_memory(chip8_cpu_t *cpu,
                                                   uint16_t memaddr,
                                                   uint8_t value)
