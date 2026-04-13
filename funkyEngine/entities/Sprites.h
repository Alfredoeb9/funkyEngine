#pragma once
#include <GL/glew.h>
#include "graphics/GLTexture.h"

#include <string>

class Sprite {
    public:
        Sprite();
        ~Sprite();

        void init(float x, float y, float width, float height, std::string texturePath);

        void draw();

    private:
        float _x;
        float _y;
        float _width;
        float _height;
        GLuint _vboID;  // GLunsigned int guaranteed to be 32 bits (unsigned int is not guaranteed to be 32 bits)
        
        // Store instance of a texture load into memory until the program exist
        GLTexture _texture;

};