%segment code
%org 0

li r1 16
li r2 512

:loop
ldew e1 r3 [r1 0]
stw c r3 [r1 0]

inc r1 r1

cmp r1 r2
jz [z 16]

jmp [z loop]
