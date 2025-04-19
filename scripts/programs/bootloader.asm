%segment code

%include "scripts/programs/file_system.asm"

:setup
    li ds 2
    li ss 2
    
    li bp 0xff
    mov sp bp

:test
    li r1 0
    push r1
    push r1

    li r1 0x4141
    push r1
    push r1
    push r1

    call [z find_file]

    pop 0
    pop 0
    pop 0

    pop r2
    pop r1

:end
    jmp [z end]

%org 0x100
%segment data

%dw 0
