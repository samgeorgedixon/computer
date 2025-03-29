%segment code
%org 0

li r1 0x100
li r2 0x300
li r4 2

:loop
ldew e1 r3 [r1 0]
stw c r3 [r1 0]

add r1 r1 r4

cmp r1 r2
jz [z start]

jmp [z loop]

:start
li r1 0x0001
jmpf r1 [z 0]
