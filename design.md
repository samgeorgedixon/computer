# Computer Design

 - 16 Bit Bus
 - 24 Bit Address Bus (Segment 0xffff00) + (Address 0x00ffff)
 - 4 General Registers
 - 16 Expansion Modules
 - Big Endian

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

- operand-0 - 2b (00: Off, 01: seg_sel, 10: bus_dest, 11: bus_src)
- operand-1 - 2b (00: Off, 01: bus_dest, 10: bus_src, 01&alu_e_raw: alu-a-sel)
- operand-2 - 1b (0: Off, 1&alu_e_raw: alu-b-sel, 1: bus_src)

 -  (opcode, 6b)    (operand-0, 2b)   (operand-1, 4b)    (operand-2, 4b)    (imm, 16b)
    ldw                  {seg}                     rs1                           [rs2                          {imm}]
	.                       1 + 3o                  1 + 2o                     1 + 1o - src              1 + 1o - bus : ( + alu_e)

- instr-end - 1b

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
	- clk, r
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

### Instruction Set
 -  (opcode, 6b)    (operand-0, 2b)   (operand-1, 4b)    (operand-2, 4b)    (imm, 16b)
 -  [op2 imm] - address = op2 + imm

0. nop

1. mov dest src
2. li dest imm

3. ldw seg dest [op2 imm]
4. stw seg dest [op2 imm]
5. lde dest exp [op2 imm]
6. ste dest exp [op2 imm]

7. jmp [op2 imm]
8. jmpf op1 [op2 imm]
9. jz [op2 imm]
10. jc [op2 imm]

11. add res op1 op2
12. sub res op1 op2
13. inc res op1
14. inc2 res op1
15. dec res op1
16. dec2 res op1
17. mul res op1 op2
18. div res op1 op2
19. and res op1 op2
20. or res op1 op2
21. cmp op1 op2

22. push src
23. pop dest
24. call [op2 imm]
25. callf op1 [op2 imm]
26. ret
27. retf
