%org 0x0
%segment code

li ss 1
li sp 0xff
mov bp sp

call [z boom]

li r1 0x0f

:boom
li r1 0xf0
ret
li r1 0xf00
