#include "core.h"
#include "compiler.h"
#include "assembler.h"
#include "entry.h"

#include <fstream>

int main(int argc, char* argv[]) {
    Args args = GetPaths(argc, argv);

#ifdef DEBUG
    args.src = "../../programs/src/os/bootloader.c";
#endif

    std::vector<std::vector<std::string>> lines = LoadFile(args.src);

    if (lines.size() == 0) {
        return 1;
    }

    std::vector<std::string> program = CreateProgram(lines, args);
    std::vector<char> binProgram = AssembleLines(program);

    std::ofstream asmFile(args.asmb);
    std::ostream_iterator<std::string> asmFileIterator(asmFile, "\n");
    std::copy(std::begin(program), std::end(program), asmFileIterator);
    asmFile.close();

    std::ofstream outFile(args.out, std::ios::out | std::ios::binary);
    outFile.write(&binProgram[0], binProgram.size() * sizeof(char));
    outFile.close();

    return 0;
}