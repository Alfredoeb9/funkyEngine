#pragma once

#include <string>
#include <vector>

namespace FunkyEngine {
    class IOManager {
        public:
            static bool readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer);
    };
}
