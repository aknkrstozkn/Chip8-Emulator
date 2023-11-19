//
// Created by akink on 11/12/2023.
//

#include <iostream>
#include <limits>
#include <cmath>
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
                }
                else if(NNN == 0x0EE)
                {
                    pc = stack.back();
                    stack.pop_back();
                }
                break;

            case 0x1:
                pc = NNN;
                break;
            case 0x2:
                stack.push_back(pc);
                pc = NNN;
                break;
            case 0x3:
                if(variableRegisters[xNibble] == NN)
                {
                    pc += 2;
                }
                break;
            case 0x4:
                if(variableRegisters[xNibble] != NN)
                {
                    pc += 2;
                }
                break;
            case 0x5:
                if(N == 0)
                {
                    if(variableRegisters[xNibble] == variableRegisters[yNibble])
                    {
                        pc += 2;
                    }
                }

                break;
            case 0x6:
                // Simply set the register VX to the value NN.
                variableRegisters[xNibble] = NN;
                break;
            case 0x7:
                // Add the value NN to VX.
                variableRegisters[xNibble] += NN;

                break;
            case 0x8:
                switch (N)
                {
                    case 0x0:
                        variableRegisters[xNibble] = variableRegisters[yNibble];
                        break;
                    case 0x1:
                        variableRegisters[xNibble] |= variableRegisters[yNibble];
                        break;
                    case 0x2:
                        variableRegisters[xNibble] &= variableRegisters[yNibble];
                        break;
                    case 0x3:
                        variableRegisters[xNibble] ^= variableRegisters[yNibble];
                        break;
                    case 0x4:
                        variableRegisters[0xF] = variableRegisters[yNibble] > (/*std::numeric_limits<uint8_t>::max()*/255 - variableRegisters[xNibble])
                                ? 1
                                : 0;
                        variableRegisters[xNibble] += variableRegisters[yNibble];
                        break;
                    case 0x5:
                        variableRegisters[0xF] = variableRegisters[yNibble] > variableRegisters[xNibble]
                                                 ? 0
                                                 : 1;
                        variableRegisters[xNibble] -= variableRegisters[yNibble];
                        break;
                    case 0x6:
                        variableRegisters[0xF] = (variableRegisters[xNibble]) & 0x1;
                        /*In the CHIP-8 interpreter for the original COSMAC VIP, this instruction did the following: It put the value of VY into VX, and then shifted the value in VX 1 bit to the right (8XY6) or left (8XYE). VY was not affected, but the flag register VF would be set to the bit that was shifted out.
                        However, starting with CHIP-48 and SUPER-CHIP in the early 1990s, these instructions were changed so that they shifted VX in place, and ignored the Y completely.
                        This is one of the main differences between implementations that cause problems for programs.*/
                        //variableRegisters[xNibble] = variableRegisters[yNibble];
                        variableRegisters[xNibble] >>= 1;
                        break;
                    case 0x7:
                        variableRegisters[0xF] = variableRegisters[xNibble] > variableRegisters[yNibble]
                                                 ? 0
                                                 : 1;
                        variableRegisters[xNibble] = variableRegisters[yNibble] - variableRegisters[xNibble];
                        break;
                    case 0xE:

                        variableRegisters[0xF] = (variableRegisters[xNibble] >> 7) & 0x1;
                        /*In the CHIP-8 interpreter for the original COSMAC VIP, this instruction did the following: It put the value of VY into VX, and then shifted the value in VX 1 bit to the right (8XY6) or left (8XYE). VY was not affected, but the flag register VF would be set to the bit that was shifted out.
                        However, starting with CHIP-48 and SUPER-CHIP in the early 1990s, these instructions were changed so that they shifted VX in place, and ignored the Y completely.
                        This is one of the main differences between implementations that cause problems for programs.*/
                        //variableRegisters[xNibble] = variableRegisters[yNibble];
                        variableRegisters[xNibble] <<= 1;
                        break;


                }
                break;
            case 0x9:
                if(N == 0)
                {
                    if(variableRegisters[xNibble] != variableRegisters[yNibble])
                    {
                        pc += 2;
                    }
                }
                break;
            case 0xA:
                // This sets the index register I to the value NNN.
                I = NNN;
                break;
            case 0xB:
                pc = NNN + variableRegisters[0];
                /*pc = NNN + variableRegisters[xNibble];*/
                break;
            case 0xC:
                variableRegisters[xNibble] = std::rand() & NN;
                break;
            case 0xD:
                drawSprite(xNibble, yNibble, N);
                break;
            case 0xE:
                if(NN == 0x9E && keypad[variableRegisters[xNibble]])
                {
                    pc += 2;
                }
                else if(NN == 0xA1 && !keypad[variableRegisters[xNibble]])
                {
                    pc += 2;
                }
                break;
            case 0xF:
                if(NN == 0x07)
                {
                    variableRegisters[xNibble] = delayTimer;
                }
                else if(NN == 0x15)
                {
                    delayTimer = variableRegisters[xNibble];
                }
                else if(NN == 0x18)
                {
                    soundTimer = variableRegisters[xNibble];
                }
                else if(NN == 0x1E)
                {
                    if(variableRegisters[xNibble] >= (0x1000 - I))
                    {
                        variableRegisters[0xF] =  1;
                    }

                    I += variableRegisters[xNibble];
                }
                else if(NN == 0x0A)
                {
                    pc -= 2;

                    for (int i = 0; i < keypad.size(); ++i)
                    {
                        if(keypad[i])
                        {
                            variableRegisters[xNibble] = i;
                            pc += 2;
                            break;
                        }
                    }
                }
                else if(NN == 0x29)
                {
                    I = 0x0050 + (variableRegisters[xNibble] * 5);
                }
                else if(NN == 0x33)
                {
                    uint8_t tempVal = variableRegisters[xNibble];
                    for (int i = 2; i >= 0; --i)
                    {
                        memory[I + i] = tempVal % 10;
                        tempVal /= 10;
                    }
                }
                else if(NN == 0x55)
                {
                    for (uint16_t i = 0; i <= xNibble; ++i)
                    {
                        memory[I + i] = variableRegisters[i];
                    }
                }
                else if(NN == 0x65)
                {
                    for (uint16_t i = 0; i <= xNibble; ++i)
                    {
                        variableRegisters[i] = memory[I + i];
                    }
                }
                break;
        }
    }

    return 0;
}
