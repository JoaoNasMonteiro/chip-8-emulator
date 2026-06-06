#include "chip8.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG

const uint8_t temp_rom[] = {
    0x65, 0x01, // LD V5, 0x01
    0x75, 0x01, // ADD V5, 0x01
    0x75, 0x05, // ADD v%, 0x05
    0x00, 0x00  // BRK
};

int main(void) {
  chip8_cpu_t *cpu = (chip8_cpu_t *)malloc(sizeof(chip8_cpu_t));
  init_cpu(cpu);

  memcpy(cpu->memory + 0x200, temp_rom, sizeof(temp_rom));

  while (1) {
    cpu_step(cpu);
  }

  return EXIT_SUCCESS;
}
