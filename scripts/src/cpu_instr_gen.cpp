#include <iostream>
#include <vector>
#include <fstream>
#include <bitset>
#include "memory.h"

// Micro Instructions
#define AI   0b0000000000000000000000000000000000000000000000000000000000000001
#define AO   0b0000000000000000000000000000000000000000000000000000000000000010
#define BI   0b0000000000000000000000000000000000000000000000000000000000000100
#define BO   0b0000000000000000000000000000000000000000000000000000000000001000
#define CI   0b0000000000000000000000000000000000000000000000000000000000010000
#define CO   0b0000000000000000000000000000000000000000000000000000000000100000
#define DI   0b0000000000000000000000000000000000000000000000000000000001000000
#define DO   0b0000000000000000000000000000000000000000000000000000000010000000
#define EI   0b0000000000000000000000000000000000000000000000000000000100000000
#define EO   0b0000000000000000000000000000000000000000000000000000001000000000
#define II   0b0000000000000000000000000000000000000000000000000000010000000000
#define IO   0b0000000000000000000000000000000000000000000000000000100000000000
#define MI   0b0000000000000000000000000000000000000000000000000001000000000000
#define RI   0b0000000000000000000000000000000000000000000000000010000000000000
#define RO   0b0000000000000000000000000000000000000000000000000100000000000000
#define PCE  0b0000000000000000000000000000000000000000000000001000000000000000
#define PCI  0b0000000000000000000000000000000000000000000000010000000000000000
#define PCO  0b0000000000000000000000000000000000000000000000100000000000000000
#define SO   0b0000000000000000000000000000000000000000000001000000000000000000
#define S0   0b0000000000000000000000000000000000000000000010000000000000000000
#define S1   0b0000000000000000000000000000000000000000000100000000000000000000
#define S2   0b0000000000000000000000000000000000000000001000000000000000000000
#define FI   0b0000000000000000000000000000000000000000010000000000000000000000
#define AEI  0b0000000000000000000000000000000000000000100000000000000000000000
#define AEO  0b0000000000000000000000000000000000000001000000000000000000000000
#define BEI  0b0000000000000000000000000000000000000010000000000000000000000000
#define BEO  0b0000000000000000000000000000000000000100000000000000000000000000
#define CEI  0b0000000000000000000000000000000000001000000000000000000000000000
#define CEO  0b0000000000000000000000000000000000010000000000000000000000000000
#define DEI  0b0000000000000000000000000000000000100000000000000000000000000000
#define DEO  0b0000000000000000000000000000000001000000000000000000000000000000
#define SPI  0b0000000000000000000000000000000010000000000000000000000000000000
#define SPO  0b0000000000000000000000000000000100000000000000000000000000000000
#define SBI  0b0000000000000000000000000000001000000000000000000000000000000000
#define SBO  0b0000000000000000000000000000010000000000000000000000000000000000
#define CSI  0b0000000000000000000000000000100000000000000000000000000000000000
#define CSO  0b0000000000000000000000000001000000000000000000000000000000000000
#define DSI  0b0000000000000000000000000010000000000000000000000000000000000000
#define SSI  0b0000000000000000000000000100000000000000000000000000000000000000
#define ESI  0b0000000000000000000000001000000000000000000000000000000000000000
#define SSO0 0b0000000000000000000000010000000000000000000000000000000000000000
#define SSO1 0b0000000000000000000000100000000000000000000000000000000000000000
#define SSO2 0b0000000000000000000001000000000000000000000000000000000000000000
#define SSO  0b0000000000000000000010000000000000000000000000000000000000000000
#define RSBI 0b0000000000000000000100000000000000000000000000000000000000000000
#define RSBO 0b0000000000000000001000000000000000000000000000000000000000000000
#define RSTI 0b0000000000000000010000000000000000000000000000000000000000000000
#define RSTO 0b0000000000000000100000000000000000000000000000000000000000000000

#define R    0b1000000000000000000000000000000000000000000000000000000000000000

// Nop
#define NOP PCO|MI, SCODE|PCE|RO|II

// Alu
#define ADD  SO          // S000 - add
#define SUB  SO|S0       // S001 - sub
#define INC  SO|S1       // S010 - inc
#define DEC  SO|S1|S0    // S011 - dec
#define MULT SO|S2       // S100 - mult
#define DIV  SO|S2|S0    // S101 - div
#define AND  SO|S2|S1    // S110 - and
#define OR   SO|S2|S1|S0 // S111 - or

// Segments
// SSO000 - none
#define SCODE  SSO0 // SSO001 - code
#define SDATA  SSO1 // SSO010 - data
#define SSTACK SSO1|SSO0 // SSO011 - stack
#define SDRIVE SSO2 // SSO100 - drive
#define SEXTRA SSO2|SSO0 // SSO101 - extra

