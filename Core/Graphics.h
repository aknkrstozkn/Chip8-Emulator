//
// Created by akink on 11/21/2023.
//

#ifndef CHIP8_EMULATOR_GRAPHICS_H
#define CHIP8_EMULATOR_GRAPHICS_H

#include <array>
#include <SDL.h>
#include <vector>

class Graphics {

public:
    static constexpr int LOGICAL_WIDTH {64};
    static constexpr int LOGICAL_HEIGHT {32};

    /// Create SDL Window with specified, pixel vise, size values.
    /// \param windowWidth Windows width.
    /// \param windowHeight Windows height.
    /// \param logicalWidth Windows logical(emulation) width.
    /// \param logicalHeight Windows logical(emulation) height.
    /// \return
    Graphics(int windowWidth, int windowHeight, std::vector<uint8_t>& memory);

    ~Graphics();

    int init();
    void renderTexture();

    void clearPixelData()
    {
        pixelData.fill(0x000000FF);
    }

    /// Draw specified sprite to pixel data
    /// \param xCor x coordinate of the sprite
    /// \param yCor y coordinate of the sprite
    /// \param spriteHeight height of the sprite
    /// \param spriteMemoryLocation memory location of the sprite
    /// \return return 1 if any pixel turned of if not return 0
    std::uint8_t drawSprite(uint8_t xCor, uint8_t yCor, uint8_t spriteHeight, std::uint16_t spriteMemoryLocation);

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;

    // Chip-8 emulator's pixel data array, filled with emulator's screen data.
    // This should be an array of 64*32 unsigned 32-bit integers.
    std::array<uint32_t, LOGICAL_WIDTH * LOGICAL_HEIGHT> pixelData{0x000000FF};

    const int windowWidth, windowHeight;

    std::vector<uint8_t>& memory;

    void setPixelOfPixelData(uint8_t val, uint16_t index)
    {
        pixelData[index] = val == 1
                           ? 0xFFFFFFFF
                           : 0x000000FF;
    }

    uint8_t getPixelOfPixelData(uint16_t index)
    {
        return pixelData[index] > 0x000000FF ? 1 : 0;
    }
};

#endif //CHIP8_EMULATOR_GRAPHICS_H