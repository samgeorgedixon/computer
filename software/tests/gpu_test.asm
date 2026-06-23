%org 0x100
%segment code

mov sp 0xff
mov bp sp

mov r1 z
mov r3 0x7fff

ldw c r2 [z colour]

:loop
    and r2 r1 r3

    stxw r2 xu3 [r1]

    inc r1 r1
    bnc [z loop]

:end
    jmp [z end]

:colour
%dw 0b0111110000000000
