//
// Created by akink on 11/12/2023.
//

#include <iostream>
#include "Cpu.h"

int Cpu::MainLoop()
{
    for (int i = 0; i < CycleTotalIC; ++i)
    {
        int16_t currentInstruction = (memory[pc] << 8) | memory[pc + 1];

        int16_t categoryNibble = (currentInstruction & 0xF000) >> 12;
        int16_t xNibble = (currentInstruction & 0x0F00) >> 8;
        int16_t yNibble = (currentInstruction & 0x00F0) >> 4;
        int16_t N = (currentInstruction & 0x000F);
        int16_t NN = (currentInstruction & 0x00FF);
        int16_t NNN = (currentInstruction & 0x0FFF);

        pc += 2;

        switch (categoryNibble)
        {
            case 0x0:
                if(NNN == 0x0E0)
                {
                    clearPD();
                    break;
                }
                break;

            case 0x1:
                pc = NNN;
                break;
            /*case 0x2:
                throw std::exception("Unimplemented 2");
                break;
            case 0x3:
                throw std::exception("Unimplemented 3");
                break;
            case 0x4:
                throw std::exception("Unimplemented 4");
                break;
            case 0x5:
                break;*/
            case 0x6:
                // Simply set the register VX to the value NN.
                variableRegisters[xNibble] = NN;
                break;
            case 0x7:
                // Add the value NN to VX.
                variableRegisters[xNibble] += NN;

                break;
            /*case 0x8:
                throw std::exception("Unimplemented 8");
                break;
            case 0x9:
                throw std::exception("Unimplemented 9");
                break;*/
            case 0xA:
                // This sets the index register I to the value NNN.
                I = NNN;
                break;
            /*case 0xB:
                throw std::exception("Unimplemented 11");
                break;
            case 0xC:
                throw std::exception("Unimplemented 12");
                break;*/
            case 0xD:
                drawSprite(xNibble, yNibble, N);
                break;
            /*case 0xE:
                throw std::exception("Unimplemented 14");
                break;
            case 0xF:
                throw std::exception("Unimplemented 14");*/
                break;
        }
    }

    return 0;
}
