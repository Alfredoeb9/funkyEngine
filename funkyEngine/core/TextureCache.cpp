#include "TextureCache.h"
#include "Logger.h"
#include "graphics/ImageLoader.h"

#include <iostream>

FunkyEngine::TextureCache::TextureCache() {

}

FunkyEngine::TextureCache::~TextureCache() {
    
}
/**
 * function: getTexture
 * @description:
 * - This function retrieves a texture from the cache based on the provided texture path. 
 *      If the texture is not already cached, it loads the texture using the ImageLoader, stores it in the cache, and returns it. 
 *      If the texture is already cached, it simply returns the cached texture.
 * @param texturePath: The file path of the texture to retrieve.
 * @return: The GLTexture object corresponding to the requested texture.
 */
FunkyEngine::GLTexture FunkyEngine::TextureCache::getTexture(std::string texturePath) {
    // traverse the nodes looking for existing one
    // if it reaches a nullptr it going to return a iterator that 
    // points to map::end
    // else point to the element we are looing for 
    // std::map<std::string, GLTexture>::iterator 
    auto mit = _textureMap.find(texturePath);

    // Check if its not in the map
    if (mit == _textureMap.end()) {
        // Load the texture
        GLTexture newTexture = ImageLoader::loadPNG(texturePath);

        // create the key: value pair
        // std::pair<std::string, GLTexture> newPair(texturePath, newTexture); or use make_pair()
        // insert into map
        _textureMap.insert(make_pair(texturePath, newTexture));

        FunkyEngine::Logger::log(FunkyEngine::LogLevel::INFO, "Texture loaded: " + texturePath);


        return newTexture;
    }

    FunkyEngine::Logger::log(FunkyEngine::LogLevel::INFO, "Texture used from cache: " + texturePath);

    return mit->second;
}
