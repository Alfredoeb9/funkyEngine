#pragma once
#include "GLTexture.h"

#include <string>

namespace FunkyEngine {
    // Loads images nto GLTextures
    class ImageLoader {
        public:
            static GLTexture loadPNG(std::string filePath);

    };
}
