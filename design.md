# Computer Design

 -  16 Bit Bus
 -  24 Bit Address Bus (Segment 0xffff00) + (Address 0x00ffff)
 -  4 General Registers
 -  16 Extension Modules

### Control Unit (CU) - Control Signals - 30b (CPLD / EEPROM)

 - bus-src - 5b (16 registers / 16 extensions, other)
 - bus-dest - 5b (16 registers / 16 extensions, other)
 - bus-dest-special - 4b (16 register groups)

 - alu-op-sel / seg-sel - 4b (16 operations / 4 segments)
 - alu-or-seg-pc - 1b (alu-op-sel / seg-sel,  pc-e)

 - alu-a-sel - 4b (16 registers)
 - alu-b-sel - 4b (16 registers)

- pc-e - 1b (Program Counter Enable)
- flags-e - 1b (Enable Flags)

- + Operands

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

#### Extension Units

- Interconnects
	- clk, r
	- we, oe
	- bus - 16b
	- addr - 24b

1.  Memory (Internal)
2. GPU (Internal?)
3. Storage Drive
4. USB - Keyboard / Mouse
5. Ethernet?

#### ALU

 -  Direct BUS Output
 -  Operation Select - 4b
 -  A Select - 4b
 -  B Select - 4b

### Instruction Set
 -  (opcode, 6b)    (operand-rs, 2b)   (operand-rs1, 4b)    (operand-rs2, 4b)    (imm, 16b)
    ldw             {seg}              rs1                  [rs2                 {imm}]

 -  {} - optional
 -  [] - address

1. General
	 -  No Instr:          nop
2.  Moving
	 -  Move Reg:       mov rs1 rs2
	 -  Load Imm:       li rs1 imm
	
	 -  Load Word:      ldw seg rs1 [rs2 imm]
	 -  Load Byte:        ldb seg rs1 [rs2 imm]
	
	 -  Store Word:     stw seg rs1 [rs2 imm]
	 -  Store Byte:       stb seg rs1 [rs2 imm]
	
	 -  Load Ext:          lde ext rs1 [rs2 imm]
	 -  Store Ext:          ste ext rs1 [rs2 imm]
3. Jump / Conditions
	 -  Jump:                jmp      [rs2 imm]
	 -  Jump Far:          jmpf rs1 [rs2 imm]
	 -  Jump If Zero:    jz       [rs2 imm]
	 -  Jump If Carry:   jc       [rs2 imm]
	 -  Compare:         cmp rs1 rs2
4. ALU
	 -  Addition:          add res rs1 rs2
	 -  Subtraction:     sub res rs1 rs2
	 -  Increment:       inc res rs1
	 -  Decrement:      dec res rs1
	 -  Multiply:          mul res rs1 rs2
	 -  Divide:             div res rs1 rs2

	 -  AND:               and res rs1 rs2
	 -  OR:                  or  res rs1 rs2
5. Stack and Functions
	 -  Push Reg:        push rs1
	 -  Pop Into Reg:  pop rs1
	 -  Function:          call [rs2 imm]
	 -  Function Far:    callf rs1 [rs2 imm]
	 -  Return:             ret
	 -  Return Far:       retf
