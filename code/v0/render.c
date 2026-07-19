#include "render.h"
#include "chip8.h"
#include <SDL2/SDL.h>
#include <stdio.h>

#define CHIP8_WIDTH 64
#define CHIP8_HEIGHT 32
#define WINDOW_SCALE 15

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;

bool render_init(const char *title) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n",
                SDL_GetError());
        return false;
    }

    window =
        SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         CHIP8_WIDTH * WINDOW_SCALE,
                         CHIP8_HEIGHT * WINDOW_SCALE, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n",
                SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n",
                SDL_GetError());
        return false;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_STREAMING, CHIP8_WIDTH,
                                CHIP8_HEIGHT);
    if (!texture) {
        fprintf(stderr, "Texture could not be created! SDL_Error: %s\n",
                SDL_GetError());
        return false;
    }

    return true;
}

void render_update(const uint8_t *display_buffer) {
    uint32_t pixels[CHIP8_WIDTH * CHIP8_HEIGHT];

    for (int i = 0; i < CHIP8_WIDTH * CHIP8_HEIGHT; i++) {
        pixels[i] = (display_buffer[i] == 1) ? 0xFFFFFFFF : 0x000000FF;
    }

    SDL_UpdateTexture(texture, NULL, pixels, CHIP8_WIDTH * sizeof(uint32_t));

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void render_cleanup(void) {
    if (texture)
        SDL_DestroyTexture(texture);
    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window)
        SDL_DestroyWindow(window);
    SDL_Quit();
}

bool process_input_events(chip8_cpu_t *cpu) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {

        if (event.type == SDL_QUIT) {
            return true;
        }

        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                return true;
            case SDLK_x:
                cpu->keypad |= (1 << 0x0);
                break;
            case SDLK_1:
                cpu->keypad |= (1 << 0x1);
                break;
            case SDLK_2:
                cpu->keypad |= (1 << 0x2);
                break;
            case SDLK_3:
                cpu->keypad |= (1 << 0x3);
                break;
            case SDLK_q:
                cpu->keypad |= (1 << 0x4);
                break;
            case SDLK_w:
                cpu->keypad |= (1 << 0x5);
                break;
            case SDLK_e:
                cpu->keypad |= (1 << 0x6);
                break;
            case SDLK_a:
                cpu->keypad |= (1 << 0x7);
                break;
            case SDLK_s:
                cpu->keypad |= (1 << 0x8);
                break;
            case SDLK_d:
                cpu->keypad |= (1 << 0x9);
                break;
            case SDLK_z:
                cpu->keypad |= (1 << 0xA);
                break;
            case SDLK_c:
                cpu->keypad |= (1 << 0xB);
                break;
            case SDLK_4:
                cpu->keypad |= (1 << 0xC);
                break;
            case SDLK_r:
                cpu->keypad |= (1 << 0xD);
                break;
            case SDLK_f:
                cpu->keypad |= (1 << 0xE);
                break;
            case SDLK_v:
                cpu->keypad |= (1 << 0xF);
                break;
            }
        }

        if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym) {
            case SDLK_x:
                cpu->keypad &= ~(1 << 0x0);
                break;
            case SDLK_1:
                cpu->keypad &= ~(1 << 0x1);
                break;
            case SDLK_2:
                cpu->keypad &= ~(1 << 0x2);
                break;
            case SDLK_3:
                cpu->keypad &= ~(1 << 0x3);
                break;
            case SDLK_q:
                cpu->keypad &= ~(1 << 0x4);
                break;
            case SDLK_w:
                cpu->keypad &= ~(1 << 0x5);
                break;
            case SDLK_e:
                cpu->keypad &= ~(1 << 0x6);
                break;
            case SDLK_a:
                cpu->keypad &= ~(1 << 0x7);
                break;
            case SDLK_s:
                cpu->keypad &= ~(1 << 0x8);
                break;
            case SDLK_d:
                cpu->keypad &= ~(1 << 0x9);
                break;
            case SDLK_z:
                cpu->keypad &= ~(1 << 0xA);
                break;
            case SDLK_c:
                cpu->keypad &= ~(1 << 0xB);
                break;
            case SDLK_4:
                cpu->keypad &= ~(1 << 0xC);
                break;
            case SDLK_r:
                cpu->keypad &= ~(1 << 0xD);
                break;
            case SDLK_f:
                cpu->keypad &= ~(1 << 0xE);
                break;
            case SDLK_v:
                cpu->keypad &= ~(1 << 0xF);
                break;
            }
        }
    }

    return false;
}
