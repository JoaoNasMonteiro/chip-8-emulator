#include "render.h"
#include <SDL2/SDL.h>
#include <stdio.h>

// We define these here to keep the renderer completely decoupled from the CPU headers
#define CHIP8_WIDTH 64
#define CHIP8_HEIGHT 32
#define WINDOW_SCALE 15 // Scales the tiny 64x32 display up to a 960x480 window

// Static variables keep the SDL state completely hidden from the rest of the program
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Texture* texture = NULL;

bool render_init(const char* title) {
    // 1. Initialize SDL Video Subsystem
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // 2. Create the Window
    window = SDL_CreateWindow(title, 
                              SDL_WINDOWPOS_CENTERED, 
                              SDL_WINDOWPOS_CENTERED, 
                              CHIP8_WIDTH * WINDOW_SCALE, 
                              CHIP8_HEIGHT * WINDOW_SCALE, 
                              SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // 3. Create the GPU Renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // 4. Create the Texture (The blank canvas in GPU memory)
    texture = SDL_CreateTexture(renderer, 
                                SDL_PIXELFORMAT_RGBA8888, 
                                SDL_TEXTUREACCESS_STREAMING, 
                                CHIP8_WIDTH, 
                                CHIP8_HEIGHT);
    if (!texture) {
        fprintf(stderr, "Texture could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

void render_update(const uint8_t* display_buffer) {
    // SDL requires a 32-bit color value for every pixel.
    uint32_t pixels[CHIP8_WIDTH * CHIP8_HEIGHT];

    for (int i = 0; i < CHIP8_WIDTH * CHIP8_HEIGHT; i++) {
        // If the CPU buffer has a 1, make it White. Else, make it Black.
        pixels[i] = (display_buffer[i] == 1) ? 0xFFFFFFFF : 0x000000FF;
    }

    // Send the translated pixels to the GPU texture
    SDL_UpdateTexture(texture, NULL, pixels, CHIP8_WIDTH * sizeof(uint32_t));

    // Clear the screen, copy the new texture over, and present it to the monitor
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void render_cleanup(void) {
    if (texture) SDL_DestroyTexture(texture);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

bool render_check_quit(void) {
    SDL_Event e;
    // Process all pending events in the queue
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            return true; // The user clicked the 'X' button
        }
    }
    return false;
}
