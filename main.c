#include <SDL2/SDL_timer.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./chip8.h"
#include "./render.h"

size_t read_rom(uint8_t *rom_buffer, char *filename);
void handle_sigint(int signum);

static volatile sig_atomic_t emulator_running = 1;

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Usage: chip_emulator [rom]\n");
        return EXIT_FAILURE;
    }

    chip8_cpu_t *cpu = (chip8_cpu_t *)malloc(sizeof(chip8_cpu_t));
    init_cpu(cpu);

    uint8_t rom_buffer[MAX_ROM_SIZE];
    size_t rom_size = read_rom(rom_buffer, argv[1]);
    load_rom(cpu, rom_buffer, rom_size);

    if (!render_init("CHIP8 Emulator")) {
        return EXIT_FAILURE;
    }

    signal(SIGINT, handle_sigint);

    while (emulator_running) {

        if (process_input_events(cpu)) {
            break;
        }

        for (int i = 0; i < 8; i++) {
            if (!cpu->is_halted) {
                cpu_step(cpu);
            }
        }

        cpu_update_timers(cpu);
        render_update(cpu->display_buffer);

        SDL_Delay(16);
    }

    render_cleanup();
    free(cpu);
    return EXIT_SUCCESS;
}

size_t read_rom(uint8_t *rom_buffer, char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error Reading ROM - could not open file");
        return -1;
    }

    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    if (file_size > MAX_ROM_SIZE) {
        perror("Error reading ROM - ROM too big");
        return -1;
    }

    size_t bytes_read = fread(rom_buffer, 1, file_size, file);
    if (bytes_read != file_size) {
        printf("warning - expected %zu bytes, but got %zu bytes\n", file_size,
               bytes_read);
    }

    printf("ROM %s read successfully \n", filename);
    fclose(file);
    return bytes_read;
}

void handle_sigint(int signum) {
    (void)signum;
    emulator_running = 0;
}
