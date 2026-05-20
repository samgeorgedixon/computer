# Computer Design

 - 16 Bit Bus
 - 24 Bit Address Bus (Segment 0xffff00) + (Address 0x00ffff)
 - 4 General Registers
 - 16 Expansion Modules
 - Big Endian

### ISA (Instruction Set Architecture)

| Index | Instr (6b) | op0 (2b) | op1 (4b)  | op2 (4b)   | imm (16b) | Notes                                                                                | Clock Cycles (Fetch = 2) |
| :---: | :--------- | :------- | :-------- | :--------- | :-------- | ------------------------------------------------------------------------------------ | ------------------------ |
|   0   | nop        |          |           |            |           | No Operation                                                                         | 3                        |
|   1   | mov        |          | dest      | src        |           | Copy Register: dest <- src                                                           | 3                        |
|   2   | mov        |          | dest-1    | {dest-2}   | value     | Load Immediate: dest-1, {optional dest-2} <- imm                                     | 4                        |
|   3   | lea        |          | dest      | [src-addr, | offset]   | Load Effective Address: dest <-src-addr + offset                                     | 5                        |
|  ---  |            |          |           |            |           |                                                                                      |                          |
|   4   | ldw        | seg      | dest      | [src-addr, | offset]   | Load Word from Memory at src-addr + offset                                           | 6                        |
|   5   | ldw        | seg      | dest      | src-addr   |           | Load Word from Memory at src-addr                                                    | 4                        |
|       | ldw+       | seg      | dest      | src-addr   |           | Load Word from Memory at src-addr & Post Increment src-addr by 2                     |                          |
|   6   | ldb        | seg      | dest      | [src-addr, | offset]   | Load Byte from Memory at src-addr + offset                                           | 6                        |
|   7   | ldb        | seg      | dest      | src-addr   |           | Load Byte from Memory at src-addr                                                    |                          |
|       | ldb+       | seg      | dest      | src-addr   |           | Load Byte from Memory at src-addr & Post Increment src-addr by 1                     |                          |
|   8   | stw        | seg      | src       | [src-addr, | offset]   | Store Word to Memory at src-addr + offset                                            | 6                        |
|   9   | stw        | seg      | src       | src-addr   |           | Store Word to Memory at src-addr                                                     |                          |
|       | stw+       | seg      | src       | src-addr   |           | Store Word to Memory at src-addr & Post Increment src-addr by 2                      |                          |
|  10   | stb        | seg      | src       | [src-addr, | offset]   | Store Byte to Memory at src-addr + offset                                            | 6                        |
|  11   | stb        | seg      | src       | src-addr   |           | Store Byte to Memory at src-addr                                                     |                          |
|       | stb+       | seg      | src       | src-addr   |           | Store Byte to Memory at src-addr & Post Increment src-addr by 1                      |                          |
|  12   | ldew       | dest     | exp       | [src-addr, | offset]   | Load Word from Expansion Unit at src-addr + offset                                   | 6                        |
|  13   | ldew       | dest     | exp       | src-addr   |           | Load Word from Expansion Unit at src-addr                                            |                          |
|       | ldew+      | dest     | exp       | src-addr   |           | Load Word from Expansion Unit at src-addr & Post Increment src-addr by 2             |                          |
|  14   | ldeb       | dest     | exp       | [src-addr, | offset]   | Load Byte from Expansion Unit at src-addr + offset                                   | 6                        |
|  15   | ldeb       | dest     | exp       | src-addr   |           | Load Byte from Expansion Unit at src-addr                                            |                          |
|       | ldeb+      | dest     | exp       | src-addr   |           | Load Byte from Expansion Unit at src-addr & Post Increment src-addr by 1             |                          |
|  16   | stew       | src      | exp       | [src-addr, | offset]   | Store Word to Expansion Unit at src-addr + offset                                    | 6                        |
|  17   | stew       | src      | exp       | src-addr   |           | Store Word to Expansion Unit at src-addr                                             |                          |
|       | stew+      | src      | exp       | src-addr   |           | Store Word to Expansion Unit at src-addr & Post Increment src-addr by 2              |                          |
|  18   | steb       | src      | exp       | [src-addr, | offset]   | Store Byte to Expansion Unit at src-addr + offset                                    | 6                        |
|       | steb       | src      | exp       | src-addr   |           | Store Byte to Expansion Unit at src-addr                                             |                          |
|       | steb+      | src      | exp       | src-addr   |           | Store Byte to Expansion Unit at src-addr & Post Increment src-addr by 1              |                          |
|  ---  |            |          |           |            |           |                                                                                      |                          |
|  11   | jmp        |          |           | [src-addr, | offset]   | Jump to Address                                                                      | 5                        |
|  12   | jmpf       |          | src-seg   | [src-addr, | offset]   | Jump to Address & Set Code Segment                                                   | 6                        |
|  13   | bz         |          |           | [src-addr, | offset]   | Branch to Address if Zero Flag                                                       | 5 ( / 3)                 |
|  14   | bnz        |          |           | [src-addr, | offset]   | Branch to Address if Not Zero Flag                                                   | 5 ( / 3)                 |
|  15   | bc         |          |           | [src-addr, | offset]   | Branch to Address if Carry Flag                                                      | 5 ( / 3)                 |
|  16   | bnc        |          |           | [src-addr, | offset]   | Branch to Address if Not Carry Flag                                                  | 5 ( / 3)                 |
|  17   | bs         |          |           | [src-addr, | offset]   | Branch to Address if Sign Flag                                                       | 5 ( / 3)                 |
|  18   | bns        |          |           | [src-addr, | offset]   | Branch to Address if Not Sign Flag                                                   | 5 ( / 3)                 |
|  19   | bo         |          |           | [src-addr, | offset]   | Branch to Address if Overflow Flag                                                   | 5 ( / 3)                 |
|  20   | bno        |          |           | [src-addr, | offset]   | Branch to Address if Not Overflow Flag                                               | 5 ( / 3)                 |
|  ---  |            |          |           |            |           |                                                                                      |                          |
|  21   | add        | res      | src-alu-a | src-alu-b  |           | Add & Set Flags                                                                      | 3                        |
|  22   | sub        | res      | src-alu-a | src-alu-b  |           | Subtract & Set Flags                                                                 | 3                        |
|  23   | inc        | res      | src-alu-a |            |           | Increment by 1 & Set Flags                                                           | 3                        |
|  24   | inc2       | res      | src-alu-a |            |           | Incrementby 2 & Set Flags                                                            | 3                        |
|  25   | dec        | res      | src-alu-a |            |           | Decrement by 1 & Set Flags                                                           | 3                        |
|  26   | dec2       | res      | src-alu-a |            |           | Decrement by 2 & Set Flags                                                           | 3                        |
|  27   | not        | res      | src-alu-a |            |           | Logical NOT & Set Flags                                                              | 3                        |
|  28   | and        | res      | src-alu-a | src-alu-b  |           | Logical AND & Set Flags                                                              | 3                        |
|  29   | or         | res      | src-alu-a | src-alu-b  |           | Logical OR & Set Flags                                                               | 3                        |
|  30   | xor        | res      | src-alu-a | src-alu-b  |           | Logical XOR & Set Flags                                                              | 3                        |
|  31   | sll        | res      | src-alu-a | src-alu-b  |           | Logical Shift Left by src-alu-b & Set Flags                                          | 3                        |
|  32   | srl        | res      | src-alu-a | src-alu-b  |           | Logical Shift Right by src-alu-b & Set Flags                                         | 3                        |
|  33   | sra        | res      | src-alu-a | src-alu-b  |           | Arithmetic Shift Right by src-alu-b & Set Flags                                      | 3                        |
|  34   | neg        | res      | src-alu-a |            |           | Negate & Set Flags                                                                   | 3                        |
|  35   | cmp        | res      | src-alu-a | src-alu-b  |           | Compare by Subtraction & Set Flags                                                   | 3                        |
|  ---  |            |          |           |            |           |                                                                                      |                          |
|  36   | push       |          | src       |            |           | Push to Stack                                                                        | 4                        |
|  37   | pushb      |          | src       |            |           | Push Byte to Stack                                                                   | 4                        |
|  38   | pop        |          | dest      |            |           | Pop from Stack                                                                       | 5                        |
|  39   | popb       |          | dest      |            |           | Pop Byte from Stack                                                                  | 5                        |
|  40   | call       |          |           | [src-addr, | offset]   | Push Program Counter to Stack & Jump to Address                                      | 7                        |
|  41   | callf      |          | src-seg   | [src-addr, | offset]   | Push Code Segment then Program Counter to Stack & Jump to Address & Set Code Segment | 10                       |
|  42   | ret        |          |           |            |           | Pop Program Counter                                                                  | 5                        |
|  43   | retf       |          |           |            |           | Pop Code Segment then Program Counter                                                | 8                        |

