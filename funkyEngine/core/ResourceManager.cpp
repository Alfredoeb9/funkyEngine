#include "ResourceManager.h"

ResourceManager::ResourceManager() {

}

ResourceManager::~ResourceManager() {

}

TextureCache ResourceManager::_textureCache;

// Wrapper function to getTexture
GLTexture ResourceManager::getTexture(std::string texturePath) {
    return _textureCache.getTexture(texturePath);
}
