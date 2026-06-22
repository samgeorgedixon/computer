
---
## Overview

This is a proof of concept 16b CPU with an experimental architecture. This document describes this architecture and how to work on it.

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

## Units
---

Making up this CPU are a variety of units doing specific tasks and all connected to the 16 bus and some the 24b address bus.

| Unit                   | ID  | CXN 1           | CNX 2     | CNX 3 | CNX 4        | CNX 5        | CNX 6     | Description |
| ---------------------- | --- | --------------- | --------- | ----- | ------------ | ------------ | --------- | ----------- |
| Control Unit           | cu  | control_signals | clk       | r     | instr_direct | flags_direct |           |             |
| Arithmetic Logic Unit  | alu | control_signals | bus_flags | r     | bus          | bus_alu_a    | bus_alu_b |             |
| Registers Unit         | ru  | control_signals | clk       | r     | bus          | bus_alu_a    | bus_alu_b |             |
| Program Counter        | pc  |                 |           |       |              |              |           |             |
| Address Manager        |     |                 |           |       |              |              |           |             |
| Expansion Unit Manager | eum |                 |           |       |              |              |           |             |


---

<div style="page-break-before: always;"></div>

## Registers / Expansion Units
---

Registers are indexed from 0-15 with a separate flags register used by the CPU and ALU but not indexable. Then expansion units take up the rest of the registers indexes from 16-31 allowing a total of 16 expansion units to be used.

However expansion units do not act like registers (with mov or movi...) and instead act like a part of memory where you use lde or ste which allows you to take advantage of the 24b address as well as the 16b bus to communicate with the expansion units. In fact this has meant RAM can act as expansion unit 1 (exp1) and means drives can be accesses in the same simple way as RAM.

| Index   | ID          | Width             | Role                                             |
| ------- | ----------- | ----------------- | ------------------------------------------------ |
| 0       | N/A         | 16b               | Nothing                                          |
| 1       | r1          | 16b               | 1st General Purpose                              |
| 2       | r2          | 16b               | 2nd General Purpose                              |
| 3       | r3          | 16b               | 3rd General Purpose                              |
| 4       | r4          | 16b               | 4th General Purpose                              |
| 5       | pc          | 16b               | Program Counter                                  |
| 6       | mem         | 16b               | Current Memory Address                           |
| 7       | instr       | 16b               | Current Instruction                              |
| 8       | sp          | 16b               | Stack Pointer                                    |
| 9       | bp          | 16b               | Base Pointer                                     |
| 10      | cs          | 16b               | Code Segment Address                             |
| 1       | ds          | 16b               | Data Segment Address                             |
| 12      | ss          | 16b               | Stack Segment Address                            |
| 13      | es          | 16b               | Extra Segment Address (+ Expansion Unit Address) |
| 14      | z           | 16b               | Zero                                             |
| 15      |             |                   |                                                  |
|         | f           | 8b                | Flags (Not Indexable)                            |
| 16 - 31 | exp(1...16) | 16b bus, 24b addr | Expansion Unit Indexes (lde / ste)               |

---

<div style="page-break-before: always;"></div>

## Control Unit (CU)
---

#### Makeup of an Instruction

| Opcode (6b) | Operand 0 (2b) | Operand 1 (4b) | Operand 2 (2b) | Immediate (16b) |
| ----------- | -------------- | -------------- | -------------- | --------------- |
|             |                |                |                |                 |
|             |                |                |                |                 |

#### Control Signals

| Control Signal ID       | Width | Role                                                                                |
| ----------------------- | ----- | ----------------------------------------------------------------------------------- |
| bus_src_raw             | 5b    | Selects what outputs to the bus (Register / Expansion unit Index based)             |
| bus_dest_raw            | 5b    | Selects what takes an input from the bus (Register / Expansion unit Index based)    |
| bus_dest_mem_sp_raw     | 1b    | Select memory and stack pointer registers to take an input from the bus.            |
|                         |       |                                                                                     |
| alu_e_raw               | 1b    | 1: ALU On / 0: Segment Selection and ALU Off (so No Memory use while using the ALU) |
| seg_sel__alu_op_sel_raw | 4b    | Selection of Segment or ALU opcode                                                  |
|                         |       |                                                                                     |
| alu_op_sel              | 4b    |                                                                                     |
| alu_a_sel_raw           | 4b    |                                                                                     |
| alu_b_sel_raw           | 4b    |                                                                                     |
|                         |       |                                                                                     |
| pc_e                    | 1b    |                                                                                     |
| flags_e                 | 1b    |                                                                                     |
| byte_low                | 1b    |                                                                                     |
|                         |       |                                                                                     |
| operand_0_raw           | 2b    |                                                                                     |
| operand_1_raw           | 2b    |                                                                                     |
| operand_2_raw           | 2b    |                                                                                     |

#### Decodes of the Outbound Control Signals

| Operand Condition          | Sub Condition | bus_src (31:0)       | bus_dest (31:0)      | seg_sel (3:0)           | alu_op_sel (3:0)        | alu_a_sel (15:0) | alu_b_sel (15:0) |
| -------------------------- | ------------- | -------------------- | -------------------- | ----------------------- | ----------------------- | ---------------- | ---------------- |
| DEFAULT                    |               | bus_src_raw          | bus_dest_raw         |                         | 0                       | alu_a_sel_raw    | alu_b_sel_raw    |
| -                          | alu_e_raw     | bus_src_raw          | bus_dest_raw         |                         | seg_sel__alu_op_sel_raw | alu_a_sel_raw    | alu_b_sel_raw    |
|                            |               |                      |                      |                         |                         |                  |                  |
| OP0_BUS_DEST               |               |                      | (3:0) <- operand_0   |                         |                         |                  |                  |
| OP0_BUS_SRC                |               | (3:0) <- operand_0   |                      |                         |                         |                  |                  |
| OP0_SEG_SEL                |               |                      |                      | seg_sel__alu_op_sel_raw |                         |                  |                  |
|                            |               |                      |                      |                         |                         |                  |                  |
| OP1_BUS_DEST               |               |                      | (15:0) <- operand_1  |                         |                         |                  |                  |
| -                          | SEL_ES        |                      | (31:16) <- operand_1 |                         |                         |                  |                  |
| OP1_BUS_SRC_OR_ALU_A_SEL   |               | (15:0) <- operand_1  |                      |                         |                         |                  |                  |
| -                          | alu_e_raw     |                      |                      |                         |                         | operand_1        |                  |
| -                          | SEL_ES        | (31:16) <- operand_1 |                      |                         |                         |                  |                  |
| OP1_BUS_DEST_AND_ALU_A_SEL |               |                      | (15:0) <- operand_1  |                         |                         | operand_1        |                  |
|                            |               |                      |                      |                         |                         |                  |                  |
| OP2_BUS_DEST               |               |                      | (15:0) <- operand_2  |                         |                         |                  |                  |
| OP2_BUS_SRC_OR_ALU_B_SEL   |               | (15:0) <- operand_2  |                      |                         |                         |                  |                  |
| -                          | alu_e_raw     |                      |                      |                         |                         |                  | operand_2        |
| OP2_ALU_A_SEL              |               |                      |                      |                         |                         | operand_2        |                  |

