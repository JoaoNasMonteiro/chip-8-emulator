#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chip8.h"

#ifdef DEBUG_BUILD
#define DEBUG_PRINT(fmt, ...) fprintf(stderr, "DEBUG: " fmt, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...)
#endif

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
    uint8_t y = (opcode & 0x00f0) >> 4;
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

    case 0x0000: // 00 family
        switch (kk) {
        case 0x00: // 0000 - HLT
            sprintf(mnemonic, "HLT");
            while (1) {
                printf("");
            }
            break;
        case 0xe0: // 00E0 - CLS
            sprintf(mnemonic, "CLS");
            memset(cpu->display_buffer, 0, MAX_DISPLAY_SIZE);
            break;
        case 0xee: // 00EE - RET
            sprintf(mnemonic, "RET");
            cpu->sp--;
            cpu->pc = cpu->stack[cpu->sp];
            break;
        default:
            sprintf(mnemonic, "SYS 0x%03X (IGNORED)", nnn);
            break;
        }
        break;

    case 0x1000: // 1nnn - JP addr
        sprintf(mnemonic, "JP 0x%03X", nnn);
        cpu->pc = nnn;
        break;

    case 0x6000: // 6xkk - LD Vx, Byte
        sprintf(mnemonic, "LD V%X, 0x%02X", x, kk);
        cpu->registers[x] = kk;
        break;

    case 0x7000: // 7xkk - ADD Vx, byte
        sprintf(mnemonic, "ADD V%X, 0x%02X", x, kk);
        cpu->registers[x] += kk;
        break;

    case 0xA000: // Annn - LD I, addr
        sprintf(mnemonic, "LD I, 0x%03X", nnn);
        cpu->I = nnn;
        break;

    case 0xD000: // DRW Vy, Vy, nibble
        // fetch a sprite from memory and draw it starting from the positionat
        // vx and vy each sprite is one byte long (so 1100 0001 would be liek
        // draw two pixels, skip 5 then draw another) sprites are xored rather
        // than setting the memory to one if a sprite happens to destroy (set a
        // bit that was high to low) we must set Vf the coordinate at Vx and Vy
        // must be modulo wrapped, but the sprite stself can blled over the edge
        // of the display

        sprintf(mnemonic, "DRW V%X, V%X, %X", x, y, n);

        uint8_t start_x = cpu->registers[x] % DISPLAY_WIDTH;
        uint8_t start_y = cpu->registers[y] % DISPLAY_HEIGHT;

        cpu->registers[0xF] = 0;

        for (int row = 0; row < n; row++) {
            uint8_t sprite_byte = cpu->memory[cpu->I + row];

            for (int col = 0; col < 8; col++) {
                uint8_t sprite_pixel = (sprite_byte >> (7 - col)) & 1;

                if (sprite_pixel == 0) {
                    continue;
                }

                uint16_t current_x = start_x + col;
                uint16_t current_y = start_y + row;

                if (current_x >= DISPLAY_WIDTH || current_y >= DISPLAY_HEIGHT) {
                    continue;
                }

                uint16_t buff_index = (current_y * DISPLAY_WIDTH) + current_x;

                if (cpu->display_buffer[buff_index] == 1) {
                    cpu->registers[0xF] = 1;
                }

                cpu->display_buffer[buff_index] ^= 1;
            }
        }
        break;

    default:
        errno = EINVAL;
        sprintf(mnemonic, "Safe String");
        perror("Could not execute instruction - invalid operation");
        break;
    }

    DEBUG_PRINT("PC:%04X | OP:%04X | %s | V%X: %04X\n", cpu->pc - 2, opcode,
                mnemonic, x, cpu->registers[x]);
}

size_t load_rom(chip8_cpu_t *cpu, const uint8_t *rom_buffer, size_t rom_size) {
    if (cpu == NULL || rom_buffer == NULL) {
        errno = EINVAL;
        perror("Could not load rom - bad args");
        return -1;
    }

    assert(rom_size <= MAX_ROM_SIZE);

    memcpy(cpu->memory + ADDR_PROG_START, rom_buffer, rom_size);
    return rom_size;
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
