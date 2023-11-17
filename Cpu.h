//
// Created by akink on 11/12/2023.
//
#ifndef CHIP8_EMULATOR_CPU_H
#define CHIP8_EMULATOR_CPU_H

#include <windows.h>
#include <cstdint>
#include <exception>
#include <chrono>
#include <fstream>

class Cpu
{
public:
    Cpu(std::vector<byte>& memory, std::vector<byte> instructionsVector)
    {
        this->memory = memory;
        this->instructionsVector = instructionsVector;

        cycleDeltaTime = std::chrono::high_resolution_clock::now();
        pc = 0;
    }
    int MainLoop();

private:

    int16_t pc;
    int16_t lastInstruction;
    int currentCycleIC{ 0 };
    std::chrono::time_point<std::chrono::steady_clock> cycleDeltaTime;
    std::vector<byte> memory;
    std::vector<byte> instructionsVector;
    static constexpr int cycleInstructionCount{ 700 };
    static constexpr int CycleDurationMs{ 1000 };

    std::uint8_t GetFirstNibble()
    {
        return std::uint8_t();
    }

    int GetInstructionDurations()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - cycleDeltaTime).count();
    }
};

#endif //CHIP8_EMULATOR_CPU_H
