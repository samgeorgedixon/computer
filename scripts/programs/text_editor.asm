%org 16

mov sb 1024
mov sp sb

ldr ra 0b00001000001
str put_pos ra
ldr ra A
str put_char_code ra
call put

ldr ra 0b00001000110
str put_pos ra
ldr ra B
str put_char_code ra
call put

ldr ra 0b00001001011
str put_pos ra
ldr ra C
str put_char_code ra
call put

ldr ra 0b00001010000
str put_pos ra
ldr ra D
str put_char_code ra
call put

:end
jmp end

// Put Function
:put
ldr ra put_pos
ldr rb put_gpu_data
or ra

:loop
push ra
ldr ra put_index
lda rb put_char_code
and

pop ra
jz blank
jmp white

:blank
ldr rb 0b11111111111
and ra

:white
wepa
inc ra

ldr rb put_gpu_data
or ra

push ra
ldr ra put_index
ldr rb 2
div
str put_index ra
pop ra

push ra
ldr rb 0b111111
and ra

push ra
ldr ra 4
ldr rb put_pos
add ra
ldr rb 0b111111
and rb
pop ra
sub ra
pop rb

jz outer_loop
jmp loop

:outer_loop
swb
ldi 0b00001000000
add

pusha
swb
ldi 0b11111000000
and

pusha
ldi 0b100000000
swb
lda put_pos
add
swb
ldi 0b11111000000
and
swb
popa
sub
swb
popa

jz put_end

swb
ldi 0b11111000000
and
pusha
lda put_pos
swb
ldi 0b00000111111
and
swb
popa
or
swb
lda put_gpu_data
or

jmp loop

:put_end
ldi 0
sta put_char_code
sta put_pos
ldi 0x80
swb
ldi 256
mult
sta put_index
ret

:seg_data

:put_gpu_data
%dw 0b0111000000000000
:put_pos
%dw 0b000000000000
:put_index
%dw 0b1000000000000000
:put_char_code
%dw 0

:chars
// SP - 32 = 0000,0000,0000,0000
:SP
%dw 0b0000000000000000
// A - 65 = 1111,1001,1111,1001
:A
%dw 0b1111100111111001
// B - 66 = 1110,1011,1101,1111
:B
%dw 0b1110101111011111
// C - 67 = 1111,1000,1000,1111
:C
%dw 0b1111100010001111
// D - 68 = 1110,1001,1001,1110
:D
%dw 0b1110100110011110
