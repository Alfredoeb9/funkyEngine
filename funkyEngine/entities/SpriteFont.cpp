#include "SpriteFont.h"

#include "SpriteBatch.h"

#include <SDL.h>

/**
 * function: closestPow2
 * @description:
 * - This function takes an integer input and returns the closest power of 2 that is greater
 *  than or equal to the input value. It works by decrementing the input value, then repeatedly 
 *  right-shifting it until it becomes zero, while left-shifting a power of 2 variable to keep track of the closest power of 2. Finally, it returns the calculated power of 2.
 */
int closestPow2(int i) {
    i--;
    int pi = 1;
    while (i > 0) {
        i >>= 1;
        pi <<= 1;
    }
    return pi;
}

// define the maximum texture resolution (width and height) for the font atlas
#define MAX_TEXTURE_RES 4096

namespace FunkyEngine {
    /**
     * function: createRows
     * @description:
     * - This function takes an array of glyph rectangles, the number of rectangles, 
     *   the desired number of rows, padding between glyphs, and a reference to an integer for width. 
     * 
     */
    SpriteFont::SpriteFont(const char* font, int size, char cs, char ce) {
        init(font, size, cs, ce);
    }

    /**
     * function: init
     * @description:
     * - This function initializes the SpriteFont by loading a TrueType font, creating a texture atlas
     */
    void SpriteFont::init(const char* font, int size) {
        init(font, size, FIRST_PRINTABLE_CHAR, LAST_PRINTABLE_CHAR);
    }

