#include "entry.h"
#include "core.h"

Args GetPaths(int argc, char* argv[]) {
    Args args = { "", "", "", ExeType::NONE, 0 };

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] == 'o') {
            i++;
            args.out = argv[i];
        }
        else if ((argv[i][0] == '-' && argv[i][1] == 'a')) {
            i++;
            args.asmb = argv[i];
        }
        else if ((argv[i][0] == '-' && argv[i][1] == 'e')) {
            i++;
            if (Trim(argv[i]) == "ofst_data") {
                args.exeType = ExeType::OFST_DATA;
                i++;
                args.dataSegOffset = std::stoi(Trim(argv[i]));
            }
            else if (Trim(argv[i]) == "size_seg") {
                args.exeType = ExeType::SIZE_SEG;
            }
        }
        else {
            args.src = argv[i];
        }
    }

    return args;
}