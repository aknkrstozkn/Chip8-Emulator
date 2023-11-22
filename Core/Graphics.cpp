//
// Created by akink on 11/21/2023.
//

#include "Graphics.h"

Graphics::Graphics(int windowWidth, int windowHeight, std::vector<uint8_t>& memory)
        : windowWidth(windowWidth), windowHeight(windowHeight), memory(memory)
{

}

Graphics::~Graphics()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(texture);
}

int Graphics::init()
{
    // Create an SDL window.
    window = SDL_CreateWindow("CHIP-8 Emulator",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              windowWidth, windowHeight,
                              SDL_WINDOW_SHOWN);
    if( window == nullptr )
    {
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        return - 1;
    }

    // Create an SDL renderer.
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    // Set the logical size of the rendering context to your Chip-8 screen size.
    SDL_RenderSetLogicalSize(renderer, LOGICAL_WIDTH, LOGICAL_HEIGHT);

    // Create a texture that will be used as a frame buffer.
    texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                LOGICAL_WIDTH, LOGICAL_HEIGHT);

    renderTexture();

    return 0;
}

void Graphics::renderTexture()
{
    SDL_UpdateTexture(texture, NULL, pixelData.data(), LOGICAL_WIDTH * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

uint8_t Graphics::drawSprite(uint8_t xCor, uint8_t yCor, uint8_t spriteHeight, std::uint16_t spriteMemoryLocation)
{
    xCor %= 64;
    yCor %= 32;

    auto maxX = std::min(xCor + 8, 64);
    auto maxY = std::min(yCor + spriteHeight, 32);

    bool isAnyPixelTurnedOff = false;

    for (int y = yCor; y < maxY; ++y)
    {
        uint8_t spriteRow = memory[spriteMemoryLocation++];
        int i = 1;
        for (int x = xCor; x < maxX; ++x)
        {
            auto index = 64 * y + x;
            uint8_t spritePixel = spriteRow >> (8 - i) & 0x1;
            uint8_t pdPixel = getPixelOfPixelData(index);

            if(!isAnyPixelTurnedOff && pdPixel == 1 && spritePixel == 1)
            {
                isAnyPixelTurnedOff = true;
            }

            setPixelOfPixelData(pdPixel ^ spritePixel, index);
            i++;
        }
    }

    return isAnyPixelTurnedOff ? 1 : 0;
}