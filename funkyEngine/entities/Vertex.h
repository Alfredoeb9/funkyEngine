#pragma once

#include <GL/glew.h>

// Unsigned char is a single byte instead of 4 bytes

struct Vertex {
    // Create a struct called Position and a instance called position
    // Inside of another struct or class, it is called composition
    struct Position {
        float x;
        float y;
    } position;

    // float position[2];              // float: 4 bytes, an array[2]: 4 bytes. For a total of 8 bytes
    
    struct Color {
        GLubyte r;
        GLubyte g;
        GLubyte b;
        GLubyte a;
    } color;
    
    // GLubyte color[4];         // 4 bytes, 1 for each Red, Green, Blue, and alpha - 4 bytes in continuous memory


    // UV texture coordinates 
    struct UV {
        float u;
        float v;
    } uv;
    
    // float uv[2]      // float: 4 bytes, an array[2]: 4 bytes. Total of 8 bytes

    void setPosition(float x, float y) {
        position.x = x;
        position.y = y;
    }
    
    void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a ) {
        color.r = r;
        color.g = g;
        color.b = b;
        color.a = a;
    }

    void setUV(float u, float v) {
        uv.u = u;
        uv.v = v;
    }
};