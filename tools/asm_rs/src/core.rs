// CPU Values
pub enum OpCode {
    NOP = 0, MOV, LI,
    LDW, LDB, STW, STB, LDE, LDEB, STE, STEB,
    JMP, JMPF, BZ, BNZ, BC, BNC, BS, BNS, BO, BNO,
    ADD, SUB, INC, INC2, DEC, DEC2, NOT, AND, OR, XOR, SLL, SRL, SRA, NEG, CMP,
    PUSH, PUSHB, POP, POPB, CALL, CALLF, RET, RETF,

    ORG, SEGMENT, DB, DW, DEFINE,
}

pub enum Register {
    R1 = 1, R2 = 2, R3 = 3, R4 = 4,
    SP = 8, BP = 9,
    CS = 10, DS = 11, SS = 12, ES = 13, ZERO = 14,
    EXP1 = 16, EXP2 = 17,
}

pub enum Segment {
    C = 0, D, S, E
}

// Assembly Program
pub enum Parmameter {
    OP0 = 0, OP1, OP2, IMM, NONE,
}

pub const NO_PARAMS: [Parmameter; 4] = [Parmameter::NONE, Parmameter::NONE, Parmameter::NONE, Parmameter::NONE];

pub struct Instruction {
    pub opcode: OpCode,
    pub params: [Parmameter; 4],

    pub imm: bool,
    pub special: bool,
}

pub struct Label {
    pub name: String,
    pub address: u32,
}

pub struct ProgramData {
    pub labels: Vec<Label>,
}
