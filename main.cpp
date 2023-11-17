#include <fstream>
#include <vector>
#include <SDL.h>
#include <iostream>
#include "Cpu.h"

std::vector<byte> readFile(const char* filename)
{
    // open the file:
    std::streampos fileSize;
    std::ifstream file(filename, std::ios::binary);

    // get its size:
    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // read the data:
    std::vector<byte> fileData(fileSize);
    file.read((char*) &fileData[0], fileSize);
    return fileData;
}

void generateFonts(std::vector<byte>& memory)
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

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Failed to initialize the SDL2 library\n";
        return -1;
    }

    int windowWidth{64}, windowHeight{32};
    SDL_Window* window = SDL_CreateWindow("Chip 8",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          windowWidth, windowHeight,
                                          0);
    if( window == NULL )
    {
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        return 1;
    }

    //Get window surface
    SDL_Surface* screenSurface = SDL_GetWindowSurface( window );
    //Fill the surface white
    SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
    //Update the surface
    SDL_UpdateWindowSurface( window );

    // Memory
    std::vector<byte> memory(4096);
    generateFonts(memory);

    Cpu cpu(memory, readFile("D:\\Projects\\CPP_Projects\\Chip8-Emulator\\ROMs\\IBMLogo.ch8"));

    //Hack to get window to stay up
    SDL_Event e; bool quit = false; while( quit == false )
    {
        while( SDL_PollEvent( &e ) )
        {
            if( e.type == SDL_QUIT ) { quit = true; }
        }

        cpu.MainLoop();
    }

    return 0;
}
