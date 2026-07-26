#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./chip8.h"

#ifdef DEBUG_BUILD
#define DEBUG_PRINT(fmt, ...) fprintf(stderr, "DEBUG: " fmt, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...)
#endif

static inline void i_drw_vx_vy_n(chip8_cpu_t *cpu, uint8_t x, uint8_t y,
                                 uint8_t n);

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

    memset(cpu, 0, sizeof(chip8_cpu_t));
    cpu->is_halted = 0;
    memcpy(cpu->memory + ADDR_FONT_START, fontset, sizeof(fontset));
    cpu->pc = ADDR_PROG_START;

    return 0;
}

void cpu_step(chip8_cpu_t *cpu) {
    if (cpu == NULL) {
        perror("cpu_step failed:");
        errno = ENOMEM;
        return;
    }

    uint16_t opcode = (cpu->memory[cpu->pc] << 8) | cpu->memory[cpu->pc + 1];
    cpu->pc += 2;

    uint8_t x = (opcode & 0x0f00) >> 8;
    uint8_t y = (opcode & 0x00f0) >> 4;
    uint8_t kk = opcode & 0x00ff;
    uint8_t n = opcode & 0x000f;
    uint16_t nnn = opcode & 0x0fff;

    char mnemonic[32];

    switch (opcode & 0xf000) {
    case 0x0000: // 00 Family
        switch (kk) {
        case 0x00: // 0000 - HLT
            snprintf(mnemonic, sizeof(mnemonic), "HLT");
            cpu->is_halted = 1;
            break;
        case 0xe0: // 00E0 - CLS
            snprintf(mnemonic, sizeof(mnemonic), "CLS");
            memset(cpu->display_buffer, 0, MAX_DISPLAY_SIZE);
            break;
        case 0xee: // 00EE - RET
            snprintf(mnemonic, sizeof(mnemonic), "RET");
            cpu->sp--;
            cpu->pc = cpu->stack[cpu->sp];
            break;
        default:
            snprintf(mnemonic, sizeof(mnemonic), "SYS 0x%03X (IGNORED)", nnn);
            break;
        }
        break;

    case 0x1000: // 1nnn - JP addr
        snprintf(mnemonic, sizeof(mnemonic), "JP 0x%03X", nnn);
        cpu->pc = nnn;
        break;

    case 0x2000: // 2nnn - CALL addr
        snprintf(mnemonic, sizeof(mnemonic), "CALL 0x%03X", nnn);
        cpu->stack[cpu->sp] = cpu->pc;
        cpu->sp++;
        cpu->pc = nnn;
        break;

    case 0x3000: // 3xkk - SE Vx, byte
        snprintf(mnemonic, sizeof(mnemonic), "SE V%X, 0x%02X", x, kk);
        if (cpu->registers[x] == kk) {
            cpu->pc += 2;
        }
        break;

    case 0x4000: // 4xkk - SNE Vx, byte
        snprintf(mnemonic, sizeof(mnemonic), "SNE V%X, 0x%02X", x, kk);
        if (cpu->registers[x] != kk) {
            cpu->pc += 2;
        }
        break;

    case 0x5000: // 5xy0 - SE Vx, Vy
        snprintf(mnemonic, sizeof(mnemonic), "SE V%X, V%X", x, y);
        if (cpu->registers[x] == cpu->registers[y]) {
            cpu->pc += 2;
        }
        break;

    case 0x6000: // 6xkk - LD Vx, byte
        snprintf(mnemonic, sizeof(mnemonic), "LD V%X, 0x%02X", x, kk);
        cpu->registers[x] = kk;
        break;

    case 0x7000: // 7xkk - ADD Vx, byte
        snprintf(mnemonic, sizeof(mnemonic), "ADD V%X, 0x%02X", x, kk);
        cpu->registers[x] += kk;
        break;

    case 0x8000: // 8xy Family
        switch (n) {
        case 0x0: // 8xy0 - LD Vx, Vy
            snprintf(mnemonic, sizeof(mnemonic), "LD V%X, V%X", x, y);
            cpu->registers[x] = cpu->registers[y];
            break;
        case 0x1: // 8xy1 - OR Vx, Vy
            snprintf(mnemonic, sizeof(mnemonic), "OR V%X, V%X", x, y);
            cpu->registers[x] |= cpu->registers[y];
            break;
        case 0x2: // 8xy2 - AND Vx, Vy
            snprintf(mnemonic, sizeof(mnemonic), "AND V%X, V%X", x, y);
            cpu->registers[x] &= cpu->registers[y];
            break;
        case 0x3: // 8xy3 - XOR Vx, Vy
            snprintf(mnemonic, sizeof(mnemonic), "XOR V%X, V%X", x, y);
            cpu->registers[x] ^= cpu->registers[y];
            break;
        case 0x4: { // 8xy4 - ADD Vx, Vy
            snprintf(mnemonic, sizeof(mnemonic), "ADD V%X, V%X", x, y);
            uint16_t sum = cpu->registers[x] + cpu->registers[y];
            cpu->registers[x] = sum & 0xff;
            cpu->registers[0xf] = (sum > 255) ? 1 : 0;
            break;
        }
        case 0x5: { // 8xy5 - SUB Vx, Vy
            snprintf(mnemonic, sizeof(mnemonic), "SUB V%X, V%X", x, y);
            uint8_t not_borrow =
                (cpu->registers[x] >= cpu->registers[y]) ? 1 : 0;
            cpu->registers[x] = cpu->registers[x] - cpu->registers[y];
            cpu->registers[0xf] = not_borrow;
            break;
        }
        case 0x6: // 8xy6 - SHR Vx {, Vy}
            snprintf(mnemonic, sizeof(mnemonic), "SHR V%X {, V%X}", x, y);
            if (cpu->config.mode == CLASSIC) {
                cpu->registers[x] = cpu->registers[y];
            }
            uint8_t lsb = cpu->registers[x] & 0x1;
            cpu->registers[x] >>= 1;
            cpu->registers[0xf] = lsb;
            break;
        case 0x7: { // 8xy7 - SUBN Vx, Vy
            snprintf(mnemonic, sizeof(mnemonic), "SUBN V%X, V%X", x, y);
            uint8_t not_borrow =
                (cpu->registers[y] >= cpu->registers[x]) ? 1 : 0;
            cpu->registers[x] = cpu->registers[y] - cpu->registers[x];
            cpu->registers[0xf] = not_borrow;
            break;
        }
        case 0xe: // 8xyE - SHL Vx {, Vy}
            snprintf(mnemonic, sizeof(mnemonic), "SHL V%X {, V%X}", x, y);
            if (cpu->config.mode == CLASSIC) {
                cpu->registers[x] = cpu->registers[y];
            }
            uint8_t msb = (cpu->registers[x] & 0x80) >> 7;
            cpu->registers[x] <<= 1;
            cpu->registers[0xf] = msb;
            break;
        default:
            errno = EINVAL;
            snprintf(mnemonic, sizeof(mnemonic), "invalid operation");
            break;
        }
        break;

    case 0x9000: // 9xy0 - SNE Vx, Vy
        snprintf(mnemonic, sizeof(mnemonic), "SNE V%X, V%X", x, y);
        if (cpu->registers[x] != cpu->registers[y]) {
            cpu->pc += 2;
        }
        break;

    case 0xA000: // Annn - LD I, addr
        snprintf(mnemonic, sizeof(mnemonic), "LD I, 0x%03X", nnn);
        cpu->I = nnn;
        break;

    case 0xB000: // Bnnn - JP V0, addr
        snprintf(mnemonic, sizeof(mnemonic), "JP V0, 0x%03X", nnn);
        cpu->pc = nnn + cpu->registers[0];
        break;

    case 0xC000: // Cxkk - RND Vx, byte
        snprintf(mnemonic, sizeof(mnemonic), "RND V%X, 0x%02X", x, kk);
        cpu->registers[x] = (rand() % 256) & kk;
        break;

    case 0xD000: // Dxyn - DRW Vx, Vy, nibble
        snprintf(mnemonic, sizeof(mnemonic), "DRW V%X, V%X, %X", x, y, n);
        i_drw_vx_vy_n(cpu, x, y, n);
        break;

    case 0xE000: // Ex Family
        switch (kk) {
        case 0x9E: // Ex9E - SKP Vx
            snprintf(mnemonic, sizeof(mnemonic), "SKP V%X", x);
            if ((cpu->keypad & (1 << cpu->registers[x])) != 0) {
                cpu->pc += 2;
            }
            break;
        case 0xA1: // ExA1 - SKNP Vx
            snprintf(mnemonic, sizeof(mnemonic), "SKNP V%X", x);
            if ((cpu->keypad & (1 << cpu->registers[x])) == 0) {
                cpu->pc += 2;
            }
            break;
        default:
            errno = EINVAL;
            snprintf(mnemonic, sizeof(mnemonic), "invalid operation");
            break;
        }
        break;

    case 0xF000: // Fx Family
        switch (kk) {
        case 0x07: // Fx07 - LD Vx, DT
            snprintf(mnemonic, sizeof(mnemonic), "LD V%X, DT", x);
            cpu->registers[x] = cpu->delay_timer;
            break;
        case 0x0A: // Fx0A - LD Vx, K
            snprintf(mnemonic, sizeof(mnemonic), "LD V%X, K", x);
            int key_pressed = -1;
            for (int i = 0; i < 16; i++) {
                if (cpu->keypad & (1 << i)) {
                    key_pressed = i;
                    break;
                }
            }
            if (key_pressed == -1) {
                cpu->pc -= 2;
            } else {
                cpu->registers[x] = (uint8_t)key_pressed;
            }
            break;
        case 0x15: // Fx15 - LD DT, Vx
            snprintf(mnemonic, sizeof(mnemonic), "LD DT, V%X", x);
            cpu->delay_timer = cpu->registers[x];
            break;
        case 0x18: // Fx18 - LD ST, Vx
            snprintf(mnemonic, sizeof(mnemonic), "LD ST, V%X", x);
            cpu->sound_timer = cpu->registers[x];
            break;
        case 0x1E: // Fx1E - ADD I, Vx
            snprintf(mnemonic, sizeof(mnemonic), "ADD I, V%X", x);
            cpu->I += cpu->registers[x];
            break;
        case 0x29: // Fx29 - LD F, Vx
            snprintf(mnemonic, sizeof(mnemonic), "LD F, V%X", x);
            cpu->I = ADDR_FONT_START + (cpu->registers[x] * 5);
            break;
        case 0x33: // Fx33 - LD B, Vx
            snprintf(mnemonic, sizeof(mnemonic), "LD B, V%X", x);
            cpu->memory[cpu->I] = cpu->registers[x] / 100;
            cpu->memory[cpu->I + 1] = (cpu->registers[x] / 10) % 10;
            cpu->memory[cpu->I + 2] = cpu->registers[x] % 10;
            break;
        case 0x55: // Fx55 - LD [I], Vx
            snprintf(mnemonic, sizeof(mnemonic), "LD [I], V%X", x);
            for (int i = 0; i <= x; i++) {
                cpu->memory[cpu->I + i] = cpu->registers[i];
            }
            break;
        case 0x65: // Fx65 - LD Vx, [I]
            snprintf(mnemonic, sizeof(mnemonic), "LD V%X, [I]", x);
            for (int i = 0; i <= x; i++) {
                cpu->registers[i] = cpu->memory[cpu->I + i];
            }
            break;
        default:
            errno = EINVAL;
            snprintf(mnemonic, sizeof(mnemonic), "invalid operation");
            break;
        }
        break;

    default:
        errno = EINVAL;
        snprintf(mnemonic, sizeof(mnemonic), "invalid operation");
        break;
    }

    DEBUG_PRINT("PC:%04X | OP:%04X | %s\n", cpu->pc - 2, opcode, mnemonic);
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

void cpu_update_timers(chip8_cpu_t *cpu) {
    if (cpu->delay_timer > 0) {
        cpu->delay_timer--;
    }
    if (cpu->sound_timer > 0) {
        cpu->sound_timer--;
    }
}

static inline void i_drw_vx_vy_n(chip8_cpu_t *cpu, uint8_t x, uint8_t y,
                                 uint8_t n) {
    uint8_t start_x = cpu->registers[x] % DISPLAY_WIDTH;
    uint8_t start_y = cpu->registers[y] % DISPLAY_HEIGHT;

    cpu->registers[0xF] = 0;

    for (int row = 0; row < n; row++) {
        // correction number one
        // The program will still crash this ay, but now it crashes in a
        // controlled manner that an attacker (hopefully) cannot exploit
        assert((cpu->I + row) < MAX_ROM_SIZE);
        assert((cpu->I + row) > 0);

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
}
