%segment code
:main
push bp
mov bp sp
li ds 2
li ss 2
li bp 0xff
mov sp bp
li r3 7
sub r2 sp r3
mov sp r2
li r3 7
sub r2 sp r3
mov sp r2
ldb s r1 [bp -6]
stb s r1 [bp -13]
ldb s r1 [bp -5]
stb s r1 [bp -12]
ldb s r1 [bp -4]
stb s r1 [bp -11]
ldb s r1 [bp -3]
stb s r1 [bp -10]
ldb s r1 [bp -2]
stb s r1 [bp -9]
ldb s r1 [bp -1]
stb s r1 [bp -8]
ldb s r1 [bp 0]
stb s r1 [bp -7]
call [z LoadKernel]
ldb s r1 [bp -13]
stb s r1 [bp -6]
ldb s r1 [bp -12]
stb s r1 [bp -5]
ldb s r1 [bp -11]
stb s r1 [bp -4]
ldb s r1 [bp -10]
stb s r1 [bp -3]
ldb s r1 [bp -9]
stb s r1 [bp -2]
ldb s r1 [bp -8]
stb s r1 [bp -1]
ldb s r1 [bp -7]
stb s r1 [bp 0]
li r1 7
add r1 sp r1
mov sp r1
:while_378_true
li r2 1
li r1 0
cmp r1 r2
jz [z while_378_true_end]
:while_378_true_loop
jmp [z while_378_true]
:while_378_true_end
li sp -7
:main_end
mov sp bp
pop bp
ret
:CheckStrEqual
push bp
mov bp sp
push z
li r1 0
stw s r1 [bp -1]
li r1 1
stb s r1 [bp 4]
:while_9_i
ldw s r1 [bp -1]
li r2 -1
cmp r1 r2
jz [z while_9_i_end]
:while_9_i_loop
ldb s r2 [bp 5]
li r1 0
cmp r1 r2
jz [z if_15_strA_end]
:if_15_strA
li r1 0
stb s r1 [bp 4]
li r1 -1
stw s r1 [bp -1]
:if_15_strA_end
li sp -2
ldb s r2 [bp 5]
li r1 0
cmp r1 r2
jz [z if_26_strA_end]
:if_26_strA
li r1 -1
stw s r1 [bp -1]
:if_26_strA_end
li sp -2
jmp [z while_9_i]
:while_9_i_end
li sp -2
:CheckStrEqual_end
mov sp bp
pop bp
ret
:GetDriveByte
push bp
mov bp sp
ldw s r2 [bp 9]
ldw s es [bp 7]
lde e1 r1 [r2 0]
stb s r1 [bp 6]
:GetDriveByte_end
mov sp bp
pop bp
ret
:GetDriveWord
push bp
mov bp sp
ldw s r2 [bp 10]
ldw s es [bp 8]
lde e1 r1 [r2 0]
stw s r1 [bp 6]
:GetDriveWord_end
mov sp bp
pop bp
ret
:GetFileTableIndex
push bp
mov bp sp
push z
li r1 0
stw s r1 [bp -1]
:while_70_i
ldw s r1 [bp -1]
li r2 250
cmp r1 r2
jz [z while_70_i_end]
:while_70_i_loop
li r3 7
sub r2 sp r3
mov sp r2
push z
li r1 0
stw s r1 [bp -10]
:while_82_j
ldw s r1 [bp -10]
li r2 6
cmp r1 r2
jz [z while_82_j_end]
:while_82_j_loop
push z
ldw s r1 [bp -1]
stw s r1 [bp -12]
ldw s r1 [bp -10]
ldw s r2 [bp -12]
add r1 r2 r1
stw s r1 [bp -12]
push z
ldw s r1 [bp -12]
stw s r1 [bp -14]
li r3 6
sub r2 sp r3
mov sp r2
li r1 0
stw s r1 [bp -20]
pushb z
ldw s r3 [bp -10]
add r3 r3 bp
ldb s r1 [r3 -8]
stb s r1 [bp -21]
call [z GetDriveByte]
ldb s r1 [bp -21]
stb s r1 [bp -8]
ldb s r1 [bp -20]
stb s r1 [bp -7]
ldb s r1 [bp -19]
stb s r1 [bp -6]
ldb s r1 [bp -18]
stb s r1 [bp -5]
ldb s r1 [bp -17]
stb s r1 [bp -4]
ldb s r1 [bp -16]
stb s r1 [bp -3]
ldb s r1 [bp -15]
stb s r1 [bp -2]
ldw s r3 [bp -10]
li r1 9
add r1 sp r1
mov sp r1
li r1 1
ldw s r2 [bp -10]
add r1 r2 r1
stw s r1 [bp -10]
jmp [z while_82_j]
:while_82_j_end
li sp -11
pushb z
li r3 20
sub r2 sp r3
mov sp r2
stb s r1 [bp -31]
stb s r1 [bp -30]
stb s r1 [bp -29]
stb s r1 [bp -28]
stb s r1 [bp -27]
stb s r1 [bp -26]
stb s r1 [bp -25]
stb s r1 [bp -24]
stb s r1 [bp -23]
stb s r1 [bp -22]
stb s r1 [bp -21]
stb s r1 [bp -20]
stb s r1 [bp -19]
stb s r1 [bp -18]
stb s r1 [bp -17]
stb s r1 [bp -16]
stb s r1 [bp -15]
stb s r1 [bp -14]
stb s r1 [bp -13]
stb s r1 [bp -12]
li r3 20
sub r2 sp r3
mov sp r2
ldb s r1 [bp -8]
stb s r1 [bp -51]
ldb s r1 [bp -7]
stb s r1 [bp -50]
ldb s r1 [bp -6]
stb s r1 [bp -49]
ldb s r1 [bp -5]
stb s r1 [bp -48]
ldb s r1 [bp -4]
stb s r1 [bp -47]
ldb s r1 [bp -3]
stb s r1 [bp -46]
ldb s r1 [bp -2]
stb s r1 [bp -45]
ldb s r1 [bp -1]
stb s r1 [bp -44]
ldb s r1 [bp 0]
stb s r1 [bp -43]
ldb s r1 [bp 1]
stb s r1 [bp -42]
ldb s r1 [bp 2]
stb s r1 [bp -41]
ldb s r1 [bp 3]
stb s r1 [bp -40]
ldb s r1 [bp 4]
stb s r1 [bp -39]
ldb s r1 [bp 5]
stb s r1 [bp -38]
ldb s r1 [bp 6]
stb s r1 [bp -37]
ldb s r1 [bp 7]
stb s r1 [bp -36]
ldb s r1 [bp 8]
stb s r1 [bp -35]
ldb s r1 [bp 9]
stb s r1 [bp -34]
ldb s r1 [bp 10]
stb s r1 [bp -33]
ldb s r1 [bp 11]
stb s r1 [bp -32]
pushb z
ldb s r1 [bp -11]
stb s r1 [bp -52]
call [z CheckStrEqual]
ldb s r1 [bp -52]
stb s r1 [bp -11]
li r1 41
add r1 sp r1
mov sp r1
ldb s r2 [bp -11]
li r1 0
cmp r1 r2
jz [z if_210_equal_end]
:if_210_equal
ldw s r1 [bp -1]
stw s r1 [bp 4]
jmp [z GetFileTableIndex_end]
:if_210_equal_end
li sp -12
li r1 10
ldw s r2 [bp -1]
add r1 r2 r1
stw s r1 [bp -1]
jmp [z while_70_i]
:while_70_i_end
li sp -2
:GetFileTableIndex_end
mov sp bp
pop bp
ret
:GetFile
push bp
mov bp sp
li r1 6
ldw s r2 [bp 8]
add r1 r2 r1
stw s r1 [bp 8]
push z
ldw s r1 [bp 8]
stw s r1 [bp -1]
push z
li r1 0
stw s r1 [bp -3]
push z
ldw s r1 [bp 6]
stw s r1 [bp -5]
call [z GetDriveWord]
ldw s r1 [bp -5]
stw s r1 [bp 6]
li r1 6
add r1 sp r1
mov sp r1
li r1 2
ldw s r2 [bp 8]
add r1 r2 r1
stw s r1 [bp 8]
push z
ldw s r1 [bp 8]
stw s r1 [bp -1]
push z
li r1 0
stw s r1 [bp -3]
push z
ldw s r1 [bp 4]
stw s r1 [bp -5]
call [z GetDriveWord]
ldw s r1 [bp -5]
stw s r1 [bp 4]
li r1 6
add r1 sp r1
mov sp r1
:GetFile_end
mov sp bp
pop bp
ret
:LoadKernel
push bp
mov bp sp
li r3 7
sub r2 sp r3
mov sp r2
push z
li r3 7
sub r2 sp r3
mov sp r2
ldb s r1 [bp -6]
stb s r1 [bp -15]
ldb s r1 [bp -5]
stb s r1 [bp -14]
ldb s r1 [bp -4]
stb s r1 [bp -13]
ldb s r1 [bp -3]
stb s r1 [bp -12]
ldb s r1 [bp -2]
stb s r1 [bp -11]
ldb s r1 [bp -1]
stb s r1 [bp -10]
ldb s r1 [bp 0]
stb s r1 [bp -9]
push z
ldw s r1 [bp -8]
stw s r1 [bp -17]
call [z GetFileTableIndex]
ldw s r1 [bp -17]
stw s r1 [bp -8]
li r1 9
add r1 sp r1
mov sp r1
push z
push z
push z
ldw s r1 [bp -8]
stw s r1 [bp -14]
push z
ldw s r1 [bp -10]
stw s r1 [bp -16]
push z
ldw s r1 [bp -12]
stw s r1 [bp -18]
call [z GetFile]
ldw s r1 [bp -16]
stw s r1 [bp -10]
ldw s r1 [bp -18]
stw s r1 [bp -12]
li r1 6
add r1 sp r1
mov sp r1
jmp [z LoadKernel_end]
:LoadKernel_end
mov sp bp
pop bp
ret
%segment data
