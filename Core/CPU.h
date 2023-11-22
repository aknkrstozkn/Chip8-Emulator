//
// Created by akink on 11/12/2023.
//
#ifndef CHIP8_EMULATOR_CPU_H
#define CHIP8_EMULATOR_CPU_H

#include <cstdint>
#include <exception>
#include <fstream>
#include <vector>
#include <SDL.h>
#include <array>
#include <algorithm>
#include <cstdlib>
#include "Graphics.h"

class CPU {

public:
    CPU(std::vector<uint8_t>& memory, Graphics& graphics, std::array<bool, 16>& keypad, uint8_t& delayTimer, uint8_t& soundTimer);
    int DoFrameInstructions();

private:
    std::vector<uint8_t>& memory;
    std::vector<uint16_t> stack;

    Graphics& graphics;

    // Program Counter to track CPU instructions.
    int16_t pc{0x200};

    // 16-bit index register called “I” which is used to point at locations in memory
    std::uint16_t I{0};

    // 16 8-bit (one byte) general-purpose variable registers numbered 0 through F hexadecimal, ie 0 through 15 in decimal, called V0 through VF
    std::uint8_t variableRegisters[16]{0};

    std::array<bool, 16>& keypad;

    uint8_t& delayTimer, soundTimer;

    static constexpr int CycleTotalIC{10};
};

#endif //CHIP8_EMULATOR_CPU_H