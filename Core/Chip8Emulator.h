//
// Created by akink on 11/21/2023.
//

#ifndef CHIP8_EMULATOR_CHIP8EMULATOR_H
#define CHIP8_EMULATOR_CHIP8EMULATOR_H

#include <vector>
#include <array>
#include "Graphics.h"
#include "CPU.h"

class Chip8Emulator {
public:
    Chip8Emulator(const std::vector<uint8_t>& romInstructions, int windowWidth, int windowHeight, int fps);
    int init();
    int Run();

private:
    const int windowWidth, windowHeight;
    // Duration of each frame (Millisecond)
    const std::uint32_t frameDurationMs;

    std::vector<uint8_t> memory;
    // An 8-bit delay timer which is decremented at a rate of 60 Hz (60 times per second) until it reaches 0
    std::uint8_t delayTimer{0};
    // An 8-bit sound timer which functions like the delay timer, but which also gives off a beeping sound as long as it’s not 0
    std::uint8_t soundTimer{0};

    std::array<SDL_Keycode, 16> keymap;
    // TODO use std bit set
    std::array<bool, 16> keypad;

    std::unique_ptr<Graphics> graphics;
    std::unique_ptr<CPU> cpu;

    void importFontsToMemory();
};


#endif //CHIP8_EMULATOR_CHIP8EMULATOR_H