#include <iostream>
#include <vector>
#include <fstream>
#include <bitset>
#include "memory.h"

// Micro Instructions
#define R1I     ((uint64_t)1 << 0 )      // Reg 1 In
#define R1O     ((uint64_t)1 << 1 )      // Reg 1 Out
#define R2I     ((uint64_t)1 << 2 )      // Reg 2 In
#define R2O     ((uint64_t)1 << 3 )      // Reg 2 Out
#define R3I     ((uint64_t)1 << 4 )      // Reg 3 In
#define R3O     ((uint64_t)1 << 5 )      // Reg 3 Out
#define R4I     ((uint64_t)1 << 6 )      // Reg 4 In
#define R4O     ((uint64_t)1 << 7 )      // Reg 4 Out
#define SPI     ((uint64_t)1 << 8 )     // Stack Pointer In
#define SPO     ((uint64_t)1 << 9 )     // Stack Pointer Out
#define SBI     ((uint64_t)1 << 10)     // Base Pointer In
#define SBO     ((uint64_t)1 << 11)     // Base Pointer Out

#define CSI     ((uint64_t)1 << 12)     // Code Segment In
#define CSO     ((uint64_t)1 << 13)     // Code Segment Out
#define DSI     ((uint64_t)1 << 14)     // Data Segment In
#define DSO     ((uint64_t)1 << 15)     // Data Segment Out
#define SSI     ((uint64_t)1 << 16)     // Stack Segment In
#define SSO     ((uint64_t)1 << 17)     // Stack Segment Out
#define ESI     ((uint64_t)1 << 18)     // Extra Segment In
#define ESO     ((uint64_t)1 << 19)     // Extra Segment Out

#define ZO      ((uint64_t)1 << 20)     // Zero Register Out

#define II      ((uint64_t)1 << 21)      // Instruction In
#define IO      ((uint64_t)1 << 22)     // Instruction Out
#define MI      ((uint64_t)1 << 23)     // Memory In
#define MO      ((uint64_t)1 << 24)     // Memory Out
#define RI      ((uint64_t)1 << 25)     // RAM In
#define RBI     ((uint64_t)1 << 26)     // RAM Byte In
#define RO      ((uint64_t)1 << 27)     // RAM Out
#define RBO     ((uint64_t)1 << 28)     // RAM Byte Out
#define PCE     ((uint64_t)1 << 29)     // Program Counter Enable
#define PCI     ((uint64_t)1 << 30)     // Program Counter In
#define PCO     ((uint64_t)1 << 31)     // Program Counter Out

#define SO      ((uint64_t)1 << 32)     // Sum Out
#define FI      ((uint64_t)1 << 33)     // Flags In
#define S0      ((uint64_t)1 << 34)     // Sum Select 0
#define S1      ((uint64_t)1 << 35)     // Sum Select 1
#define S2      ((uint64_t)1 << 36)     // Sum Select 2
#define S3      ((uint64_t)1 << 37)     // Sum Select 3
#define AS0     ((uint64_t)1 << 38)     // A Select 0
#define AS1     ((uint64_t)1 << 39)     // A Select 1
#define AS2     ((uint64_t)1 << 40)     // A Select 2
#define AS3     ((uint64_t)1 << 41)     // A Select 3
#define BS0     ((uint64_t)1 << 42)     // B Select 0
#define BS1     ((uint64_t)1 << 43)     // B Select 1
#define BS2     ((uint64_t)1 << 44)     // B Select 2
#define BS3     ((uint64_t)1 << 45)     // B Select 3

#define SSO0    ((uint64_t)1 << 46)     // Segment Select 0
#define SSO1    ((uint64_t)1 << 47)     // Segment Select 1
#define SSO2    ((uint64_t)1 << 48)     // Segment Select 2

#define OS1I    ((uint64_t)1 << 49)     // Operand 1 Select In
#define OS1O    ((uint64_t)1 << 50)     // Operand 1 Select Out
#define OS1A    ((uint64_t)1 << 51)     // Operand 1 Select ASum

#define OS2I    ((uint64_t)1 << 52)     // Operand 2 Select In
#define OS2O    ((uint64_t)1 << 53)     // Operand 2 Select Out
#define OS2B    ((uint64_t)1 << 54)    // Operand 2 Select BSum

