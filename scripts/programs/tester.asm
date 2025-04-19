%segment code

%define red [z red]

li ds 2
li ss 2

li bp 0xff
mov sp bp

ldw d r1 $red

push r1
call [z fill]
pop r1

ldw d r1 [z green]

push r1
call [z fill]
pop r1

:end
    jmp [z end]

:fill
    push bp
    mov bp sp

    li r1 0
    ldw s r2 [bp 6]

    :fill_loop
    ste e2 r2 [r1 0]
    inc r1 r1

    jc [z fill_end]
    jmp [z fill_loop]

    :fill_end
    mov sp bp
    pop bp
    ret

%org 0x100
%segment data

:red
%dw 0b0111110000000000
:green
%dw 0b0000001111100000
