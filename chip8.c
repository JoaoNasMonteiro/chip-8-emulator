#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chip8.h"

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

const uint8_t fontset[80] = {
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

int init_cpu(chip8_cpu_t *cpu) {
  if (cpu == NULL) {
    perror("init_cpu failed:");
    errno = ENOMEM;
    return EXIT_FAILURE;
  }
  // initialize everything to know state (all zeroes)
  memset(cpu, 0, sizeof(chip8_cpu_t));

  // init fontset
  memcpy(cpu->memory + 0x050, fontset, sizeof(fontset));

  // init pc
  cpu->pc = 0x0200;

  return 0;
}

void cpu_step(chip8_cpu_t *cpu) {
  if (cpu == NULL) {
    perror("cpu_step failed:");
    errno = ENOMEM;
    return;
  }
  // fetch (get the opcode, meaning the word coposed of (byte at pc) << 8 |
  // (byte at pc + 1))
  uint16_t opcode = (cpu->memory[cpu->pc] << 8) | cpu->memory[cpu->pc + 1];
  cpu->pc += 2;

  // decode & axecute

  // parse the opcode to extract the fields.
  uint8_t x = (opcode & 0x0f00) >> 8;
  uint8_t y = (opcode & 0x00f0) >> 6;
  uint8_t kk = opcode & 0x00ff;
  uint8_t n = opcode & 0x000f;
  uint16_t nnn = opcode & 0x0fff;

  char mnemonic[32];

  // a routing switch statement that calls
  // types are:
  // AAAA (hardcoded) - 0
  // fnnn - 1 (memory ops)
  // fxkk - 2 (one reg ops with args)
  // fxAA - 3 (one reg ops without args)
  // fxyA - 4 (two reg ops without args)
  // fxyn - 5 (two reg ops with args)

  switch (opcode & 0xf000) {
  case 0x6000: // 6xkk - LD Vx, Byte
    sprintf(mnemonic, "LD V%X, 0x%02X", x, kk);
    cpu->registers[x] = kk;
    break;
  case 0x7000: // 7xkk - ADD Vx, byte
    sprintf(mnemonic, "ADD V%X, 0x%02X", x, kk);
    cpu->registers[x] += kk;
    break;
  case 0x0000:
    memset(0, 1, 1);
    break;
  default:
    errno = EINVAL;
    sprintf(mnemonic, "Safe String");
    perror("Could not execute instruction - invalid operation");
    break;
  }

  printf("PC:%04X | OP:%04X | %s | V%X: %04X\n", cpu->pc - 2, opcode, mnemonic,
         x, cpu->registers[x]);
}

// uint16_t get_keyboad_input()
//
// int load_rom()
//
//         void cpu_update_timers()
//
//             uint8_t
//     read_memory(chip8_cpu_t *cpu,
//                 uint16_t memaddr) int write_memory(chip8_cpu_t *cpu,
//                                                    uint16_t memaddr,
//                                                   uint8_t value)
