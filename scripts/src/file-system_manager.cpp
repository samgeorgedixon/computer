#include <iostream>
#include <vector>
#include <fstream>
#include <bitset>
#include <cmath>

std::vector<char> drive;
std::vector<char> fileTable;

uint16_t nextSeg = 0;

struct File {
    char name[7]; // 6b plus 0
    std::string filePath;
};

std::vector<File> files = {
    { "boot  ", "bin/programs/bootloader.bin" },
    { "kernel", "bin/programs/kernel.bin" },
};

/*

"kernel"6b - 2(start seg)2b - 5(size segs)2b : 10b

*/

void AddFile(const File& file) {
    std::ifstream input(file.filePath, std::ios::binary );

    std::vector<char> fileContents(std::istreambuf_iterator<char>(input), {});
    input.close();
    
    for (int i = 0; i < 6; i++) {
        fileTable.push_back(file.name[i]);
    }
    
    uint16_t fileSegSize = std::ceil((float)fileContents.size() / (float)256);

    while (fileContents.size() % 256 != 0) {
        fileContents.push_back(0);
    }

    std::cout << "start: " << nextSeg << "\n";
    std::cout << "size: " << fileSegSize << "\n";
    
    fileTable.push_back((nextSeg & 0xff00) >> 8);
    fileTable.push_back(nextSeg & 0xff);
    
    fileTable.push_back((fileSegSize & 0xff00) >> 8);
    fileTable.push_back(fileSegSize & 0xff);
    
    drive.insert(drive.end(), fileContents.begin(), fileContents.end());

    nextSeg += fileSegSize;
}

void AddFiles() {
    for (int i = 0; i < files.size(); i++) {
        AddFile(files[i]);
    }
}

void WriteDrive(std::string filePath) {
    while (fileTable.size() < 256) {
        fileTable.push_back(0);
    }

    std::ofstream file(filePath, std::ios::out | std::ios::binary);

    file.write(&fileTable[0], fileTable.size() * sizeof(char));
    file.write(&drive[0], drive.size() * sizeof(char));

    file.close();
}

int main(int argc, char* argv[]) {
    AddFiles();

    WriteDrive("bin/drives/drive.bin");

    return 0;
}
