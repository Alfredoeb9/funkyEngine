#pragma once
#include <map>
#include "graphics/GLTexture.h"

#include <string>

namespace FunkyEngine {
    // Not a static class since we will want multple instances
    class TextureCache {
        public:
            TextureCache();
            ~TextureCache();

            GLTexture getTexture(std::string texturePath);

        private:
            std::map<std::string, GLTexture> _textureMap;   // "key": value pair
    };
}