    /**
     * function: init
     * @description:
     * - This function initializes the SpriteFont by loading a TrueType font, creating a texture
     */
    void SpriteFont::init(const char* font, int size, char cs, char ce) {
        // Initialize SDL_ttf
        if (!TTF_WasInit()) {
            TTF_Init();
        }
        // Load the font and get its height
        TTF_Font* f = TTF_OpenFont(font, size);

        // Check if the font was loaded successfully
        if (f == nullptr) {
            fprintf(stderr, "Failed to open TTF font %s\n", font);
            fflush(stderr);
            throw 281;
        }
        // Get the font height and set up the character region
        _fontHeight = TTF_FontHeight(f);
        _regStart = cs;
        _regLength = ce - cs + 1;
        int padding = size / 8;

        // First measure all the regions
        glm::ivec4* glyphRects = new glm::ivec4[_regLength];
        int i = 0, advance;

        // Loop through the specified character range and get the glyph metrics for each character, storing them in the glyphRects array. 
        // The metrics include the minimum and maximum x and y coordinates of the glyph, as well as the advance width 
        // (the horizontal distance to move the cursor after rendering the glyph). The function TTF_GlyphMetrics is used to retrieve these metrics for each character in the specified range.
        for (char c = cs; c <= ce; c++) {
            TTF_GlyphMetrics(f, c, &glyphRects[i].x, &glyphRects[i].z, &glyphRects[i].y, &glyphRects[i].w, &advance);
            glyphRects[i].z -= glyphRects[i].x;
            glyphRects[i].x = 0;
            glyphRects[i].w -= glyphRects[i].y;
            glyphRects[i].y = 0;
            i++;
        }

        // Find best partitioning of glyphs
        int rows = 1, w, h, bestWidth = 0, bestHeight = 0, area = MAX_TEXTURE_RES * MAX_TEXTURE_RES, bestRows = 0;
        std::vector<int>* bestPartition = nullptr;

        // This loop tries different numbers of rows to find the best way to pack the glyphs into a texture atlas.
        while (rows <= _regLength) {
            h = rows * (padding + _fontHeight) + padding;
            auto gr = createRows(glyphRects, _regLength, rows, padding, w);

            // Desire a power of 2 texture
            w = closestPow2(w);
            h = closestPow2(h);

            // A texture must be feasible
            if (w > MAX_TEXTURE_RES || h > MAX_TEXTURE_RES) {
                rows++;
                delete[] gr;
                continue;
            }

            // Check for minimal area
            if (area >= w * h) {
                if (bestPartition) delete[] bestPartition;
                bestPartition = gr;
                bestWidth = w;
                bestHeight = h;
                bestRows = rows;
                area = bestWidth * bestHeight;
                rows++;
            } else {
                delete[] gr;
                break;
            }
        }

        // Can a bitmap font be made?
        if (!bestPartition) {
            fprintf(stderr, "Failed to Map TTF font %s to texture. Try lowering resolution.\n", font);
            fflush(stderr);
            throw 282;
        }
        // Create the texture
        glGenTextures(1, &_texID);
        glBindTexture(GL_TEXTURE_2D, _texID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bestWidth, bestHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        // Now draw all the glyphs
        SDL_Color fg = { 255, 255, 255, 255 };
        int ly = padding;
        for (int ri = 0; ri < bestRows; ri++) {
            int lx = padding;
            for (size_t ci = 0; ci < bestPartition[ri].size(); ci++) {
                int gi = bestPartition[ri][ci];

                SDL_Surface* glyphSurface = TTF_RenderGlyph_Blended(f, (char)(cs + gi), fg);

                // Pre-multiplication occurs here
                unsigned char* sp = (unsigned char*)glyphSurface->pixels;
                int cp = glyphSurface->w * glyphSurface->h * 4;
                for (int i = 0; i < cp; i += 4) {
                    float a = sp[i + 3] / 255.0f;
                    sp[i] = (unsigned char)((float)sp[i] * a);
                    sp[i + 1] = sp[i];
                    sp[i + 2] = sp[i];
                }

                // Save glyph image and update coordinates
                glTexSubImage2D(GL_TEXTURE_2D, 0, lx, bestHeight - ly - 1 - glyphSurface->h, glyphSurface->w, glyphSurface->h, GL_BGRA, GL_UNSIGNED_BYTE, glyphSurface->pixels);
                glyphRects[gi].x = lx;
                glyphRects[gi].y = ly;
                glyphRects[gi].z = glyphSurface->w;
                glyphRects[gi].w = glyphSurface->h;

                SDL_FreeSurface(glyphSurface);
                glyphSurface = nullptr;

                lx += glyphRects[gi].z + padding;
            }
            ly += _fontHeight + padding;
        }

        // Draw the unsupported glyph
        int rs = padding - 1;
        int* pureWhiteSquare = new int[rs * rs];
        memset(pureWhiteSquare, 0xffffffff, rs * rs * sizeof(int));
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, rs, rs, GL_RGBA, GL_UNSIGNED_BYTE, pureWhiteSquare);
        delete[] pureWhiteSquare;
        pureWhiteSquare = nullptr;

        // Set some texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        // Create spriteBatch glyphs
        _glyphs = new CharGlyph[_regLength + 1];
        for (i = 0; i < _regLength; i++) {
            _glyphs[i].character = (char)(cs + i);
            _glyphs[i].size = glm::vec2(glyphRects[i].z, glyphRects[i].w);
            _glyphs[i].uvRect = glm::vec4(
                (float)glyphRects[i].x / (float)bestWidth,
                (float)glyphRects[i].y / (float)bestHeight,
                (float)glyphRects[i].z / (float)bestWidth,
                (float)glyphRects[i].w / (float)bestHeight
                );
        }
        _glyphs[_regLength].character = ' ';
        _glyphs[_regLength].size = _glyphs[0].size;
        _glyphs[_regLength].uvRect = glm::vec4(0, 0, (float)rs / (float)bestWidth, (float)rs / (float)bestHeight);

        glBindTexture(GL_TEXTURE_2D, 0);
        delete[] glyphRects;
        delete[] bestPartition;
        TTF_CloseFont(f);
    }

    void SpriteFont::dispose() {
        if (_texID != 0) {
            glDeleteTextures(1, &_texID);
            _texID = 0;
        }
        if (_glyphs) {
            delete[] _glyphs;
            _glyphs = nullptr;
        }
    }

