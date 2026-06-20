use std::fs::read_to_string;

pub fn read_file_tokenised(path: &str) -> Vec<Vec<String>> {
    let mut data: Vec<Vec<String>> = Vec::new();

    for line in read_to_string(path).unwrap().lines() {
        if line.trim() == "" { continue };

        let mut line_tokens: Vec<String> = Vec::new();

        for token in line.trim().split_whitespace() {
            line_tokens.push(token.trim().to_string());
        }
        data.push(line_tokens);
    }

    return data;
}