// i  - Immediate.
// ns - No Segment.

// Instructions
std::vector<std::vector<uint64_t>> instructions = {
    { NOP, R },                                             // 000000 - nop
    { NOP, RSTI|RSBO, R },                                  // 000001 - mov
    { NOP, PCO|MI, RSTI|RO|SCODE|PCE, R },                  // 000010 - mov i
    { NOP, MI|RSBO, SSO|RSTI|RO, R },                       // 000011 - ldr
    { NOP, PCO|MI, MI|SCODE|RO|PCE, SSO|RSTI|RO, R },       // 000100 - ldr i
    { NOP, MI|RSBO, RSTI|RO, R },                           // 000101 - ldr   ns
    { NOP, PCO|MI, MI|SCODE|RO|PCE, RSTI|RO, R },           // 000110 - ldr i ns
    { NOP, MI|RSTO, SSO|RI|RSBO, R },                       // 000111 - str
    { NOP, PCO|MI, MI|SCODE|RO|PCE, SSO|RI|RSBO, R },       // 001000 - str i
    { NOP, MI|RSTO, RI|RSBO, R },                           // 001001 - str   ns
    { NOP, PCO|MI, MI|SCODE|RO|PCE, RI|RSBO, R },           // 001010 - str i ns

    { NOP, MI|RSBO, SO|RSTI|AEO, R },                       // 001011 - ldepa
    { NOP, MI|RSBO, RSTI|AEO, R },                          // 001100 - ldepa ns
    { NOP, MI|RSTO, SO|AEI|RSBO, R },                       // 001101 - stepa
    { NOP, MI|RSTO, AEI|RSBO, R },                          // 001110 - stepa ns

    { NOP, MI|RSBO, SO|RSTI|BEO, R },                       // 001111 - ldepb
    { NOP, MI|RSBO, RSTI|BEO, R },                          // 010000 - ldepb ns
    { NOP, MI|RSTO, SO|BEI|RSBO, R },                       // 010001 - stepb
    { NOP, MI|RSTO, BEI|RSBO, R },                          // 010010 - stepb ns

    { NOP, MI|RSBO, SO|RSTI|CEO, R },                       // 010011 - ldepc
    { NOP, MI|RSBO, RSTI|CEO, R },                          // 010100 - ldepc ns
    { NOP, MI|RSTO, SO|CEI|RSBO, R },                       // 010101 - stepc
    { NOP, MI|RSTO, CEI|RSBO, R },                          // 010110 - stepc ns

    { NOP, MI|RSBO, SO|RSTI|DEO, R },                       // 010111 - ldepd
    { NOP, MI|RSBO, RSTI|DEO, R },                          // 011000 - ldepd ns
    { NOP, MI|RSTO, SO|DEI|RSBO, R },                       // 011001 - stepd
    { NOP, MI|RSTO, DEI|RSBO, R },                          // 011010 - stepd ns

    { NOP, RSBO|PCI, R },                                   // 011011 - jmp
    { NOP, PCO|MI, PCI|RO|SCODE, R },                       // 011100 - jmp i
    { NOP, RSBO|CSI, PCI, R },                              // 011101 - jmpf
    { NOP, PCO|MI, CSI|RO|SCODE, PCI, R },                  // 011110 - jmpf i
    { NOP, SUB|FI, R },                                     // 011111 - cmp
    { NOP, R },                                             // 100000 - jz
    { NOP, PCE, R },                                        // 100001 - jz i
    { NOP, R },                                             // 100010 - jc
    { NOP, PCE, R },                                        // 100011 - jc is

    { NOP, RSTI|ADD|FI, R },                                // 100100 - add
    { NOP, RSTI|SUB|FI, R },                                // 100101 - sub
    { NOP, RSTI|INC|FI, R },                                // 100110 - inc
    { NOP, RSTI|DEC|FI, R },                                // 100111 - dec
    { NOP, RSTI|MULT|FI, R },                               // 101000 - mult
    { NOP, RSTI|DIV|FI, R },                                // 101001 - div
    { NOP, RSTI|AND|FI, R },                                // 101010 - and
    { NOP, RSTI|OR|FI, R },                                 // 101011 - or

    { NOP, SPO|MI, SSTACK|RSBO|RI, SPO|AI, SPI|DEC, SSTACK|RO|RSBI, R },                                                // 101100 - push
    { NOP, SPO|AI, SPI|INC, SPO|MI, SSTACK|RO|RSTI, R },                                                                // 101101 - pop
    { NOP, SPO|MI, SSTACK|PCO|RI, RSBO|PCI, SPO|AI, DEC|SPI, RSBI|PCO, R },                                             // 101110 - call
    { NOP, PCO|MI, RO|AI|SCODE|PCE, SPO|MI, SSTACK|PCO|RI, AO|PCI, SPO|AI, DEC|AI|SPI, R },                             // 101111 - call i
    { NOP, SPO|MI, SSTACK|PCO|RI, SPO|AI, DEC|MI, SSTACK|CSO|RI, DEC|SPI, RSBO|CSI, PCI, R },                           // 110000 - callf
    { NOP, PCO|MI, RO|BI|SCODE|PCE, SPO|MI, SSTACK|PCO|RI, SPO|AI, DEC|AI, SSTACK|CSO|RI, DEC|SPI, BO|CSI, PCI, R },    // 110001 - callf i
    { NOP, SPO|AI, INC|MI|AI, SSTACK|RO|PCI, R },                                                                       // 110010 - ret
    { NOP, SPO|AI, INC|MI|AI, SSTACK|RO|CSI, INC|MI|SPI, SSTACK|RO|PCI, R },                                            // 110011 - retf
};

