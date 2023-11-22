//
// Created by akink on 11/21/2023.
//

#include <SDL.h>
#include <iostream>
#include "Chip8Emulator.h"
#include "Graphics.h"
#include "CPU.h"

Chip8Emulator::Chip8Emulator(const std::vector<uint8_t>& romInstructions, int windowWidth, int windowHeight, int fps)
    : windowWidth(windowWidth), windowHeight(windowHeight), frameDurationMs(1000 / fps)
{
    memory.resize(4096);
    std::copy(romInstructions.begin(), romInstructions.end(), memory.begin() + 0x200);
    importFontsToMemory();
    // Mapping SDL keys to CHIP-8 keys (customize this mapping as needed)
    keymap = {
            SDLK_x, SDLK_1, SDLK_2, SDLK_3,
            SDLK_q, SDLK_w, SDLK_e, SDLK_a,
            SDLK_s, SDLK_d, SDLK_z, SDLK_c,
            SDLK_4, SDLK_r, SDLK_f, SDLK_v,
    };
    std::srand(SDL_GetTicks());
}

int Chip8Emulator::init()
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        std::cout << "Failed to initialize the SDL2 library\n";
        return -1;
    }

    graphics = std::make_unique<Graphics>(windowWidth, windowHeight, memory);
    int graphicsInitStatus = graphics->init();
    if (graphicsInitStatus < 0) {
        std::cerr << "Graphics initialization failed with error code: " << graphicsInitStatus << '\n';
        // Perform any necessary cleanup or error handling
        return graphicsInitStatus;
    }

    cpu = std::make_unique<CPU>(memory, *graphics, keypad, delayTimer, soundTimer);
    return 0;
}

int Chip8Emulator::Run()
{
    SDL_Event e; bool quit = false;
    while( quit == false )
    {
        while( SDL_PollEvent( &e ) )
        {
            if( e.type == SDL_QUIT )
            {
                quit = true;
            }
            else if( e.type == SDL_KEYDOWN)
            {
                for (int i = 0; i < keypad.size(); ++i)
                {
                    if (e.key.keysym.sym == keymap[i])
                    {
                        keypad[i] = true;
                    }
                }
            }
            else if(e.type == SDL_KEYUP)
            {
                for (int i = 0; i < keypad.size(); ++i)
                {
                    if (e.key.keysym.sym == keymap[i])
                    {
                        keypad[i] = false;
                    }
                }
            }
        }

        cpu->DoFrameInstructions();

        graphics->renderTexture();

        // Timer registers
        if(delayTimer > 0)
        {
            delayTimer--;
        }

        if(soundTimer > 0)
        {
            soundTimer = soundTimer - 1 < 0 ? 0 : soundTimer - 1;
        }
        // -----

        SDL_Delay(frameDurationMs);
    }

    return 0;
}

void Chip8Emulator::importFontsToMemory()
{
    uint8_t fonts[80]
            {
                    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
                    0x20, 0x60, 0x20, 0x20, 0x70, // 1
                    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
                    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
                    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
                    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
                    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
                    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
                    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
                    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
                    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
                    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
                    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
                    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
                    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
                    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
            };
    std::copy(fonts, fonts + 80, memory.begin() + 0x50);
}