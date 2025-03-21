%org 0

mov ra 16
mov rb 512

:loop
ldepa rc ra
str ra rc
inc ra

cmp
jz 16

jmp loop
