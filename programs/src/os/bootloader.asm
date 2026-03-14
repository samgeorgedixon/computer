%segment code
li ds 2
li ss 2
li bp 0xffff
mov sp bp
:main
push bp
mov bp sp
push z
li r1 31744
stw s r1 [bp -1]
call [z FillScreen]
li r1 2
add r1 sp r1
mov sp r1
li r3 7
sub r2 sp r3
mov sp r2
li r3 6
sub r2 sp r3
mov sp r2
ldb s r1 [bp -6]
stb s r1 [bp -12]
ldb s r1 [bp -5]
stb s r1 [bp -11]
ldb s r1 [bp -4]
stb s r1 [bp -10]
ldb s r1 [bp -3]
stb s r1 [bp -9]
ldb s r1 [bp -2]
stb s r1 [bp -8]
ldb s r1 [bp -1]
stb s r1 [bp -7]
call [z LoadKernel]
ldb s r1 [bp -12]
stb s r1 [bp -6]
ldb s r1 [bp -11]
stb s r1 [bp -5]
ldb s r1 [bp -10]
stb s r1 [bp -4]
ldb s r1 [bp -9]
stb s r1 [bp -3]
ldb s r1 [bp -8]
stb s r1 [bp -2]
ldb s r1 [bp -7]
stb s r1 [bp -1]
ldb s r1 [bp -6]
stb s r1 [bp 0]
li r1 6
add r1 sp r1
mov sp r1
:while_441_true
li r2 1
li r1 0
cmp r1 r2
jz [z while_441_true_end]
:while_441_true_loop
jmp [z while_441_true]
:while_441_true_end
li r1 -7
add r1 bp r1
mov sp r1
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
stb s r1 [bp 5]
:while_13_i
ldw s r1 [bp -1]
li r2 -1
cmp r1 r2
jz [z while_13_i_end]
:while_13_i_loop
ldb s r2 [bp 6]
li r1 0
cmp r1 r2
jz [z if_19_strA_end]
:if_19_strA
li r1 0
stb s r1 [bp 5]
li r1 -1
stw s r1 [bp -1]
:if_19_strA_end
li r1 -2
add r1 bp r1
mov sp r1
ldb s r2 [bp 6]
li r1 0
cmp r1 r2
jz [z if_32_strA_end]
:if_32_strA
li r1 -1
stw s r1 [bp -1]
:if_32_strA_end
li r1 -2
add r1 bp r1
mov sp r1
jmp [z while_13_i]
:while_13_i_end
li r1 -2
add r1 bp r1
mov sp r1
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
:GetFileAddrSize
push bp
mov bp sp
push z
li r1 0
stw s r1 [bp -1]
push z
li r1 0
stw s r1 [bp -3]
:while_83_i
ldw s r1 [bp -3]
li r2 250
cmp r1 r2
jz [z while_83_i_end]
:while_83_i_loop
li r3 7
sub r2 sp r3
mov sp r2
push z
li r1 0
stw s r1 [bp -12]
:while_95_j
ldw s r1 [bp -12]
li r2 6
cmp r1 r2
jz [z while_95_j_end]
:while_95_j_loop
push z
ldw s r1 [bp -3]
stw s r1 [bp -14]
ldw s r1 [bp -12]
ldw s r2 [bp -14]
add r1 r2 r1
stw s r1 [bp -14]
push z
ldw s r1 [bp -14]
stw s r1 [bp -16]
li r3 6
sub r2 sp r3
mov sp r2
li r1 0
stw s r1 [bp -22]
pushb z
ldw s r3 [bp -12]
add r3 r3 bp
ldb s r1 [r3 -10]
stb s r1 [bp -23]
call [z GetDriveByte]
ldb s r1 [bp -23]
stb s r1 [bp -10]
ldb s r1 [bp -22]
stb s r1 [bp -9]
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
ldw s r3 [bp -12]
li r1 9
add r1 sp r1
mov sp r1
li r1 1
ldw s r2 [bp -12]
add r1 r2 r1
stw s r1 [bp -12]
jmp [z while_95_j]
:while_95_j_end
li r1 -13
add r1 bp r1
mov sp r1
pushb z
li r3 20
sub r2 sp r3
mov sp r2
stb s r1 [bp -33]
stb s r1 [bp -32]
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
li r3 20
sub r2 sp r3
mov sp r2
ldb s r1 [bp -10]
stb s r1 [bp -53]
ldb s r1 [bp -9]
stb s r1 [bp -52]
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
pushb z
ldb s r1 [bp -13]
stb s r1 [bp -54]
call [z CheckStrEqual]
ldb s r1 [bp -54]
stb s r1 [bp -13]
li r1 41
add r1 sp r1
mov sp r1
ldb s r2 [bp -13]
li r1 0
cmp r1 r2
jz [z if_225_equal_end]
:if_225_equal
ldw s r1 [bp -3]
stw s r1 [bp -1]
jmp [z GetFileAddrSize_end]
:if_225_equal_end
li r1 -14
add r1 bp r1
mov sp r1
li r1 10
ldw s r2 [bp -3]
add r1 r2 r1
stw s r1 [bp -3]
jmp [z while_83_i]
:while_83_i_end
li r1 -4
add r1 bp r1
mov sp r1
li r1 6
ldw s r2 [bp -1]
add r1 r2 r1
stw s r1 [bp -1]
push z
ldw s r1 [bp -1]
stw s r1 [bp -5]
push z
li r1 0
stw s r1 [bp -7]
push z
ldw s r1 [bp 7]
stw s r1 [bp -9]
call [z GetDriveWord]
ldw s r1 [bp -9]
stw s r1 [bp 7]
li r1 6
add r1 sp r1
mov sp r1
li r1 2
ldw s r2 [bp -1]
add r1 r2 r1
stw s r1 [bp -1]
push z
ldw s r1 [bp -1]
stw s r1 [bp -5]
push z
li r1 0
stw s r1 [bp -7]
push z
ldw s r1 [bp 5]
stw s r1 [bp -9]
call [z GetDriveWord]
ldw s r1 [bp -9]
stw s r1 [bp 5]
li r1 6
add r1 sp r1
mov sp r1
:GetFileAddrSize_end
mov sp bp
pop bp
ret
:LoadFile
push bp
mov bp sp
:LoadFile_end
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
:while_313_done
ldb s r1 [bp -2]
li r2 0
cmp r1 r2
jz [z while_313_done_loop]
jmp [z while_313_done_end]
:while_313_done_loop
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
jz [z if_334_addr]
jmp [z if_334_addr_end]
:if_334_addr
li r1 1
stb s r1 [bp -2]
:if_334_addr_end
li r1 -3
add r1 bp r1
mov sp r1
jmp [z while_313_done]
:while_313_done_end
li r1 -3
add r1 bp r1
mov sp r1
:FillScreen_end
mov sp bp
pop bp
ret
:LoadKernel
push bp
mov bp sp
li r3 6
sub r2 sp r3
mov sp r2
push z
call [z GetFileTableIndex]
li r1 0
add r1 sp r1
mov sp r1
push z
push z
call [z GetFile]
li r1 0
add r1 sp r1
mov sp r1
push z
ldw s r1 [bp -9]
stw s r1 [bp -13]
call [z FillScreen]
li r1 2
add r1 sp r1
mov sp r1
ldb s r1 [bp -5]
stb s r1 [bp 5]
ldb s r1 [bp -4]
stb s r1 [bp 6]
ldb s r1 [bp -3]
stb s r1 [bp 7]
ldb s r1 [bp -2]
stb s r1 [bp 8]
ldb s r1 [bp -1]
stb s r1 [bp 9]
ldb s r1 [bp 0]
stb s r1 [bp 10]
:LoadKernel_end
mov sp bp
pop bp
ret
%segment data
