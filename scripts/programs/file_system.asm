// Expects Setup

%include "scripts/programs/std/vars.asm"

// params - par5 out: int16 startSeg, par4 out: int16 sizeSeg, par1-2-3 in: char name[6]
:find_file
    push bp
    mov bp sp

    li r1 0 // var1: addr
    push r1
    
    li r1 0 // var2: nameIndex
    push r1

    li r1 1 // var3: charEqual
    push r1

    :find_file_loop
    
        :find_file_inner_loop
        ldw s r1 $var2

        li r2 10
        add r2 r2 bp
        add r2 r2 r1

        ldw s r3 [r2 0] // param name[i]

        ldw s r2 $var1
        add r2 r2 r1
        lde e1 r4 [r2 0] // file table name[i]

        cmp r3 r4
        jz [z find_file_inner_loop_end]

        :find_file_inner_loop_false
        li r1 0
        stw s r1 $var3

        jmp [z find_file_loop_end]

        :find_file_inner_loop_end
        li r2 6

        inc r1 r1
        cmp r1 r2

        stw s r1 $var2

        jz [z find_file_loop_check]
        jmp [z find_file_loop]

    :find_file_loop_check
    ldw s r1 $var3
    li r2 1

    cmp r1 r2
    jz [z find_file_found]
    jmp [z find_file_loop_end]

    :find_file_found
    ldw s r1 $var1
    li r2 6
    add r2 r2 r1

    lde e1 r2 [r2 0]
    stw s r2 $par4

    li r2 8
    add r2 r2 r1

    lde e1 r2 [r2 0]
    stw s r2 $par5

    jmp [z find_file_end]
    
    :find_file_loop_end
    li r1 0
    stw s r1 $var2
    li r1 1
    stw s r1 $var3

    ldw s r1 $var1
    li r2 10
    li r3 260
    
    add r1 r1 r2
    cmp r1 r3
    
    stw s r1 $var1
    
    jz [z find_file_end]
    jmp [z find_file_loop]

    :find_file_end
    mov sp bp
    pop bp
    ret