#define OSLI    ((uint64_t)1 << 55)     // Operand Select In
#define OSLO    ((uint64_t)1 << 56)     // Operand Select Out
#define OSL0    ((uint64_t)1 << 57)     // Operand Job Select 0
#define OSL1    ((uint64_t)1 << 58)    // Operand Job Select 1

#define R       ((uint64_t)1 << 63)     // Intruction End

// NOP
#define NOP PCO|MI, SCODE|PCE|RO|II

// Alu

// ALU Operations
#define ADD  SO          // S000 - add
#define SUB  SO|S0       // S001 - sub
#define INC  SO|S1       // S010 - inc
#define DEC  SO|S1|S0    // S011 - dec
#define MULT SO|S2       // S100 - mult
#define DIV  SO|S2|S0    // S101 - div
#define AND  SO|S2|S1    // S110 - and
#define OR   SO|S2|S1|S0 // S111 - or

// ALU A Selects
#define ASR1             AS0
#define ASR2         AS1
#define ASR3         AS1|AS0
#define ASR4     AS2

#define ASSP     AS2    |AS0
#define ASBP     AS2|AS1

#define ASCS     AS2|AS1|AS0
#define ASDS AS3
#define ASSS AS3        |AS0
#define ASES AS3    |AS1

#define ASZ  AS3    |AS1|AS0
#define ASM  AS3|AS2
#define ASPC AS3|AS2|    AS0

// ALU B Selects
#define BSR1             BS0
#define BSR2         BS1
#define BSR3         BS1|BS0
#define BSR4     BS2

#define BSSP     BS2    |BS0
#define BSBP     BS2|BS1

#define BSCS     BS2|BS1|BS0
#define BSDS BS3
#define BSSS BS3        |BS0
#define BSES BS3    |BS1

#define BSZ  BS3    |BS1|BS0
#define BSM  BS3|BS2
#define BSPC BS3|BS2|    BS0

// Segments
// SSO000 - none
#define SCODE  SSO0         // SSO001 - code
#define SDATA  SSO1         // SSO010 - data
#define SSTACK SSO1|SSO0    // SSO011 - stack
#define SEXTRA SSO2         // SSO101 - extra

// Select Operand
#define SSELI OSLI|     OSL0      // Segment Select In
#define SSELO OSLO|     OSL0      // Segment Select Out
#define ESELI OSLI|OSL1           // Ext Port Select In
#define ESELO OSLO|OSL1           // Ext Port Select Out
#define RSELI OSLI|OSL1|OSL0      // Register Select In
#define RSELO OSLO|OSL1|OSL0      // Register Select Out

