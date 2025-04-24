%segment code
:main
push bp
mov bp sp
li r4 5
li r3 20
sub r2 sp r3
mov sp r2
li r3 20
sub r2 sp r3
mov sp r2
li r3 5
sub r2 sp r3
mov sp r2
li r1 11
stw s r1 [bp -15]
li r1 1
stb s r1 [bp -40]
li r1 3
stb d r1 [z 5]
push z
li r1 5
stw s r1 [bp -46]
push z
li r1 2
stw s r1 [bp -48]
li r1 1
ldw s r2 [bp -46]
add r1 r2 r1
stw s r1 [bp -46]
ldw s r1 [bp -48]
ldw s r2 [bp -46]
sub r1 r2 r1
stw s r1 [bp -46]
ldw d r1 [z 0]
ldw s r2 [bp -46]
mul r1 r2 r1
stw s r1 [bp -46]
ldw s r1 [bp -46]
li r2 6
cmp r1 r2
jz [z if_52_a]
jmp [z if_52_a_end]
:if_52_a
push z
ldw s r1 [bp -46]
stw s r1 [bp -50]
push z
call [z Blah]
pop r1
li r2 2
add r2 sp r2
mov sp r2
stw s r1 [bp -46]
:if_52_a_end
li sp -49
:while_70_true
li r2 1
li r1 0
cmp r1 r2
jz [z while_70_true_end]
:while_70_true_loop
jmp [z while_70_true]
:while_70_true_end
li sp -49
:main_end
mov sp bp
pop bp
ret
:Blah
push bp
mov bp sp
li r1 2
ldw s r2 [bp 8]
mul r1 r2 r1
stw s r1 [bp 8]
ldw s r1 [bp 8]
stw s r1 [bp 6]
jmp [z Blah_end]
:Blah_end
mov sp bp
pop bp
ret
%segment data
%dw 2
%db 0
%db 0
%db 0
%db 0
