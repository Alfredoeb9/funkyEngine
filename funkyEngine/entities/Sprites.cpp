#include "Sprites.h"
#include "Vertex.h"
#include "core/ResourceManager.h"

#include <cstddef>
#include <string>

FunkyEngine::Sprite::Sprite(){
    _vboID = 0;
}

FunkyEngine::Sprite::~Sprite(){

    // Always delete buffers when done
    if (_vboID != 0) {
        glDeleteBuffers(1, &_vboID);
    }
}

// Initilize the sprite VBA, x, y, width, and height are
// in the normalized device coordinate space. so, [-1, 1]
void FunkyEngine::Sprite::init(float x, float y, float width, float height, std::string texturePath) {
    _x = x;
    _y = y;
    _width = width;
    _height = height;

    // Texture cache - generate the buffer if it hasn't already been generated
    _texture = ResourceManager::getTexture(texturePath);

    // only generate this vertex buffer if it hasnt been created
    if (_vboID == 0) {
        glGenBuffers(1, &_vboID);
    }

    // construct vertex 
    // This array will hold our vertex data
    // we need 6 verticese, and each vertex has 2 floats for x and y
    Vertex vertexData[6];

    // Screen Coordinate Space
    // Top Right Corner - First triangle
    vertexData[0].setPosition(x + width, y + height);
    vertexData[0].setUV(1.0f, 1.0f);
    
    // Top Left Corner - First triangle
    vertexData[1].setPosition(x, y + height);
    vertexData[1].setUV(0.0f, 1.0f);

    // Bottom left Corner - Frist triangle
    vertexData[2].setPosition(x, y);
    vertexData[2].setUV(0.0f, 0.0f);

    // Bottom Left Corner - Second Triangle
    vertexData[3].setPosition(x, y);
    vertexData[3].setUV(0.0f, 0.0f);
    
    // Bottom Right Corner - Second triangle
    vertexData[4].setPosition(x + width, y);
    vertexData[4].setUV(1.0f, 0.0f);

    // Top RIGHT Corner - Second triangle
    vertexData[5].setPosition(x + width, y + height);
    vertexData[5].setUV(1.0f, 1.0f);

    // set all vertex colors to maganta
    for (int i = 0; i < 6; i++) {
        vertexData[i].setColor(255, 0, 255, 255);
    }

    vertexData[1].setColor(0, 0, 255, 255);

    vertexData[4].setColor(0, 255, 0, 255);
    
    // we want this buffer to be active
    glBindBuffer(GL_ARRAY_BUFFER, _vboID);

    // upload data buffer to GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    // unbind the buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void FunkyEngine::Sprite::draw() {
    // Dont want to unbind textures
    glBindTexture(GL_TEXTURE_2D, _texture.id);

    glBindBuffer(GL_ARRAY_BUFFER, _vboID);

    // send one array of positions
    glEnableVertexAttribArray(0);
    // send one array of colors
    glEnableVertexAttribArray(1);

    // do the drawing and tell openGL where the vertex is inthe buffer ID
    // Point openGL to the start of our data
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    // Color attribute pointer
    // normailize if we want to convert rgba from 255 to 0 - 1
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    // This is the UV attribute pointer
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    // Acturally draw the data
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}