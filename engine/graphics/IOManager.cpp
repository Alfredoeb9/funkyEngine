#include "IOManager.h"

#include <fstream>

bool IOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer) {
    // read file in binary format (receieve exact contents of the file)
    std::ifstream file(filePath, std::ios::binary);

    if (file.fail()) {
        perror(filePath.c_str());
        return false;
    }

    // Put our cursor seek to the end of the file
    file.seekg(0, std::ios::end);
    // Get the file size
    int fileSize = file.tellg();
    // go back to the beginning of the file
    file.seekg(0, std::ios::beg);

    // Reduce the file size by any eader bytes that might be present
    fileSize -= file.tellg();

    // resize buffer to correct size
    buffer.resize(fileSize);
    // get the first element in the buffer and what is the memory to that
    // pretend its a buffer of chars not going to phyiscally change the bytes
    file.read((char *)&(buffer[0]), fileSize);
    file.close();

    return true;
}