// Instructions
std::vector<std::vector<uint64_t>> instructions = {
    { NOP, R },                                                                 // nop

    // Moving
    { NOP, OS1I|OS2O, R },                                                      // mov
    { NOP, PCO|MI, OS1I|RO|SCODE|PCE, R },                                      // li

    { NOP, PCO|MI, MI|SCODE|RO|PCE, MI|ADD|ASM|OS2B, OS1I|RO|SSELO, R },        // ldw
    { NOP, PCO|MI, MI|SCODE|RO|PCE, MI|ADD|ASM|OS2B, OS1I|RBO|SSELO, R },       // ldb
    { NOP, PCO|MI, MI|SCODE|RO|PCE, MI|ADD|ASM|OS2B, OS1O|RI|SSELO, R },        // stw
    { NOP, PCO|MI, MI|SCODE|RO|PCE, MI|ADD|ASM|OS2B, OS1O|RBI|SSELO, R },       // stb

    { NOP, PCO|MI, MI|SCODE|RO|PCE, MI|ADD|ASM|OS2B, OS1I|ESELO|SEXTRA, R },    // lde
    { NOP, PCO|MI, MI|SCODE|RO|PCE, MI|ADD|ASM|OS2B, OS1O|ESELI|SEXTRA, R },    // ste
    
    // Jump / Conditions
    { NOP, PCO|MI, PCI|RO|SCODE, PCI|ADD|ASPC|OS2B, R },                        // jmp
    { NOP, PCO|MI, PCI|RO|SCODE, PCI|ADD|ASPC|OS2B, OS1O|CSI, R },              // jmpf
    { NOP, SUB|OS1A|OS2B|FI, R },                                               // cmp
    { NOP, PCE, R },                                                            // jz
    { NOP, PCE, R },                                                            // jc
     
    // Arithmetic
    { NOP, RSELI|ADD|OS1A|OS2B|FI, R },                                         // add
    { NOP, RSELI|SUB|OS1A|OS2B|FI, R },                                         // sub
    { NOP, RSELI|INC|OS1A|FI, R },                                              // inc
    { NOP, RSELI|DEC|OS1A|FI, R },                                              // dec
    { NOP, RSELI|MULT|OS1A|OS2B|FI, R },                                        // mult
    { NOP, RSELI|DIV|OS1A|OS2B|FI, R },                                         // div

    // Bitwise
    { NOP, RSELI|AND|OS1A|OS2B|FI, R },                                         // and
    { NOP, RSELI|OR|OS1A|OS2B|FI, R },                                          // or

    // Stack / Functions
    { NOP, MI|SPO, RI|OS1O|SSTACK, SPI|DEC|ASSP, SPI|DEC|ASSP, R },             // push
    { NOP, SPI|INC|ASSP, MI|SPI|INC|ASSP, OS1I|RO|SSTACK, R },                  // pop

    { NOP, MI|SPO|PCE, RI|PCO|SSTACK, SPI|DEC|ASSP, SPI|DEC|ASSP, MI|DEC|ASPC, MI|DEC|ASM, PCI|RO|SCODE, PCI|ADD|ASPC|OS2B, R }, // call
    { NOP, MI|SPO|PCE, RI|PCO|SSTACK, SPI|DEC|ASSP, SPI|MI|DEC|ASSP, RI|CSO|SSTACK, SPI|DEC|ASSP, SPI|DEC|ASSP, MI|DEC|ASPC, MI|DEC|ASM, PCI|RO|SCODE, PCI|ADD|ASPC|OS2B, CSI|OS1O, R }, // callf

    { NOP, SPI|INC|ASSP, SPI|MI|INC|ASSP, PCI|RO|SSTACK, R },                                               // ret
    { NOP, SPI|INC|ASSP, SPI|MI|INC|ASSP, CSI|RO|SSTACK, SPI|INC|ASSP, SPI|MI|INC|ASSP, PCI|RO|SSTACK, R }, // retf
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

    for (int i = 0; i < ROM_SIZE; i++) {
        rom[i] = 0;
    }

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
    ReplaceRomInstruction(0b010011000010000, PCO|MI, rom);
    ReplaceRomInstruction(0b010011000011000, PCI|RO|SCODE, rom);
    ReplaceRomInstruction(0b010011000100000, PCI|ADD|ASPC|OS2B, rom);
    ReplaceRomInstruction(0b010011000101000, R, rom);
    // If JC+CF - JMP
    ReplaceRomInstruction(0b100011010010000, PCO|MI, rom);
    ReplaceRomInstruction(0b100011010011000, PCI|RO|SCODE, rom);
    ReplaceRomInstruction(0b100011010100000, PCI|ADD|ASPC|OS2B, rom);
    ReplaceRomInstruction(0b100011010101000, R, rom);
    // If JZ+ZF/JC+CF - JMP
    ReplaceRomInstruction(0b110011000010000, PCO|MI, rom);
    ReplaceRomInstruction(0b110011000011000, PCI|RO|SCODE, rom);
    ReplaceRomInstruction(0b110011000100000, PCI|ADD|ASPC|OS2B, rom);
    ReplaceRomInstruction(0b110011000101000, R, rom);

    ReplaceRomInstruction(0b110011010010000, PCO|MI, rom);
    ReplaceRomInstruction(0b110011010011000, PCI|RO|SCODE, rom);
    ReplaceRomInstruction(0b110011010100000, PCI|ADD|ASPC|OS2B, rom);
    ReplaceRomInstruction(0b110011010101000, R, rom);

    std::ofstream file("../../bin/cpu_instr_gen/cpu_instr_rom.bin", std::ios::out | std::ios::binary);
    file.write(rom, (ROM_SIZE + 4) * sizeof(char));
    file.close();

    return 0;
}
