%segment code
%org 16

li cs 0x0000
li ds 0x0001
li ss 0x0001

li bp 0xffff
mov sp bp

li r1 0
ldw d r2 [r3 colour]
li r3 0

:loop
stew e2 r2 [r1 0]
inc r1 r1

jc [r3 end]
jmp [r3 loop]

:end
jmp [r3 end]

%org 0x000100
%segment data

:colour
%dw 0b0111111111111111
