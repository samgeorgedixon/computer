%segment code
%org 0x10

mov cs 0

mov ra 0x200
mov rb 0x

:loop
lddr rc ra
str ra rc
inc ra

cmp
jz 16

jmp loop


mov cs 0x0002
mov ds 0x0102
mov ss 0x0202
