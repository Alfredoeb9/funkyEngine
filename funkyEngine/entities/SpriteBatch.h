#pragma once

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Vertex.h"

struct TilePos {
    int x, y;
    bool operator==(const TilePos& other) const {
        return x == other.x && y == other.y;
    }
};

namespace std {
    template <>
    struct hash<TilePos> {
        std::size_t operator()(const TilePos& k) const {
            return std::hash<int>()(k.x) ^ (std::hash<int>()(k.y) << 1);
        }
    };
};

namespace FunkyEngine {
    enum class GlyphSortType {
        NONE,
        FRONT_TO_BACK,
        BACK_TO_FRONT,
        TEXTURE
    };
    // Single sprite
    // 6 vertices for a sprite
    // Type of texture to use
    // The depth of the texture
    // We want to sort glyphs by texture bc each batch we draw has to have the same texture
    struct Glyph {
        GLuint texture;
        float depth;

        Vertex topLeft;
        Vertex bottomLeft;
        Vertex topRight;
        Vertex bottomRight;
    };

    /*
        Store the specific offset into the Vertex Buffer Object (_vbo)

            * So then if we wan to start at a certain point in the _vbo, we dont need a separate _vbo for each 
                of our different draw calls we have to make
    */
    class RenderBatch {
        public:
            RenderBatch(GLuint Offset, GLuint NumVertices, GLuint Texture) : offset(Offset), numVertices(NumVertices), texture(Texture) {

            };
            ~RenderBatch() {};

            GLuint offset;
            GLuint numVertices;
            GLuint texture;

            
    };

    // Can be multiple batches when we call draw
    class SpriteBatch {
        public:
            SpriteBatch();
            ~SpriteBatch();

            void init();

            // Get our sprites ready for drawing
            void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
            // Sort all batches
            void end();

            // Add to batch
            void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Vertex::ColorRGBA& colorRGBA );

            // Render and actually draw
            void renderBatch();

            void addGlyphAt(int x, int y, const glm::vec4& uvRect, GLuint texture, float depth, const Vertex::ColorRGBA& colorRGBA);
            void removeGlyphAt(int x, int y);
            
        private:
            GLuint _vbo;
            GLuint _vao;

            std::vector<Glyph*> _glyphs;
            std::unordered_map<TilePos, Glyph*> _tileGlyphs;
            GlyphSortType _sortType;
            std::vector<RenderBatch> _renderBatches;

            static bool compareFrontToBack(Glyph* a, Glyph* b);
            static bool compareBackToFront(Glyph* a, Glyph* b);
            static bool compareTexture(Glyph* a, Glyph* b);

            void createRenderBatches();
            void createVertexArray();
            void sortGlyphs();
    };
};
