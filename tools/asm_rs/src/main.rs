mod core;
mod files;
mod convert;
mod assemble;

fn main() {
    let program_tokens: Vec<Vec<String>> = files::read_file_tokenised("../software/tests/tester.asm");
    
    let program: Vec<u8> = assemble::assemble(&program_tokens);
    
    //dbg!(&program_tokens);
    
    for i in 0..program.len()-1 {
        print!("{}: {:08b} {:08b}\n", i, program[i], program[i + 1]);
    }
}
