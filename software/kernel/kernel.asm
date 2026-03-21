%segment code
%dw data
:main
push bp
mov bp sp
push z
ldw d r1 [z 0]
stw s r1 [bp -1]
call [z FillScreen]
li r1 2
add r1 sp r1
mov sp r1
:while_72_true
li r2 1
li r1 0
cmp r1 r2
jz [z while_72_true_end]
:while_72_true_loop
jmp [z while_72_true]
:while_72_true_end
li r1 0
add r1 bp r1
mov sp r1
:main_end
mov sp bp
pop bp
ret
:PrintPixel
push bp
mov bp sp
li es 0
ldw s r1 [bp 5]
ldw s r2 [bp 7]
ste e2 r1 [r2 0]
:PrintPixel_end
mov sp bp
pop bp
ret
:FillScreen
push bp
mov bp sp
push z
li r1 0
stw s r1 [bp -1]
pushb z
:while_20_done
ldb s r1 [bp -2]
li r2 0
cmp r1 r2
jz [z while_20_done_loop]
jmp [z while_20_done_end]
:while_20_done_loop
push z
ldw s r1 [bp -1]
stw s r1 [bp -4]
push z
ldw s r1 [bp 5]
stw s r1 [bp -6]
call [z PrintPixel]
li r1 4
add r1 sp r1
mov sp r1
li r1 1
ldw s r2 [bp -1]
add r1 r2 r1
stw s r1 [bp -1]
ldw s r1 [bp -1]
li r2 0
cmp r1 r2
jz [z if_41_addr]
jmp [z if_41_addr_end]
:if_41_addr
li r1 1
stb s r1 [bp -2]
:if_41_addr_end
li r1 -3
add r1 bp r1
mov sp r1
jmp [z while_20_done]
:while_20_done_end
li r1 -3
add r1 bp r1
mov sp r1
:FillScreen_end
mov sp bp
pop bp
ret
:data
%segment data
%dw 31775
