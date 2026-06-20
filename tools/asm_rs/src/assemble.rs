use crate::core;
use crate::convert;

use core::ProgramData;

pub fn assemble(program_tokens: &Vec<Vec<String>>) -> Vec<u8> {
    let data: ProgramData = preprocess(&program_tokens);
    let program: Vec<u8> = convert::convert_binary(&program_tokens, &data);

    return program;
}

fn preprocess(program_tokens: &Vec<Vec<String>>) -> ProgramData {
    let mut data: ProgramData = ProgramData { labels: Vec::new() };

    // TODO: Get Labels with addresses

    return data;
}
