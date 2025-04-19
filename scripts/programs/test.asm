%segment code
:Blah
push bp
mov bp sp
li r1 2
ldw s r2 [bp 8]
mul r1 r2 r1
stw s r1 [bp 8]
stw s r1 [bp 6]
jmp [z Blah_end]
:Blah_end
mov sp bp
pop bp
ret
:FindFile
push bp
mov bp sp
li r4 5
li r1 5
push r1
li r1 2
push r1
li r1 1
ldw s r2 [bp -1]
add r1 r2 r1
stw s r1 [bp -1]
ldw s r1 [bp -3]
ldw s r2 [bp -1]
sub r1 r2 r1
stw s r1 [bp -1]
ldw d r1 [z 0]
ldw s r2 [bp -1]
mul r1 r2 r1
stw s r1 [bp -1]
ldw s r1 [bp -1]
li r2 6
cmp r1 r2
jz [z if_34_a]
jmp [z if_34_a_end]
:if_34_a
ldw s r1 [bp -1]
push r1
li r1 0
push r1
call [z Blah]
pop r1
pop r2
stw s r1 [bp -1]
:if_34_a_end
:while_49_true
li r2 1
li r1 0
cmp r1 r2
jz [z while_49_true_end]
:while_49_true_loop
jmp [z while_49_true]
:while_49_true_end
:FindFile_end
mov sp bp
pop bp
ret
%segment data
%dw 2
