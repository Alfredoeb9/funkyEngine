#pragma once

#include <GL/glew.h>

// Unsigned char is a single byte instead of 4 bytes

namespace FunkyEngine {
    /*
        Defines exactly what information makes up one of the corners 
        in the engine

        Vertex: represents a single "corner" or point of a 2D or 3D shape
                OpenGL needs to know the data for each corner

        1. Data Layout (memory packing)
            - Mirrors the memory layout OpenGL expects
            - When an array of these vertices is sent to the GPU (Vertex Buffer Object, VBO)
                OpenGL reads it as single contiguous block of memory
                with 20 consecutive bytes
            * Position: 8 byts (float x, float y)
            * ColorRGBA: 4 bytes (GLubyte r, g, b, a)
            * UV: 8 bytes (float u, float v)
    */
    struct Vertex {
        // Create a struct called Position and a instance called position
        // Inside of another struct or class, it is called composition
        // (x, y): Tells the GPU where this corner is on the screen
        struct Position {
            float x;
            float y;
        } position;

        // float position[2];              // float: 4 bytes, an array[2]: 4 bytes. For a total of 8 bytes
        
        /*
            (r, g, b, a): Tells the GPU what colorRGBA tint this corner has
            GLubyte uses 1 byte per colorRGBA insead of 4 bytes
        */
        struct ColorRGBA {
            ColorRGBA() : r(0), g(0), b(0), a(255) {} // Default constructor to initialize color to opaque black
            ColorRGBA(GLubyte r, GLubyte g, GLubyte b, GLubyte a) : r(r), g(g), b(b), a(a) {} // Parameterized constructor for easy color initialization
            GLubyte r;
            GLubyte g;
            GLubyte b;
            GLubyte a;
        } color;
        
        // GLubyte colorRGBA[4];         // 4 bytes, 1 for each Red, Green, Blue, and alpha - 4 bytes in continuous memory


        /*
            UV: (u, v): Texture coordinates. Tell the GPU which part of the 
                image/texture should be pinned to the specific corner (between 0.0 and 1.0)
        */
        struct UV {
            float u;
            float v;
        } uv;
        
        // float uv[2]      // float: 4 bytes, an array[2]: 4 bytes. Total of 8 bytes

        /*
            Pass in: float x, float y

            - sets the position to the x and y
        */
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
}