#### Registers (13 + zero, f)

0. n/a
1.  r1
2.  r2
3.  r3
4.  r4
5. pc
6. mem
7. instr
8.  sp
9. bp
10. cs - Segments (code, data, stack, extra)
11. ds
12. ss
13. es
14. zero (not register - hardwire)
-  f (not selectable)

#### Expansion Units

- Interconnects
	- clk, r, byte_low
	- we, oe
	- bus - 16b
	- addr - 24b

1. Memory - ROM / RAM (Internal)
2. Drive (Internal)
3. GPU (Internal?)
4. USB - Keyboard / Mouse
5. Ethernet?

#### ALU

 -  Direct BUS Output
 -  Operation Select - 4b
 -  A Select - 4b
 -  B Select - 4b

### Control Unit (CU) - Control Signals - 35b (CPLD / EEPROM)

 - bus-src - 5b (16 registers / 16 expansions, other)
 - bus-dest - 5b (16 registers / 16 expansions, other)
 - bus-dest-special - 4b (16 register groups)

 - seg_sel__alu_op_sel_raw - 4b (__ or - (0)-(3): Segment Select, 0-15: ALU OP Select)
 - alu_e_raw - 1b (0: Segment On / ALU Off, 1: Segment On / ALU On)

 - alu-a-sel - 4b (16 registers)
 - alu-b-sel - 4b (16 registers)

- pc-e - 1b (Program Counter Enable)
- flags-e - 1b (Enable Flags)
- byte_ - 1b (Enable Flags)

- operand-0 - 2b (00: Off, 01: seg_sel, 10: bus_dest, 11: bus_src)
- operand-1 - 2b (00: Off, 01: bus_dest, 10: bus_src, 01&alu_e_raw: alu-a-sel)
- operand-2 - 1b (0: Off, 1&alu_e_raw: alu-b-sel, 1: bus_src)

 -  (opcode, 6b)    (operand-0, 2b)   (operand-1, 4b)    (operand-2, 4b)    (imm, 16b)
    ldw                  {seg}                     rs1                           [rs2                          {imm}]
	.                       1 + 3o                  1 + 2o                     1 + 1o - src              1 + 1o - bus : ( + alu_e)

- instr-end - 1b
