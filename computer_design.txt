Overall:
    16 Bit Bus
    24 Bit Address Bus (Segmented)
    5 General Registers
    4 Extension Ports

Registers:
    ra, rb, rc, rd, re
    instr, mem, pc
    sp, bp, f
    cs, ds, ss, es

Instruction Set:
    (opcode) - (segment) - (operand: dest), (operand: src)

    mov
    str
    ldr
    
    ldepa
    stepa
    ldepb
    stepb
    ldepc
    stepc
    ldepd
    stepd

    jmp
    cmp
    jz
    jc

    add
    sub
    inc
    dec
    mul
    div
    and
    or

    push
    pop
    call
    ret
