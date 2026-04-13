#pragma once
#include "TextureCache.h"
#include <string>

// Static class because only one instance
class ResourceManager {
    public:
        ResourceManager();
        ~ResourceManager();

        static GLTexture getTexture(std::string texturePath);

        // static GLuint boundTexture;

    private:
        static TextureCache _textureCache;
};