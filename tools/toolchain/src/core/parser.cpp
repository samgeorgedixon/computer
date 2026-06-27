#include "parser.h"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include "core.h"

std::vector<std::vector<std::string>> ParseASMFile(std::string srcPath) {
    std::vector<std::vector<std::string>> lines;

    std::ifstream srcFile(srcPath);

    if (!srcFile.is_open()) {
        printf("Unable to open src.asm file: %s\n", srcPath.c_str());

        srcFile.close();
        return {};
    }

    std::string line;
    std::string str;
    std::vector<std::string> lineTokens;

    while (getline(srcFile, line)) {
        line = Trim(line);

        if (line.empty()) {
            continue;
        }

        for (int i = 0; i < line.size(); i++) {
            if (line[i] == ' ' || line[i] == ',' || line[i] == '\t' || line[i] == '[' || line[i] == ']') {
                if (str.empty()) {
                    continue;
                }
                lineTokens.push_back(str);
                str = "";
            }
            else if (line[i] == '%' || line[i] == ':') {
                if (!str.empty()) {
                    lineTokens.push_back(str);
                    str = "";
                }
                lineTokens.push_back({ line[i] });
            }
            else if (line[i] == '/' && line[i + 1] == '/') {
                if (!str.empty()) {
                    lineTokens.push_back(str);
                    str = "";
                }
                if (!lineTokens.empty()) {
                    lines.push_back(lineTokens);
                    lineTokens.clear();
                }
                break;
            }
            else if (line[i] == '"' || line[i] == '\'') {
                if (!str.empty()) {
                    lineTokens.push_back(str);
                    str = "";
                }
                lineTokens.push_back({ line[i] });
                i++;

                while ((line[i] != '"' && line[i] != '\'') || line[i - 1] != '\\') {
                    str.push_back(line[i]);
                    i++;
                }
                lineTokens.push_back(str);
                str = "";

                lineTokens.push_back({ line[i] });
            }
            else {
                str.push_back(line[i]);
            }
        }

        if (!str.empty()) {
            lineTokens.push_back(str);
            str = "";
        }
        if (!lineTokens.empty()) {
            lines.push_back(lineTokens);
            lineTokens.clear();
        }
    }
    srcFile.close();

    return lines;
}

std::vector<std::vector<std::string>> ParseCFile(std::string srcPath) {
    std::vector<std::vector<std::string>> lines;

    std::ifstream srcFile(srcPath, std::ios::binary);

    srcFile.seekg(0, std::ios::end);
    size_t size = srcFile.tellg();
    srcFile.seekg(0, std::ios::beg);

    std::string src(size, '\0');
    srcFile.read(src.data(), size);
    srcFile.close();

    src = Trim(src);

    if (!srcFile.is_open()) {
        printf("Unable to open src.c file: %s\n", srcPath.c_str());

        srcFile.close();
        return {};
    }

    std::string str;
    std::vector<std::string> tokens;

    bool preDirectiveLine = false;

    for (int i = 0; i < src.size(); i++) {
        if (src[i] == ' ' || src[i] == ',' || src[i] == '\t') {
            if (str.empty()) {
                continue;
            }
            tokens.push_back(str);
            str = "";
        }
        else if (src[i] == '/' && src[i + 1] == '/') {
            if (!str.empty()) {
                tokens.push_back(str);
                str = "";
            }
            if (!tokens.empty()) {
                lines.push_back(tokens);
                tokens.clear();
            }
            while (src[i] != '\n') { i++; }
        }
        else if (src[i] == '#' && !preDirectiveLine) {
            if (!str.empty()) {
                tokens.push_back(str);
                str = "";
            }
            if (!tokens.empty()) {
                lines.push_back(tokens);
                tokens.clear();
            }
            preDirectiveLine = true;
        }
        else if (src[i] == '\n') {
            if (preDirectiveLine) {
                lines.push_back(tokens);
                tokens.clear();
                preDirectiveLine = false;
            }
            continue;
        }
        else if (src[i] == '\r') {
            continue;
        }
        else if (src[i] == ';') {
            if (!str.empty()) {
                tokens.push_back(str);
                str = "";
            }
            if (!tokens.empty() && !preDirectiveLine) {
                lines.push_back(tokens);
                tokens.clear();
            }
        }
        else if (src[i] == '{' || src[i] == '}') {
            if (!str.empty()) {
                tokens.push_back(str);
                str = "";
            }
            if (!tokens.empty() && !preDirectiveLine) {
                lines.push_back(tokens);
                tokens.clear();
            }
            lines.push_back({ std::to_string(src[i]) });
        }
        else if (src[i] == '(' || src[i] == ')' || src[i] == '[' || src[i] == ']') {
            if (!str.empty()) {
                tokens.push_back(str);
                str = "";
            }
            tokens.push_back(std::to_string(src[i]));
        }
        else if (src[i] == '"' || src[i] == '\'') {
            if (!str.empty()) {
                tokens.push_back(str);
                str = "";
            }
            tokens.push_back(std::to_string(src[i]));
            i++;

            while ((src[i] != '"' && src[i] != '\'') || src[i - 1] != '\\') {
                str.push_back(src[i]);
                i++;
            }
            tokens.push_back(str);
            str = "";

            tokens.push_back(std::to_string(src[i]));
        }
        else {
            str.push_back(src[i]);
        }
    }

    if (!str.empty()) {
        tokens.push_back(str);
    }
    if (!tokens.empty()) {
        lines.push_back(tokens);
    }

    return lines;
}
