use crate::core;

use core::OpCode;
use core::Register;
use core::Segment;

use core::NO_PARAMS;
use core::Parmameter;
use core::Instruction;

use core::Label;
use core::ProgramData;

fn get_instr_type(identifier: &str) -> Instruction {
    return match identifier {
        "nop"       => Instruction { opcode: OpCode::NOP, params: NO_PARAMS, imm: false, special: false },
        "mov"       => Instruction { opcode: OpCode::MOV, params: [Parmameter::OP1, Parmameter::OP2, Parmameter::NONE, Parmameter::NONE], imm: false, special: false },

        "%org"      =>Instruction { opcode: OpCode::ORG, params: NO_PARAMS, imm: false, special: true },
        "%segment"  =>Instruction { opcode: OpCode::SEGMENT, params: NO_PARAMS, imm: false, special: true },
        "%db"       =>Instruction { opcode: OpCode::DB, params: NO_PARAMS, imm: false, special: true },
        "%dw"       =>Instruction { opcode: OpCode::DW, params: NO_PARAMS, imm: false, special: true },
        "%define"   =>Instruction { opcode: OpCode::DEFINE, params: NO_PARAMS, imm: false, special: true },

        _           => Instruction { opcode: OpCode::NOP, params: NO_PARAMS, imm: false, special: false },
    };
}

fn convert_param_to_value(param: &str) -> u16 {
    let param_clean: String = param.replace("[", "");

    match param_clean.as_str() {
        "r1" => return Register::R1 as u16,
        "r2" => return Register::R2 as u16,
        "r3" => return Register::R3 as u16,
        "r4" => return Register::R4 as u16,

        "sp" => return Register::SP as u16,
        "bp" => return Register::BP as u16,

        "cs" => return Register::CS as u16,
        "ds" => return Register::DS as u16,
        "ss" => return Register::SS as u16,
        "es" => return Register::ES as u16,

        "c" => return Segment::C as u16,
        "d" => return Segment::D as u16,
        "s" => return Segment::S as u16,
        "e" => return Segment::E as u16,
        _ => {},
    }

    if param_clean.is_empty() {
        return 0;
    }

    else if param_clean.starts_with("0x")
        && param_clean.len() > 2
        && param_clean[2..].chars().all(|c| c.is_ascii_hexdigit()) {
        return u16::from_str_radix(&param_clean[2..], 16).unwrap();
    }
    else if param_clean.starts_with("0b")
        && param_clean.len() > 2
        && param_clean[2..].chars().all(|c| c == '0' || c == '1') {
        return u16::from_str_radix(&param_clean[2..], 2).unwrap();
    }
    else if param_clean.chars().all(|c| c.is_ascii_digit()) {
        return param_clean.parse::<u16>().unwrap_or(0);
    }
    else {
        // TODO: Labels...
        return 0;
    }
}

pub fn convert_binary(program_tokens: &Vec<Vec<String>>, program_data: &ProgramData) -> Vec<u8> {
    let mut program: Vec<u8> = Vec::new();

    let mut address: usize = 0;

    for line_tokens in program_tokens {
        let instr_type: Instruction = get_instr_type(&line_tokens[0]);

        let param_special: u16 = if line_tokens.len() > 1 { convert_param_to_value(&line_tokens[1]) } else { 0 };

        match &instr_type.opcode {
            OpCode::ORG => {
                address = param_special as usize;
            },
            OpCode::SEGMENT => {
                address = 0;
            },
            OpCode::DB => {
                program.push(param_special as u8);
            },
            OpCode::DW => {
                program.push((&param_special >> 8) as u8);
                program.push(param_special as u8);
                //program.extend_from_slice(&param_special.to_be_bytes());
                print!("dw: {:16b}\n", &param_special);
            },
            OpCode::DEFINE => {
                // TODO: Add to Defines...
            },
            _ => {
                let mut instr_binary: u16 = (instr_type.opcode as u16) << 10;
                let mut imm: u16 = 0;

                let mut i: usize = 0;
                for token in line_tokens {
                    match &instr_type.params[i] {
                        Parmameter::OP0 => instr_binary = (convert_param_to_value(token)) << 8,
                        Parmameter::OP1 => instr_binary = (convert_param_to_value(token)) << 4,
                        Parmameter::OP2 => instr_binary = convert_param_to_value(token),
                        Parmameter::IMM => imm = convert_param_to_value(token),
                        Parmameter::NONE => break,
                    }
                    i += 1;
                }
                print!("ir: {:16b}\n", &instr_binary);
            
                //program.extend_from_slice(&instr_binary.to_be_bytes());
                program.push((&instr_binary >> 8) as u8);
                program.push(instr_binary as u8);

                address += 2;
                if instr_type.imm {
                    program.push((&imm >> 8) as u8);
                    program.push(imm as u8);
                    address += 2;
                }
            }
        }
    }

    return program;
}
