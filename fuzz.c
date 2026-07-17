#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "./chip8.h"

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size > MAX_ROM_SIZE) {
        return 0;
    }

    chip8_cpu_t cpu;
    if (init_cpu(&cpu) != 0) {
        return 0;
    }

    if (load_rom(&cpu, data, size) != size) {
        return 0;
    }

    for (int i = 0; i < 10000; i++) {
        if (cpu.is_halted) { // we can do this and not worry about the halting instructions because pf the way they are implemented
            break;
        }
        cpu_step(&cpu);
    }

    return 0;
}
