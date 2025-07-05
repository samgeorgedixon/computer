//#include <svdpi.h>

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

std::vector<char> romFile;

extern "C" uint32_t LoadROMFile(const char* filePath) {
    std::ifstream file(filePath, std::ios::binary);

    if (!file.is_open()) {
        std::cout << "Unable to Open ROM File: " << filePath << "\n";
        file.close();
        return 0;
    }

    std::vector<char> buffer(std::istreambuf_iterator<char>(file), {});
    romFile = buffer;

    file.close();

    return romFile.size();
}

extern "C" char GetROMFileByte(uint32_t index) {
    if (index >= romFile.size()) {
        return 0;
    }

    return romFile[index];
}

extern "C" void CloseROMFile() {
    romFile.clear();

    return;
}
