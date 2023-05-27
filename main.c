#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "starfield.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAX_FPS 60

int main(int argc, char *argv[]) {

    /* SDL Window init */

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Starfield",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH,
                                          SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    if (!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
                                                SDL_RENDERER_ACCELERATED |
                                                SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_DestroyWindow(window);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    /* Program init */

    int nStars = 500;
    StarField starfield = StarField_create(nStars, SCREEN_WIDTH, SCREEN_HEIGHT);

    bool quit = false;
    SDL_Event event;

    Uint64 currentTime = SDL_GetPerformanceCounter();
    Uint64 lastTime = 0;
    double deltaTime = 0.0;
    double targetFrameTime = 1000.0 / MAX_FPS;

    while (!quit) {

        lastTime = currentTime;
        currentTime = SDL_GetPerformanceCounter();
        deltaTime = (double)(currentTime - lastTime) / (double)SDL_GetPerformanceFrequency();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Limit frame rate
        if (deltaTime < targetFrameTime) {
            SDL_Delay((Uint32)(targetFrameTime - deltaTime));
            currentTime = SDL_GetPerformanceCounter();
            deltaTime = (double)(currentTime - lastTime) / (double)SDL_GetPerformanceFrequency();
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Update and draw StarField
        StarField_draw(&starfield, renderer, deltaTime);

        // Update screen
        SDL_RenderPresent(renderer);

    }

    StarField_free(&starfield);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;

}