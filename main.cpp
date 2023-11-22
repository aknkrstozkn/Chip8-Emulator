#include <fstream>
#include <iostream>
#include <vector>
#include "Core/Chip8Emulator.h";

std::vector<uint8_t> readROM(const char* romFilePath)
{
    // open the file:
    std::streampos fileSize;
    std::ifstream file(romFilePath, std::ios::binary);

    // get its size:
    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // read the data:
    std::vector<uint8_t> fileData(fileSize);
    file.read((char*) &fileData[0], fileSize);
    return fileData;
}

int main() {

    Chip8Emulator chip8Emulator{readROM("D:\\Projects\\CPP_Projects\\Chip8-Emulator\\ROMs\\IBMLogo.ch8"), 1200, 600, 60};
    int chip8InitStatus = chip8Emulator.init();
    if (chip8InitStatus < 0) {
        std::cerr << "Chip8Emulator initialization failed with error code: " << chip8InitStatus << '\n';
        return chip8InitStatus;
    }

    return chip8Emulator.Run();
}