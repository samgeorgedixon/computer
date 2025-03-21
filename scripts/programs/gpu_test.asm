%segment code
%org 16

mov cs 0x0000
mov ds 0x0001
mov ss 0x0001

mov bp 0xffff
mov sp bp

call print

:end
jmp end

:print
push bp
mov bp sp

mov ra 0
ldr d rb colour

:loop
stepb ra rb
inc ra

jc print_end

jmp loop

:print_end
mov sp bp
pop bp

ret

%org 0x000100
%segment data

:colour
%dw 0b0111111111111111
