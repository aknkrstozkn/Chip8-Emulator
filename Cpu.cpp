//
// Created by akink on 11/12/2023.
//
#include <iostream>
#include "Cpu.h"

int Cpu::MainLoop()
{

    lastInstruction = (instructionsVector[pc] << 8) | instructionsVector[pc + 1];

    int16_t categoryNibble = (lastInstruction & 0xF000) >> 12;
    int16_t xNibble = (lastInstruction & 0x0F00) >> 8;
    int16_t yNibble = (lastInstruction & 0x00F0) >> 4;
    int16_t N = (lastInstruction & 0x000F);
    int16_t NN = (lastInstruction & 0x00FF);
    int16_t NNN = (lastInstruction & 0x0FFF);

    //pc += 2;
    if(pc >= instructionsVector.size())
    {
        return 1;
    }

    switch (categoryNibble)
    {
        case 0x0:
            if(NNN == 0x0E0)
            {

            }
            break;
            /*case 0x1:
                throw std::exception("Unimplemented 1");
                break;
            case 0x2:
                throw std::exception("Unimplemented 2");
                break;
            case 0x3:
                throw std::exception("Unimplemented 3");
                break;
            case 0x4:
                throw std::exception("Unimplemented 4");
                break;
            case 0x5:
                throw std::exception("Unimplemented 5");
                break;
            case 0x6:
                throw std::exception("Unimplemented 6");
                break;
            case 0x7:
                throw std::exception("Unimplemented 7");
                break;
            case 0x8:
                throw std::exception("Unimplemented 8");
                break;
            case 0x9:
                throw std::exception("Unimplemented 9");
                break;
            case 0x10:
                throw std::exception("Unimplemented 10");
                break;
            case 0x11:
                throw std::exception("Unimplemented 11");
                break;
            case 0x12:
                throw std::exception("Unimplemented 12");
                break;
            case 0x13:
                throw std::exception("Unimplemented 13");
                break;
            case 0x14:
                throw std::exception("Unimplemented 14");*/
            break;

    }

    if(++currentCycleIC == cycleInstructionCount)
    {
        auto remainingTime = CycleDurationMs - GetInstructionDurations();
        if(remainingTime < 0)
        {
            return 1;
        }
        Sleep(remainingTime);

        currentCycleIC = 0;
        cycleDeltaTime = std::chrono::high_resolution_clock::now();
    }

    return 0;
}
