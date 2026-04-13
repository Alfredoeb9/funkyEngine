#pragma once
#include "GLTexture.h"

#include <string>

// Loads images nto GLTextures
class ImageLoader {
    public:
        static GLTexture loadPNG(std::string filePath);

};