#define ROM_SIZE 0b111111111111 * 8 // 12 bit by 64 bit

void ReplaceRomInstruction(uint16_t startAddress, uint64_t instruction, char* rom) {
    for (int k = 7; k >= 0; k--) {
        uint8_t instructionByte = (instruction >> (k * 8)) & 0x000000FF;
        rom[startAddress] = instructionByte;
        
        std::cout << std::bitset<14>(startAddress) << " - "
         << std::bitset<8>(instructionByte) << "\n";
        
        startAddress++;
    }
}

int main(int argc, char* argv[]) {
    char* rom = new char[ROM_SIZE];

    for (int i = 0; i < ROM_SIZE; i++)
        rom[i] = 0x00;

    for (int i = 0; i < instructions.size(); i++) {
        for (int j = 0; j < instructions[i].size(); j++) {
            uint16_t address = 0;
            address = address | j;
            address = address | (i << 4);
            address = address << 3;
            ReplaceRomInstruction(address, instructions[i][j], rom);
        }
        std::cout << "\n";
    }

    memcpy(rom + (((ROM_SIZE) / 4) * 1) + 2, rom, (ROM_SIZE) / 4);  // 01 - ZF
    memcpy(rom + (((ROM_SIZE) / 4) * 2) + 4, rom, (ROM_SIZE) / 4);  // 10 - CF
    memcpy(rom + (((ROM_SIZE) / 4) * 3) + 6, rom, (ROM_SIZE) / 4);  // 11 - ZF+CF

    std::cout << "Flags Section: \n";

    // If JZ+ZF - JMP
    ReplaceRomInstruction(0b011000000010000, PCI|RSBO, rom);
    ReplaceRomInstruction(0b011000000011000, R, rom);
    ReplaceRomInstruction(0b011000010010000, PCO|MI, rom);
    ReplaceRomInstruction(0b011000010011000, PCI|RO|SCODE, rom);
    ReplaceRomInstruction(0b011000010100000, R, rom);
    // If JC+CF - JMP
    ReplaceRomInstruction(0b101000100010000, PCI|RSBO, rom);
    ReplaceRomInstruction(0b101000100011000, R, rom);
    ReplaceRomInstruction(0b101000110010000, PCO|MI, rom);
    ReplaceRomInstruction(0b101000110011000, PCI|RO|SCODE, rom);
    ReplaceRomInstruction(0b101000110100000, R, rom);
    // If JZ+ZF/JC+CF - JMP
    ReplaceRomInstruction(0b111000000010000, PCI|RSBO, rom);
    ReplaceRomInstruction(0b111000000011000, R, rom);
    ReplaceRomInstruction(0b111000010010000, PCO|MI, rom);
    ReplaceRomInstruction(0b111000010011000, PCI|RO|SCODE, rom);
    ReplaceRomInstruction(0b111000010100000, R, rom);

    ReplaceRomInstruction(0b111000100010000, PCI|RSBO, rom);
    ReplaceRomInstruction(0b111000100011000, R, rom);
    ReplaceRomInstruction(0b111000110010000, PCO|MI, rom);
    ReplaceRomInstruction(0b111000110011000, PCI|RO|SCODE, rom);
    ReplaceRomInstruction(0b111000110100000, R, rom);

    std::ofstream file("cpu_instr_rom.bin", std::ios::out | std::ios::binary);
    file.write(rom, (ROM_SIZE + 4) * sizeof(char));
    file.close();

    return 0;
}
