#include <cstdint>
#include <stdint.h>

typedef struct {
    uint8_t registers[16];
    uint8_t memory[4096];

} chip8_cpu_t;
