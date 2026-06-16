#include"ImageLoader.h"
#include "core/Errors.h"
#include "core/Logger.h"
#include "picoPNG.h"
#include "IOManager.h"

#include <vector>
#include <string>

FunkyEngine::GLTexture FunkyEngine::ImageLoader::loadPNG(std::string filePath) {
    // create texture on a stack no the efficient way
    // Fix (abstracts): find a better way
    GLTexture texture = {};

    std::vector<unsigned char> in;
    std::vector<unsigned char> out;

    unsigned long width, height;

    if (IOManager::readFileToBuffer(filePath, in) == false) {
        FunkyEngine::Logger::log(FunkyEngine::LogLevel::CRITICAL, "Failed to load PNG file to buffer: " + filePath);
        EngineErrors::fatalError("Failed to load PNG file to buffer!");
    }

    int errorCode = decodePNG(out, width, height, &(in[0]), in.size());

    if (errorCode != 0) {
        FunkyEngine::Logger::log(FunkyEngine::LogLevel::CRITICAL, "decodePNG failed with error: " + std::to_string(errorCode));
        EngineErrors::fatalError("decodePNG failed with error: " + std::to_string(errorCode));
    }

    // generate our texture
    // generate 1 texture and pointer to texture id
    glGenTextures(1, &(texture.id));
    // bind our texture and be able to upload our data to it
    glBindTexture(GL_TEXTURE_2D, texture.id);
    // Upload image data to the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

    // how to treat the texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);     // MIPMAP is when we make our texture and progressively make it smaller (helps the pixels scale correctling)

    // Apply mipmap
    glGenerateMipmap(GL_TEXTURE_2D);

    // Unbind
    glBindTexture(GL_TEXTURE_2D, 0);

    texture.width = width;
    texture.height = height;

    return texture;
}