    std::vector<int>* SpriteFont::createRows(glm::ivec4* rects, int rectsLength, int r, int padding, int& w) {
        // Blank initialize
        std::vector<int>* l = new std::vector<int>[r]();
        int* cw = new int[r]();
        for (int i = 0; i < r; i++) {
            cw[i] = padding;
        }

        // Loop through all glyphs
        for (int i = 0; i < rectsLength; i++) {
            // Find row for placement
            int ri = 0;
            for (int rii = 1; rii < r; rii++)
            if (cw[rii] < cw[ri]) ri = rii;

            // Add width to that row
            cw[ri] += rects[i].z + padding;

            // Add glyph to the row list
            l[ri].push_back(i);
        }

        // Find the max width
        w = 0;
        for (int i = 0; i < r; i++) {
            if (cw[i] > w) w = cw[i];
        }

        return l;
    }

    /**
     * function: measure
     * @description:
     * - This function calculates the dimensions of a given string of text by iterating through each character in the 
     *   string, checking for newline characters to adjust the height and width accordingly, and using the glyph metrics to calculate the total width of the text. It returns a glm::vec2 containing the calculated width and height of the text.
     * 
     */
    glm::vec2 SpriteFont::measure(const char* s) {
        glm::vec2 size(0, _fontHeight);
        float cw = 0;

        // Loop through each character in the string, checking for newline characters 
        // to adjust the height and width accordingly, and using the glyph metrics to calculate the total width of the text. It returns a glm::vec2 containing the calculated width and height of the text.
        for (int si = 0; s[si] != 0; si++) {
            char c = s[si];
            if (s[si] == '\n') {
                size.y += _fontHeight;
                if (size.x < cw)
                    size.x = cw;
                cw = 0;
            } else {
                // Check for correct glyph
                int gi = c - _regStart;
                if (gi < 0 || gi >= _regLength)
                    gi = _regLength;
                cw += _glyphs[gi].size.x;
            }
        }

        // Check for last line
        if (size.x < cw)
            size.x = cw;
        return size;
    }

    /**
     * function: draw
     * @description:
     * - This function draws text using a SpriteBatch by iterating through each character in the 
     *   input string, calculating the destination rectangle for each glyph based on the current 
     *   position and scaling, and then calling the draw method of the SpriteBatch to render each 
     *   glyph with the specified texture, depth, and tint. It also handles text justification by 
     *   adjusting the starting position of the text based on the measured width of the string and the desired justification (left, middle, or right).
     */
    void SpriteFont::draw(SpriteBatch& batch, const char* s, glm::vec2 position, glm::vec2 scaling, 
                          float depth, FunkyEngine::Vertex::ColorRGBA tint, Justification just /* = Justification::LEFT */) {
        glm::vec2 tp = position;
        // Apply justification
        if (just == Justification::MIDDLE) {
            tp.x -= measure(s).x * scaling.x / 2;
        } else if (just == Justification::RIGHT) {
            tp.x -= measure(s).x * scaling.x;
        }

        // Loop through each character in the input string, calculating the destination rectangle for each glyph based on the current
        // position and scaling, and then calling the draw method of the SpriteBatch to render each
        // glyph with the specified texture, depth, and tint. It also handles text justification by
        // adjusting the starting position of the text based on the measured width of the string and the desired justification (left, middle, or right).
        for (int si = 0; s[si] != 0; si++) {
            char c = s[si];
            if (s[si] == '\n') {
                tp.y += _fontHeight * scaling.y;
                tp.x = position.x;
            } else {
                // Check for correct glyph
                int gi = c - _regStart;
                if (gi < 0 || gi >= _regLength)
                    gi = _regLength;
                glm::vec4 destRect(tp, _glyphs[gi].size * scaling);
                batch.draw(destRect, _glyphs[gi].uvRect, _texID, depth, tint);
                tp.x += _glyphs[gi].size.x * scaling.x;
            }
        }
    }

}