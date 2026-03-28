//#include <svdpi.h>

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdint>

std::vector<char> memoryFile;

extern "C" uint32_t LoadMemoryFile(const char* filePath) {
    std::ifstream file(filePath, std::ios::binary);

    if (!file.is_open()) {
        std::cout << "Unable to Open Memory File: " << filePath << "\n";
        file.close();
        return 0;
    }

    std::vector<char> buffer(std::istreambuf_iterator<char>(file), {});
    memoryFile = buffer;

    file.close();

    return memoryFile.size();
}

extern "C" char GetMemoryFileByte(uint32_t index) {
    if (index >= memoryFile.size()) {
        return 0;
    }

    return memoryFile[index];
}

extern "C" void CloseMemoryFile() {
    memoryFile.clear();

    return;
}
