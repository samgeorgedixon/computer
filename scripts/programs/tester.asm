%segment code
%org 16

li cs 0x0000
li ds 0x0001
li ss 0x0000

li bp 0xffff
mov sp bp

li r1 0
call [r1 print]

:end
jmp [r3 end]

:print
push bp
mov bp sp

li r1 0
li r3 0
ldw d r2 [r3 colour]

:print_loop
stew e2 r2 [r1 0]
inc r1 r1

jc [r3 print_end]
jmp [r3 print_loop]

:print_end
mov sp bp
pop bp
ret

%org 0x000100
%segment data

:colour
%dw 0b0111110000011111
