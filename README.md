# Computer

Here I have designed a custom Computer / CPU.

## Computer Design
 -  16 Bit Bus
 -  24 Bit Address Bus (Segment 0xffff00) + (Address 0x00ffff)
 -  Program Counter + 2
 -  4 General Registers
 -  4 Extension Ports (e1, e2, e3, e4)

## Registers
 -  zero
 -  r1, r2, r3, r4, r5
 -  instr, mem, pc
 -  sp, bp, f
 -  cs, ds, ss, es - Segments (code, data, stack, extra)

## ALU
 -  Operation Select - 4b
 -  A Select - 4b
 -  B Select - 4b

## Instruction Set
 -  (opcode, 6b)    (operand-rs, 2b)   (operand-rs1, 4b)    (operand-rs2, 4b)    (imm, 16b)
    ldw             {seg}              rs1                  [rs2                 {imm}]

 -  {} - optional
 -  [] - address

#### General
 -  No Instr:       nop

#### Moving
 -  Move Reg:       mov rs1 rs2
 -  Load Imm:       li rs1 imm

 -  Load Word:      ldw seg rs1 [rs2 imm]
 -  Load Byte:      ldb seg rs1 [rs2 imm]

 -  Store Word:     stw seg rs1 [rs2 imm]
 -  Store Byte:     stb seg rs1 [rs2 imm]

 -  Load Ext:       lde ext rs1 [rs2 imm]
 -  Store Ext:      ste ext rs1 [rs2 imm]

#### Jump / Conditions
 -  Jump:           jmp      [rs2 imm]
 -  Jump Far:       jmpf rs1 [rs2 imm]
 -  Jump If Zero:   jz       [rs2 imm]
 -  Jump If Carry:  jc       [rs2 imm]
 -  Compare:        cmp rs1 rs2

#### Arithmetic
 -  Addition:       add res rs1 rs2
 -  Subtraction:    sub res rs1 rs2
 -  Increment:      inc res rs1
 -  Decrement:      dec res rs1
 -  Multiply:       mul res rs1 rs2
 -  Divide:         div res rs1 rs2

#### Bitwise
 -  AND:            and res rs1 rs2
 -  OR:             or  res rs1 rs2

#### Stack and Functions
 -  Push Reg:       push rs1
 -  Pop Into Reg:   pop rs1
 -  Function:       call [rs2 imm]
 -  Function Far:   callf rs1 [rs2 imm]
 -  Return:         ret
 -  Return Far:     retf
