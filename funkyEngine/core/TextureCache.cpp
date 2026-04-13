#include "TextureCache.h"
#include "graphics/ImageLoader.h"

#include <iostream>

TextureCache::TextureCache() {

}

TextureCache::~TextureCache() {
    
}

GLTexture TextureCache::getTexture(std::string texturePath) {
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

        std::cout << "Loaded Texture!\n";


        return newTexture;
    }

    std::cout << "Used Cached Texture!\n";

    return mit->second;
}
