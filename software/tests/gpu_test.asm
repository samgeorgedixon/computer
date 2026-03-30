%org 0x100
%segment code

li sp 0xff
mov bp sp

mov r1 z
li r3 0x7fff

ldw c r2 [z colour]

:loop
    and r2 r1 r3

    ste r2 exp2 [r1 0]

    inc r1 r1

    jc [z end]
    jmp [z loop]

:end
    jmp [z end]

:colour
%dw 0b0111110000000000
