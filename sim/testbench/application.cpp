#include "application.h"
#include "computer.h"

#include <iostream>
#include <vector>
#include <thread>

#include "SDL.h"

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event event;

static std::vector<uint16_t> displayPixels;

static bool s_finished = false;

void Setup() {
    SetupComputer();
}

void Close() {
    //ImGuiEnd();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    SDL_Quit();

    CloseComputer();
}

void StartFrame() {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            s_finished = true;
        //ImGui_ImplSDL2_ProcessEvent(&event);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    //ImGuiStartFrame();
}
void EndFrame() {
    //ImGuiRender(window, renderer);
    
    SDL_RenderPresent(renderer);
}

extern "C" void WriteDisplayPixel_256x256px_5bxRGB(uint32_t pixelAddress, uint32_t colour, uint32_t reset) {
    if (reset) {
        std::fill(displayPixels.begin(), displayPixels.end(), 0);
        return;
    }

    displayPixels[pixelAddress] = colour;

    return;
}

void RenderDisplayPixels() {
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            int pixel = displayPixels[(i * 256) + j];
            int r = (((pixel & 0b0111110000000000) >> 10) * 255) / 31;
            int g = (((pixel & 0b0000001111100000) >> 5) * 255) / 31;
            int b = (((pixel & 0b0000000000011111)) * 255) / 31;

            SDL_RenderSetScale(renderer, 2, 2);

            SDL_SetRenderDrawColor(renderer, r, g, b, 255);
            SDL_RenderDrawPoint(renderer, j, i);

            SDL_RenderSetScale(renderer, 1, 1);
        }
    }
}

void Rendering() {
    displayPixels.resize(256 * 256, 0);

    SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(256 * 2, 256 * 2, 0, &window, &renderer);
	SDL_RenderSetVSync(renderer, 1);
	SDL_RenderSetScale(renderer, 1, 1);

    //ImGuiSetup(window, renderer);

    while (!s_finished) {
        StartFrame();

        RenderDisplayPixels();

        EndFrame();
    }
}

void Run() {
    std::thread rendering(Rendering);

    RunComputer(s_finished);

    rendering.join();
}
