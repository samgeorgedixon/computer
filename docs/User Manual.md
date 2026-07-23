
---
## Overview

This is a proof of concept 16b CPU with an experimental architecture. This document describes how a user can interact with this architecture.

### Key Characteristics

| Property          | Value  | Description                                                        |
| ----------------- | ------ | ------------------------------------------------------------------ |
| Word Size (Bus)   | 16b    |                                                                    |
| Address Size      | 24b    | Segmented: (Segment of 16b: 0xffff00) + (Address of 16b: 0x00ffff) |
| Endianness        | Big    |                                                                    |
| General Registers | 4      |                                                                    |
| Expansion Units   | 16     | Interrupt Based with lde/ste to Communicate                        |
| Instruction Count | 6b: 64 |                                                                    |

---

<div style="page-break-before: always;"></div>

## Contents
---



---

<div style="page-break-before: always;"></div>

## Instruction Set Architecture (ISA)
---

| Index | Instr (6b) | op0 (2b) | op1 (4b)  | op2 (4b)   | imm (16b) | Notes                                                                                | Clock Cycles (Fetch = 2) |
| :---: | :--------- | :------- | :-------- | :--------- | :-------- | ------------------------------------------------------------------------------------ | ------------------------ |
|   0   | nop        |          |           |            |           | No Operation                                                                         | 3                        |
|   1   | mov        |          | dest      | src        |           | Copy Register: dest <- src                                                           | 3                        |
|   2   | mov        |          | dest      |            | value     | Load Immediate: dest <- imm                                                          | 4                        |
|   3   | lea        |          | dest      | [src-addr, | offset]   | Load Effective Address: dest <-src-addr + offset                                     | 5                        |
|  ---  |            |          |           |            |           |                                                                                      |                          |
|   4   | ldw        | seg      | dest      | [src-addr, | offset]   | Load Word from Memory at src-addr + offset                                           | 6                        |
|   5   | ldw        | seg      | dest      | src-addr   |           | Load Word from Memory at src-addr                                                    | 4                        |
|   6   | ldw+       | seg      | dest      | src-addr   |           | Load Word from Memory at src-addr & Post Increment src-addr by 2                     | 5                        |
|   7   | ldb        | seg      | dest      | [src-addr, | offset]   | Load Byte from Memory at src-addr + offset                                           | 6                        |
|   8   | ldb        | seg      | dest      | src-addr   |           | Load Byte from Memory at src-addr                                                    | 4                        |
|   9   | ldb+       | seg      | dest      | src-addr   |           | Load Byte from Memory at src-addr & Post Increment src-addr by 1                     | 5                        |
|  10   | stw        | seg      | src       | [src-addr, | offset]   | Store Word to Memory at src-addr + offset                                            | 6                        |
|  11   | stw        | seg      | src       | src-addr   |           | Store Word to Memory at src-addr                                                     | 4                        |
|  12   | stw+       | seg      | src       | src-addr   |           | Store Word to Memory at src-addr & Post Increment src-addr by 2                      | 5                        |
|  13   | stb        | seg      | src       | [src-addr, | offset]   | Store Byte to Memory at src-addr + offset                                            | 6                        |
|  14   | stb        | seg      | src       | src-addr   |           | Store Byte to Memory at src-addr                                                     | 4                        |
|  15   | stb+       | seg      | src       | src-addr   |           | Store Byte to Memory at src-addr & Post Increment src-addr by 1                      | 5                        |
|  16   | ldxw       | dest     | xu        | [src-addr, | offset]   | Load Word from Expansion Unit at src-addr + offset                                   | 6                        |
|  17   | ldxw       | dest     | xu        | src-addr   |           | Load Word from Expansion Unit at src-addr                                            | 4                        |
|  18   | ldxw+      | dest     | xu        | src-addr   |           | Load Word from Expansion Unit at src-addr & Post Increment src-addr by 2             | 5                        |
|  19   | ldxb       | dest     | xu        | [src-addr, | offset]   | Load Byte from Expansion Unit at src-addr + offset                                   | 6                        |
|  20   | ldxb       | dest     | xu        | src-addr   |           | Load Byte from Expansion Unit at src-addr                                            | 4                        |
|  21   | ldxb+      | dest     | xu        | src-addr   |           | Load Byte from Expansion Unit at src-addr & Post Increment src-addr by 1             | 5                        |
|  22   | stxw       | src      | xu        | [src-addr, | offset]   | Store Word to Expansion Unit at src-addr + offset                                    | 6                        |
|  23   | stxw       | src      | xu        | src-addr   |           | Store Word to Expansion Unit at src-addr                                             | 4                        |
|  24   | stxw+      | src      | xu        | src-addr   |           | Store Word to Expansion Unit at src-addr & Post Increment src-addr by 2              | 5                        |
|  25   | stxb       | src      | xu        | [src-addr, | offset]   | Store Byte to Expansion Unit at src-addr + offset                                    | 6                        |
|  26   | stxb       | src      | xu        | src-addr   |           | Store Byte to Expansion Unit at src-addr                                             | 4                        |
|  27   | stxb+      | src      | xu        | src-addr   |           | Store Byte to Expansion Unit at src-addr & Post Increment src-addr by 1              | 5                        |
|  ---  |            |          |           |            |           |                                                                                      |                          |
|  28   | jmp        |          |           | [src-addr, | offset]   | Jump to Address                                                                      | 5                        |
|  29   | jmpf       |          | src-seg   | [src-addr, | offset]   | Jump to Address & Set Code Segment                                                   | 6                        |
|  30   | b          |          | flag      | [src-addr, | offset]   | Branch to Address if Flag (zero z, carry c, sign s, overflow o)                      | 5 ( / 3)                 |
|  31   | bn         |          | flag      | [src-addr, | offset]   | Branch to Address if Not Flag (zero z, carry c, sign s, overflow o)                  | 5 ( / 3)                 |
|  ---  |            |          |           |            |           |                                                                                      |                          |
|  32   | add        | res      | src-alu-a | src-alu-b  |           | Add & Set Flags                                                                      | 3                        |
|  33   | sub        | res      | src-alu-a | src-alu-b  |           | Subtract & Set Flags                                                                 | 3                        |
|  34   | inc        | res      | src-alu-a |            |           | Increment by 1 & Set Flags                                                           | 3                        |
|  35   | inc2       | res      | src-alu-a |            |           | Incrementby 2 & Set Flags                                                            | 3                        |
|  36   | dec        | res      | src-alu-a |            |           | Decrement by 1 & Set Flags                                                           | 3                        |
|  37   | dec2       | res      | src-alu-a |            |           | Decrement by 2 & Set Flags                                                           | 3                        |
|  38   | not        | res      | src-alu-a |            |           | Logical NOT & Set Flags                                                              | 3                        |
|  39   | and        | res      | src-alu-a | src-alu-b  |           | Logical AND & Set Flags                                                              | 3                        |
|  40   | or         | res      | src-alu-a | src-alu-b  |           | Logical OR & Set Flags                                                               | 3                        |
|  41   | xor        | res      | src-alu-a | src-alu-b  |           | Logical XOR & Set Flags                                                              | 3                        |
|  42   | sll        | res      | src-alu-a | src-alu-b  |           | Logical Shift Left by src-alu-b & Set Flags                                          | 3                        |
|  43   | srl        | res      | src-alu-a | src-alu-b  |           | Logical Shift Right by src-alu-b & Set Flags                                         | 3                        |
|  44   | sra        | res      | src-alu-a | src-alu-b  |           | Arithmetic Shift Right by src-alu-b & Set Flags                                      | 3                        |
|  45   | neg        | res      | src-alu-a |            |           | Negate & Set Flags                                                                   | 3                        |
|  46   | cmp        |          | src-alu-a | src-alu-b  |           | Compare by Subtraction & Set Flags                                                   | 3                        |
|  ---  |            |          |           |            |           |                                                                                      |                          |
|  47   | push       |          | src       |            |           | Push to Stack                                                                        | 4                        |
|  48   | pushb      |          | src       |            |           | Push Byte to Stack                                                                   | 4                        |
|  49   | pop        |          | dest      |            |           | Pop from Stack                                                                       | 5                        |
|  50   | popb       |          | dest      |            |           | Pop Byte from Stack                                                                  | 5                        |
|  51   | call       |          |           | [src-addr, | offset]   | Push Program Counter to Stack & Jump to Address                                      | 7                        |
|  52   | callf      |          | src-seg   | [src-addr, | offset]   | Push Code Segment then Program Counter to Stack & Jump to Address & Set Code Segment | 10                       |
|  53   | ret        |          |           |            |           | Pop Program Counter                                                                  | 5                        |
|  54   | retf       |          |           |            |           | Pop Code Segment then Program Counter                                                | 8                        |
|  ---  |            |          |           |            |           |                                                                                      |                          |
|  55   |            |          |           |            |           |                                                                                      |                          |
|  56   |            |          |           |            |           |                                                                                      |                          |
|  57   |            |          |           |            |           |                                                                                      |                          |
|  58   |            |          |           |            |           |                                                                                      |                          |
|  59   |            |          |           |            |           |                                                                                      |                          |
|  60   |            |          |           |            |           |                                                                                      |                          |
|  61   | int        |          |           |            | index     |                                                                                      |                          |
|  62   | rint       |          |           |            |           |                                                                                      |                          |
|  63   |            |          |           |            |           |                                                                                      |                          |

