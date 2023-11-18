//
// Created by akink on 11/12/2023.
//
#ifndef CHIP8_EMULATOR_CPU_H
#define CHIP8_EMULATOR_CPU_H

#define NOMINMAX
#include <windows.h>
#include <cstdint>
#include <exception>
#include <fstream>
#include <vector>
#include <SDL.h>
#include <array>
#include <algorithm>

using PixelData = std::array<uint32_t, 64 * 32>;

class Cpu
{
public:
    Cpu(std::vector<uint8_t>& memory, const std::vector<uint8_t>& instructionsVector, PixelData& pixelData)
        : memory(memory), pixelData(pixelData)
    {
        std::copy(instructionsVector.begin(), instructionsVector.end(), memory.begin() + 0x200);
    }
    int MainLoop();

private:

    // 16-bit index register called “I” which is used to point at locations in memory
    std::uint16_t I{0};

    // 16 8-bit (one byte) general-purpose variable registers numbered 0 through F hexadecimal, ie 0 through 15 in decimal, called V0 through VF
    std::uint8_t variableRegisters[16]{0};

    int16_t pc{0x200};
    std::vector<uint8_t>& memory;
    PixelData& pixelData;
    static constexpr int CycleTotalIC{11};

    void clearPD()
    {
        pixelData.fill(0);
    }

    void setPixelOfPD(uint8_t val, uint16_t index)
    {
        pixelData[index] = val == 1
                           ? 0xFFFFFFFF
                           : 0x000000FF;
    }

    uint8_t getPixelOfPD(uint16_t index)
    {
        return pixelData[index] > 0x000000FF ? 1 : 0;
    }

    void drawSprite(uint8_t vx, uint8_t vy, uint8_t spriteHeight)
    {
        auto xCor = variableRegisters[vx] % 64;
        auto yCor = variableRegisters[vy] % 32;

        auto maxX = std::min(xCor + 8, 64);
        auto maxY = std::min(yCor + spriteHeight, 32);

        auto spritePoint = I;

        for (int y = yCor; y < maxY; ++y)
        {
            uint8_t spriteRow = memory[spritePoint++];
            int i = 1;
            for (int x = xCor; x < maxX; ++x)
            {
                auto index = 64 * y + x;
                uint8_t tempPixel = spriteRow >> (8 - i) & 0x1;
                setPixelOfPD(getPixelOfPD(index) ^ tempPixel, index);
                i++;
            }
        }
    }
};

#endif //CHIP8_EMULATOR_CPU_H
