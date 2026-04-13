#include "ResourceManager.h"

FunkyEngine::ResourceManager::ResourceManager() {

}

FunkyEngine::ResourceManager::~ResourceManager() {

}

FunkyEngine::TextureCache FunkyEngine::ResourceManager::_textureCache;

// Wrapper function to getTexture
FunkyEngine::GLTexture FunkyEngine::ResourceManager::getTexture(std::string texturePath) {
    return _textureCache.getTexture(texturePath);
}