---

<div style="page-break-before: always;"></div>

## Registers / Expansion Units
---

Registers are indexed from 0-15 with a separate flags register used by the CPU and ALU but not indexable. Then expansion units take up the rest of the registers indexes from 16-31 allowing a total of 16 expansion units to be used.

However expansion units do not act like registers (with mov or movi...) and instead act like a part of memory where you use lde or ste which allows you to take advantage of the 24b address as well as the 16b bus to communicate with the expansion units. In fact this has meant RAM can act as expansion unit 1 (xu1) and means drives can be accesses in the same simple way as RAM.

| Index   | ID         | Width             | Role                                             |
| ------- | ---------- | ----------------- | ------------------------------------------------ |
| 0       | N/A        | 16b               | Nothing                                          |
| 1       | r1         | 16b               | 1st General Purpose                              |
| 2       | r2         | 16b               | 2nd General Purpose                              |
| 3       | r3         | 16b               | 3rd General Purpose                              |
| 4       | r4         | 16b               | 4th General Purpose                              |
| 5       | sp         | 16b               | Stack Pointer                                    |
| 6       | bp         | 16b               | Base Pointer                                     |
| 7       | cs         | 16b               | Code Segment Address                             |
| 8       | ds         | 16b               | Data Segment Address                             |
| 9       | ss         | 16b               | Stack Segment Address                            |
| 10      | es         | 16b               | Extra Segment Address (+ Expansion Unit Address) |
| 11      | z          | 16b               | Zero                                             |
| 12      | ip         | 16b               | Instruction Pointer                              |
| 13      | ir         | 16b               | Current Instruction                              |
| 14      | ar         | 16b               | Current Address                                  |
| 15      |            |                   |                                                  |
|         | f          | 8b                | Flags (Not Indexable)                            |
| 16 - 31 | xu(1...16) | 16b bus, 24b addr | Expansion Unit Indexes (lde / ste)               |
#### Example Expansion Units

| ID  | Expansion Unit         | Type                |
| --- | ---------------------- | ------------------- |
| xu1 | Memory (ROM / RAM)     | Internal            |
| xu2 | Drive                  | Internal            |
| xu3 | GPU                    | Internal / External |
| xu4 | USB (Keyboard / Mouse) | External            |
| xu5 | Ethernet               | External            |
