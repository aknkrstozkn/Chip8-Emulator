#include <fstream>
#include <vector>
#include <SDL.h>
#include <iostream>
#include "Cpu.h"

std::vector<uint8_t> readFile(const char* filename)
{
    // open the file:
    std::streampos fileSize;
    std::ifstream file(filename, std::ios::binary);

    // get its size:
    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // read the data:
    std::vector<uint8_t> fileData(fileSize);
    file.read((char*) &fileData[0], fileSize);
    return fileData;
}

void generateFonts(std::vector<uint8_t>& memory)
{
    // 0
    memory[0x0050] = 0xF0, memory[0x0051] = 0x90, memory[0x0052] = 0x90, memory[0x0053] = 0x90, memory[0x0054] = 0xF0;
    // 1
    memory[0x0055] = 0x20, memory[0x0056] = 0x60, memory[0x0057] = 0x20, memory[0x0058] = 0x20, memory[0x0059] = 0x70;
    // 2
    memory[0x005A] = 0xF0, memory[0x005B] = 0x10, memory[0x005C] = 0xF0, memory[0x005D] = 0x80, memory[0x005E] = 0xF0;
    // 3
    memory[0x005F] = 0xF0, memory[0x0060] = 0x10, memory[0x0061] = 0xF0, memory[0x0062] = 0x10, memory[0x0063] = 0xF0;
    // 4
    memory[0x0064] = 0x90, memory[0x0065] = 0x90, memory[0x0066] = 0xF0, memory[0x0067] = 0x10, memory[0x0068] = 0x10;
    // 5
    memory[0x0069] = 0xF0, memory[0x006A] = 0x80, memory[0x006B] = 0xF0, memory[0x006C] = 0x10, memory[0x006D] = 0xF0;
    // 6
    memory[0x006E] = 0xF0, memory[0x006F] = 0x80, memory[0x0070] = 0xF0, memory[0x0071] = 0x90, memory[0x0072] = 0xF0;
    // 7
    memory[0x0073] = 0xF0, memory[0x0074] = 0x10, memory[0x0075] = 0x20, memory[0x0076] = 0x40, memory[0x0077] = 0x40;
    // 8
    memory[0x0078] = 0xF0, memory[0x0079] = 0x90, memory[0x007A] = 0xF0, memory[0x007B] = 0x90, memory[0x007C] = 0xF0;
    // 9
    memory[0x007D] = 0xF0, memory[0x007E] = 0x90, memory[0x007F] = 0xF0, memory[0x0080] = 0x10, memory[0x0081] = 0xF0;
    // A
    memory[0x0082] = 0xF0, memory[0x0083] = 0x90, memory[0x0084] = 0xF0, memory[0x0085] = 0x90, memory[0x0086] = 0x90;
    // B
    memory[0x0087] = 0xE0, memory[0x0088] = 0x90, memory[0x0089] = 0xE0, memory[0x008A] = 0x90, memory[0x008B] = 0xE0;
    // C
    memory[0x008C] = 0xF0, memory[0x008D] = 0x80, memory[0x008E] = 0x80, memory[0x008F] = 0x80, memory[0x0090] = 0xF0;
    // D
    memory[0x0091] = 0xE0, memory[0x0092] = 0x90, memory[0x0093] = 0x90, memory[0x0094] = 0x90, memory[0x0095] = 0xE0;
    // E
    memory[0x0096] = 0xF0, memory[0x0097] = 0x80, memory[0x0098] = 0xF0, memory[0x0099] = 0x80, memory[0x009A] = 0xF0;
    // F
    memory[0x009B] = 0xF0, memory[0x009C] = 0x80, memory[0x009D] = 0xF0, memory[0x009E] = 0x80, memory[0x009F] = 0x80;
}

int main() {

    std::srand(SDL_GetTicks());

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Failed to initialize the SDL2 library\n";
        return -1;
    }

    // Create an SDL window and renderer.
    SDL_Window* window = SDL_CreateWindow("CHIP-8 Emulator",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          1200, 600,
                                          SDL_WINDOW_SHOWN);
    if( window == NULL )
    {
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Set the logical size of the rendering context to your Chip-8 screen size.
    SDL_RenderSetLogicalSize(renderer, 64, 32);

    // Create a texture that will be used as a frame buffer.
    SDL_Texture* texture = SDL_CreateTexture(renderer,
                                             SDL_PIXELFORMAT_RGBA8888,
                                             SDL_TEXTUREACCESS_STREAMING,
                                             64, 32);

    // Chip-8 emulator's pixel data array, filled with emulator's screen data.
    // This should be an array of 64*32 unsigned 32-bit integers.
    // front display data
    PixelData pixelData{};

    // Update the texture with the new pixel data.
    SDL_UpdateTexture(texture, NULL, pixelData.data(), 64 * sizeof(uint32_t));

    // Clear the renderer.
    SDL_RenderClear(renderer);

    // Copy the texture to the renderer.
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    // Update the screen.
    SDL_RenderPresent(renderer);

    // Memory
    std::vector<uint8_t> memory(4096);
    generateFonts(memory);

    // An 8-bit delay timer which is decremented at a rate of 60 Hz (60 times per second) until it reaches 0
    std::uint8_t delayTimer{0};

    // An 8-bit sound timer which functions like the delay timer, but which also gives off a beeping sound as long as it’s not 0
    std::uint8_t soundTimer{0};

    // Frame duration ms for 60hz
    const std::uint32_t frameDuration = 16;

    // Mapping SDL keys to CHIP-8 keys (customize this mapping as needed)
    SDL_Keycode keymap[16] = {
            SDLK_1, SDLK_2, SDLK_3, SDLK_4,
            SDLK_q, SDLK_w, SDLK_e, SDLK_r,
            SDLK_a, SDLK_s, SDLK_d, SDLK_f,
            SDLK_z, SDLK_x, SDLK_c, SDLK_v,
    };

    // TODO use std bit set
    std::vector<bool> keypad(16);

    Cpu cpu(memory, readFile("D:\\Projects\\CPP_Projects\\Chip8-Emulator\\ROMs\\c8_test.ch8"), pixelData, keypad, delayTimer, soundTimer);

    //Hack to get window to stay up
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

        cpu.MainLoop();

        // Draw Screen
        SDL_UpdateTexture(texture, NULL, pixelData.data(), 64 * sizeof(uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

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

        SDL_Delay(frameDuration);
    }

    return 0;
}
