%segment code

mov r1 0x0100
mov r2 0x0200

:loop
    ldxw r3 xu2 [r1]
    stw+ c r3 [r1]

    cmp r1 r2
    bnz [z loop]

:start
    mov ds 1
    mov ss 1

    mov r1 1
    jmpf r1 [z 0]
