# Computer Design

 - 16 Bit Bus
 - 24 Bit Address Bus (Segment 0xffff00) + (Address 0x00ffff)
 - 4 General Registers
 - 16 Expansion Modules
 - Big Endian

### ISA (Instruction Set Architecture)

| Index | Instr (6b) | op0 (2b) | op1 (4b)  | op2 (4b)   | imm (16b) | Notes                                                                                |
| :---: | :--------- | :------- | :-------- | :--------- | :-------- | ------------------------------------------------------------------------------------ |
|   0   | nop        |          |           |            |           | No Operation                                                                         |
|   1   | mov        |          | dest      | src        |           | Copy Register dest <- src                                                            |
|   2   | li         |          | dest      |            | value     | Load dest <- imm                                                                     |
|  ---  |            |          |           |            |           |                                                                                      |
|   3   | ldw        | seg      |           | [src-addr, | offset]   | Load Word from Memory                                                                |
|   4   | ldb        | seg      |           | [src-addr, | offset]   | Load Byte from Memory                                                                |
|   5   | stw        | seg      |           | [src-addr, | offset]   | Store Word to Memory                                                                 |
|   6   | stb        | seg      |           | [src-addr, | offset]   | Store Byte to Memory                                                                 |
|   7   | lde        | dest     | exp       | [src-addr, | offset]   | Load Word from Expansion Unit                                                        |
|   8   | ldeb       | dest     | exp       | [src-addr, | offset]   | Load Byte from Expansion Unit                                                        |
|   9   | ste        | src      | exp       | [src-addr, | offset]   | Store Word from Expansion Unit                                                       |
|  10   | steb       | src      | exp       | [src-addr, | offset]   | Store Word from Expansion Unit                                                       |
|  ---  |            |          |           |            |           |                                                                                      |
|  11   | jmp        |          |           | [src-addr, | offset]   | Jump to Address                                                                      |
|  12   | jmpf       |          | src-seg   | [src-addr, | offset]   | Jump to Address & Set Code Segment                                                   |
|  13   | bz         |          |           | [src-addr, | offset]   | Branch to Address if Zero Flag                                                       |
|  14   | bnz        |          |           | [src-addr, | offset]   | Branch to Address if Not Zero Flag                                                   |
|  15   | bc         |          |           | [src-addr, | offset]   | Branch to Address if Carry Flag                                                      |
|  16   | bnc        |          |           | [src-addr, | offset]   | Branch to Address if Not Carry Flag                                                  |
|  17   | bs         |          |           | [src-addr, | offset]   | Branch to Address if Sign Flag                                                       |
|  18   | bns        |          |           | [src-addr, | offset]   | Branch to Address if Not Sign Flag                                                   |
|  19   | bo         |          |           | [src-addr, | offset]   | Branch to Address if Overflow Flag                                                   |
|  20   | bno        |          |           | [src-addr, | offset]   | Branch to Address if Not Overflow Flag                                               |
|  ---  |            |          |           |            |           |                                                                                      |
|  21   | add        | res      | src-alu-a | src-alu-b  |           | Add & Set Flags                                                                      |
|  22   | sub        | res      | src-alu-a | src-alu-b  |           | Subtract & Set Flags                                                                 |
|  23   | inc        | res      | src-alu-a |            |           | Increment by 1 & Set Flags                                                           |
|  24   | inc2       | res      | src-alu-a |            |           | Incrementby 2 & Set Flags                                                            |
|  25   | dec        | res      | src-alu-a |            |           | Decrement by 1 & Set Flags                                                           |
|  26   | dec2       | res      | src-alu-a |            |           | Decrement by 2 & Set Flags                                                           |
|  27   | not        | res      | src-alu-a |            |           | Logical NOT & Set Flags                                                              |
|  28   | and        | res      | src-alu-a | src-alu-b  |           | Logical AND & Set Flags                                                              |
|  29   | or         | res      | src-alu-a | src-alu-b  |           | Logical OR & Set Flags                                                               |
|  30   | xor        | res      | src-alu-a | src-alu-b  |           | Logical XOR & Set Flags                                                              |
|  31   | sll        | res      | src-alu-a | src-alu-b  |           | Logical Shift Left by src-alu-b & Set Flags                                          |
|  32   | srl        | res      | src-alu-a | src-alu-b  |           | Logical Shift Right by src-alu-b & Set Flags                                         |
|  33   | sra        | res      | src-alu-a | src-alu-b  |           | Arithmetic Shift Right by src-alu-b & Set Flags                                      |
|  34   | neg        | res      | src-alu-a |            |           | Negate & Set Flags                                                                   |
|  35   | cmp        | res      | src-alu-a | src-alu-b  |           | Compare by Subtraction & Set Flags                                                   |
|  ---  |            |          |           |            |           |                                                                                      |
|  36   | push       |          | src       |            |           | Push to Stack                                                                        |
|  37   | pushb      |          | src       |            |           | Push Byte to Stack                                                                   |
|  38   | pop        |          | dest      |            |           | Pop from Stack                                                                       |
|  39   | popb       |          | dest      |            |           | Pop Byte from Stack                                                                  |
|  40   | call       |          |           | [src-addr, | offset]   | Push Program Counter to Stack & Jump to Address                                      |
|  41   | callf      |          | src-seg   | [src-addr, | offset]   | Push Code Segment then Program Counter to Stack & Jump to Address & Set Code Segment |
|  42   | ret        |          |           |            |           | Pop Program Counter                                                                  |
|  43   | retf       |          |           |            |           | Pop Code Segment then Program Counter                                                |

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
