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
jz [z if_57_a]
jmp [z if_57_a_end]
:if_57_a
push z
ldw s r1 [bp -46]
stw s r1 [bp -50]
call [z Blah]
ldw s r1 [bp -50]
stw s r1 [bp -46]
li r1 2
add r1 sp r1
mov sp r1
:if_57_a_end
li sp -49
push z
ldw s r1 [bp -46]
stw s r1 [bp -50]
pushb z
ldw d r1 [z 0]
stb s r1 [bp -51]
li r3 5
sub r2 sp r3
mov sp r2
ldb s r1 [bp -44]
stb s r1 [bp -56]
ldb s r1 [bp -43]
stb s r1 [bp -55]
ldb s r1 [bp -42]
stb s r1 [bp -54]
ldb s r1 [bp -41]
stb s r1 [bp -53]
ldb s r1 [bp -40]
stb s r1 [bp -52]
call [z hello]
ldb s r1 [bp -56]
stb s r1 [bp -44]
ldb s r1 [bp -55]
stb s r1 [bp -43]
ldb s r1 [bp -54]
stb s r1 [bp -42]
ldb s r1 [bp -53]
stb s r1 [bp -41]
ldb s r1 [bp -52]
stb s r1 [bp -40]
li r1 8
add r1 sp r1
mov sp r1
:while_107_true
li r2 1
li r1 0
cmp r1 r2
jz [z while_107_true_end]
:while_107_true_loop
jmp [z while_107_true]
:while_107_true_end
li sp -49
:main_end
mov sp bp
pop bp
ret
:Blah
push bp
mov bp sp
li r1 2
ldw s r2 [bp 5]
mul r1 r2 r1
stw s r1 [bp 5]
:Blah_end
mov sp bp
pop bp
ret
:hello
push bp
mov bp sp
jmp [z hello_end]
:hello_end
mov sp bp
pop bp
ret
%segment data
%dw 2
%db 0
%db 0
%db 0
%db 0
