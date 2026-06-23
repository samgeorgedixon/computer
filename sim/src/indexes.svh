`ifndef INDEXES_SVH
`define INDEXES_SVH

// Instruction Opcodes

`define INSTR_NOP   0
`define INSTR_MOV   1
`define INSTR_MOVI  2
`define INSTR_LEA   3

`define INSTR_LDW   4
`define INSTR_LDWR  5
`define INSTR_LDWP  6
`define INSTR_LDB   7
`define INSTR_LDBR  8
`define INSTR_LDBP  9
`define INSTR_STW   10
`define INSTR_STWR  11
`define INSTR_STWP  12
`define INSTR_STB   13
`define INSTR_STBR  14
`define INSTR_STBP  15
`define INSTR_LDXW  16
`define INSTR_LDXWR 17
`define INSTR_LDXWP 18
`define INSTR_LDXB  19
`define INSTR_LDXBR 20
`define INSTR_LDXBP 21
`define INSTR_STXW  22
`define INSTR_STXWR 23
`define INSTR_STXWP 24
`define INSTR_STXB  25
`define INSTR_STXBR 26
`define INSTR_STXBP 27

`define INSTR_JMP   28
`define INSTR_JMPF  29
`define INSTR_BZ    30
`define INSTR_BNZ   31
`define INSTR_BC    32
`define INSTR_BNC   33
`define INSTR_BS    34
`define INSTR_BNS   35
`define INSTR_BO    36
`define INSTR_BNO   37

`define INSTR_ADD   38
`define INSTR_SUB   39
`define INSTR_INC   40
`define INSTR_INC2  41
`define INSTR_DEC   42
`define INSTR_DEC2  43
`define INSTR_NOT   44
`define INSTR_AND   45
`define INSTR_OR    46
`define INSTR_XOR   47
`define INSTR_SLL   48
`define INSTR_SRL   49
`define INSTR_SRA   50
`define INSTR_NEG   51
`define INSTR_CMP   52

`define INSTR_PUSH  53
`define INSTR_PUSHB 54
`define INSTR_POP   55
`define INSTR_POPB  56
`define INSTR_CALL  57
`define INSTR_CALLF 58
`define INSTR_RET   59
`define INSTR_RETF  60

// ALU Opcodes
`define ALU_ADD     1
`define ALU_SUB     2

`define ALU_INC     3
`define ALU_INC2    4
`define ALU_DEC     5
`define ALU_DEC2    6

`define ALU_NOT     7
`define ALU_AND     8
`define ALU_OR      9
`define ALU_XOR     10

`define ALU_SLL     11
`define ALU_SRL     12
`define ALU_SRA     13
`define ALU_NEG     14

// Registers (1 - 15), Expansion Units (16 - 31), 0 (Nothing)
`define R_R1    1
`define R_R2    2
`define R_R3    3
`define R_R4    4

`define R_SP    5
`define R_BP    6

`define R_CS    7
`define R_DS    8
`define R_SS    9
`define R_ES    10

`define R_ZERO  11

`define R_IP    12
`define R_IR    13
`define R_AR    14

`define R_XU1   16 // Memory
`define R_XU2   17 // Drive
`define R_XU3   18
`define R_XU4   19

// Flags (flags[index])
`define F_ZERO          0 // Z
`define F_CARRY         1 // C
`define F_SIGN          2 // S
`define F_OVERFLOW      3 // O

// Segment Selects
`define SEL_CS          4'b0001
`define SEL_DS          4'b0010
`define SEL_SS          4'b0100
`define SEL_ES          4'b1000

// Operand Roles (Use Inside SET_CS_RAW)
`define OP0_BUS_DEST                2'd1
`define OP0_BUS_SRC                 2'd2
`define OP0_SEG_SEL                 2'd3

`define OP1_BUS_DEST                2'd1 // or R_ExpansionUnit
`define OP1_BUS_SRC_OR_ALU_A_SEL    2'd2 // or R_ExpansionUnit
`define OP1_BUS_DEST_AND_ALU_A_SEL  2'd3

`define OP2_BUS_DEST                2'd1
`define OP2_BUS_SRC_OR_ALU_B_SEL    2'd2
`define OP2_ALU_A_SEL               2'd3

`endif
