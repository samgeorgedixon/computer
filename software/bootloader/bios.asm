%segment code

li r1 0x0100
li r2 0x0200

:loop
    lde r3 exp1 [r1 0]
    stw c r3 [r1 0]

    inc2 r1 r1

    cmp r1 r2
    jz [z start]

    jmp [z loop]

:start
    li ds 1
    li ss 1

    li r1 1
    jmpf r1 [z 0]
