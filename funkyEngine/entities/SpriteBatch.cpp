#include "SpriteBatch.h"
#include "include/Constant.h"
#include <algorithm>
#include <iostream>
#include <vector>

// Glyph constructor that sets up all parameters 
FunkyEngine::Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const FunkyEngine::Vertex::ColorRGBA& color) :
    texture(Texture),
    depth(Depth) {

    topLeft.color = color;
    topLeft.setPosition(destRect.x, destRect.y + destRect.w);
    topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

    bottomLeft.color = color;
    bottomLeft.setPosition(destRect.x, destRect.y);
    bottomLeft.setUV(uvRect.x, uvRect.y);

    bottomRight.color = color;
    bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
    bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

    topRight.color = color;
    topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
    topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
}

FunkyEngine::Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const FunkyEngine::Vertex::ColorRGBA& color, float angle) :
    texture(Texture),
    depth(Depth) {

    glm::vec2 halfDims(destRect.z / 2.0f, destRect.w / 2.0f);

    // Get points centered at origin
    glm::vec2 tl(-halfDims.x, halfDims.y);
    glm::vec2 bl(-halfDims.x, -halfDims.y);
    glm::vec2 br(halfDims.x, -halfDims.y);
    glm::vec2 tr(halfDims.x, halfDims.y);

    // Rotate the points
    tl = rotatePoint(tl, angle) + halfDims;
    bl = rotatePoint(bl, angle) + halfDims;
    br = rotatePoint(br, angle) + halfDims;
    tr = rotatePoint(tr, angle) + halfDims;

    topLeft.color = color;
    topLeft.setPosition(destRect.x + tl.x, destRect.y + tl.y);
    topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

    bottomLeft.color = color;
    bottomLeft.setPosition(destRect.x + bl.x, destRect.y + bl.y);
    bottomLeft.setUV(uvRect.x, uvRect.y);

    bottomRight.color = color;
    bottomRight.setPosition(destRect.x + br.x, destRect.y + br.y);
    bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

    topRight.color = color;
    topRight.setPosition(destRect.x + tr.x, destRect.y + tr.y);
    topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
}

glm::vec2 FunkyEngine::Glyph::rotatePoint(const glm::vec2& pos, float angle) {
    glm::vec2 newv;
    newv.x = pos.x * cos(angle) - pos.y * sin(angle);
    newv.y = pos.x * sin(angle) + pos.y * cos(angle);
    return newv;
}

FunkyEngine::SpriteBatch::SpriteBatch() : _vbo(0), _vao(0) {

}

FunkyEngine::SpriteBatch::~SpriteBatch() {
    // Empty
}

void FunkyEngine::SpriteBatch::init() {
    createVertexArray();
}

// Get our sprites ready for drawing
/*
    Setup any states we need to begin rendering
    Tell it how we want it to sort

    Default: sortType = TEXTURE
*/
void FunkyEngine::SpriteBatch::begin(GlyphSortType sortType) {
    _sortType = sortType;
    _renderBatches.clear();     // Change the size back to zero when we begin

    // This clears the vector but KEEPS the memory allocated.
    // Next frame, emplace_back will just overwrite the old memory.
    _glyphs.clear();
    _glyphPointers.clear();
    _tileGlyphs.clear();

    // Reserve enough space for the most sprites you expect.
    // This prevents reallocation and keeps pointers valid!
    if (_glyphs.capacity() < 10000) {
        _glyphs.reserve(10000);
        _glyphPointers.reserve(10000);
    }
}
 
/*
    Sort the glyphs and generate batches form the sorted glyphs
*/
void FunkyEngine::SpriteBatch::end() {
    // Set up all pointers for fast sorting
    // _glyphPointers.resize(_glyphs.size());

    // loop though _glyphs and have _glyphPointers[i] point to the refernce of _glyphs[i]
    // for (int i = 0; i < _glyphs.size(); i++) {
    //     _glyphPointers[i] = &_glyphs[i];
    // }

    // Then sort our glyphs
    sortGlyphs();
    createRenderBatches();
}

// Add to batch
void FunkyEngine::SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Vertex::ColorRGBA& colorRGBA ) {
    _glyphs.emplace_back(destRect, uvRect, texture, depth, colorRGBA);
    _glyphPointers.push_back(&_glyphs.back());                              // Safe ONLY because of reserve()
}

// Loop through all batches render and actually draw
void FunkyEngine::SpriteBatch::renderBatch() {
    glBindVertexArray(_vao);

    for (int i = 0; i < _renderBatches.size(); i++) {
        glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);

        glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
    }

    glBindVertexArray(0);
}

void FunkyEngine::SpriteBatch::addGlyphAt(int x, int y, const glm::vec4& uvRect, GLuint texture, float depth, const Vertex::ColorRGBA& colorRGBA) {
    TilePos pos{x, y};
    glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);
    // Remove existing glyph at this position if it exists
    removeGlyphAt(x, y);

    // Create the glyph at the end of the vector
    _glyphs.emplace_back(destRect, uvRect, texture, depth, colorRGBA);
    _glyphPointers.push_back(&_glyphs.back());

    // Store the index (the current last element) in the map
    _tileGlyphs[pos] = _glyphs.size() - 1;

}

// Finds the pointer in a map, locating its twin in the vector, delete the memory to prevent the leak, and then clean up both container
void FunkyEngine::SpriteBatch::removeGlyphAt(int x, int y) {
    TilePos pos{x, y};
    auto it = _tileGlyphs.find(pos);
    
    if (it != _tileGlyphs.end()) {
        int targetIndex = it->second;
        int lastIndex = (int)_glyphs.size() - 1;

        // If the one we are deleting is NOT the last one, 
        // move the last one into the gap to keep the vector packed.
        if (targetIndex < lastIndex) {
            _glyphs[targetIndex] = _glyphs.back();
            
            // CRITICAL: We moved the 'back' glyph, so we must find it 
            // in the map and update its index to targetIndex.
            // Note: This requires knowing the TilePos of the last glyph.
            // If Glyph doesn't store its own pos, we can find it via its rect:
            TilePos lastGlyphPos = { 
                (int)(_glyphs[targetIndex].bottomLeft.position.x / TILE_WIDTH), 
                (int)(_glyphs[targetIndex].bottomLeft.position.y / TILE_WIDTH) 
            };
            _tileGlyphs[lastGlyphPos] = targetIndex;
        }

        // Remove the last element and the map entry
        _glyphs.pop_back();
        _tileGlyphs.erase(it);
    }
}

/**
 * function: createRenderBatches
 * @description:
 * - This function creates render batches from the sorted glyphs. It iterates through the sorted glyph pointers, grouping them into batches based on their texture. Each batch contains a contiguous set of vertices that can be rendered together with a single draw call, optimizing rendering performance by minimizing texture binds and draw calls.
 *      The function also uploads the vertex data to the GPU using a Vertex Buffer Object (V
 * BO) for efficient rendering.
 * @return: void
 */
void FunkyEngine::SpriteBatch::createRenderBatches() {
    // Always clear previous batches before rebuilding
    _renderBatches.clear();

    if (_glyphPointers.empty()) return;

    // Prepare vertex storage
    std::vector<Vertex> vertices;
    vertices.resize(_glyphPointers.size() * 6);   // Allocates memory but doesn't "zero" it

    int offset = 0;
    int currentVertex = 0; // currentVertex

    // Initialize the first batch
    // We use . instead of -> because _glyphs[0] is an object, not a pointer
    _renderBatches.emplace_back(offset, 6, _glyphPointers[0]->texture);

    // Helper to fill vertices (avoids repeating this 6 times per glyph)
    auto fillVertices = [&](int index, int& vIdx) {
        vertices[vIdx++] = _glyphPointers[index]->topLeft;
        vertices[vIdx++] = _glyphPointers[index]->bottomLeft;
        vertices[vIdx++] = _glyphPointers[index]->bottomRight;
        vertices[vIdx++] = _glyphPointers[index]->bottomRight;
        vertices[vIdx++] = _glyphPointers[index]->topRight;
        vertices[vIdx++] = _glyphPointers[index]->topLeft;
    };

    // Fill first glyph
    fillVertices(0, currentVertex);
    offset += 6;

    // Loop through the rest of the glyphs
    for (size_t i = 1; i < _glyphPointers.size(); i++) {
        // Check if texture changed to start a new batch
        if (_glyphPointers[i]->texture != _glyphPointers[i - 1]->texture) {
            _renderBatches.emplace_back(offset, 6, _glyphPointers[i]->texture);
        } else {
            _renderBatches.back().numVertices += 6;
        }

        fillVertices(i, currentVertex);
        offset += 6;
    }

    // Upload to GPU
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    // Orphan the buffer for performance
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
    // Upload the actual data
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/*
    Type of object that encapsulates all of the state that we need to do
    render with OpenGL
*/
void FunkyEngine::SpriteBatch::createVertexArray() {
    // We only want one of these because if we have more than one than we have 
    // some kind of memory leak
    if (_vao == 0) {
        // Generate of Vertex Array Object, 1 array object
        glGenVertexArrays(1, &_vao);
    }

    // Bind means anything else to change the state will be stored in the
    // Vertex Array Object
    glBindVertexArray(_vao);
    
    if (_vbo == 0) {
        // Generate Vertex Buffer Object, 1 buffer
        glGenBuffers(1, &_vbo);
    }


    // Bind means anything else to change the state will be stored in the
    // Vertex Buffer Object
    // Adding GL_ARRAY_BUFFER: any tim there is a change in the vao then it updates our vbo
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    // send one array of positions
    glEnableVertexAttribArray(0);
    // send one array of colors
    glEnableVertexAttribArray(1);

    glEnableVertexAttribArray(2);

    // do the drawing and tell openGL where the vertex is inthe buffer ID
    // Point openGL to the start of our data
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    // colorRGBA attribute pointer
    // normailize if we want to convert rgba from 255 to 0 - 1
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    // This is the UV attribute pointer
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

    // Disable all above (save memory)
    glBindVertexArray(0);
    
}

/*
    Sorts our glyphs to prepare for batches
*/
void FunkyEngine::SpriteBatch::sortGlyphs() {
    // Stable_sort(): guarantees that two elements will remain the same order
    
    if (_sortType == GlyphSortType::BACK_TO_FRONT) {
        std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareBackToFront);
        return;
    }

    if (_sortType == GlyphSortType::FRONT_TO_BACK) {
        std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareFrontToBack);
        return;
    }

    if (_sortType == GlyphSortType::TEXTURE) {
        std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareTexture);
        return;
    }

    if (_sortType == GlyphSortType::NONE) return;
}

/*
    Return true if the value of A's depth is < the value of B's depth
*/
bool FunkyEngine::SpriteBatch::compareFrontToBack(const Glyph* a, const Glyph* b) {
    return (a->depth < b->depth);
}

/*
    Return true if the value of A's depth is > the value of B's depth
*/
bool FunkyEngine::SpriteBatch::compareBackToFront(const Glyph* a, const Glyph* b) {
    return (a->depth > b->depth);
}

/*
    Return true if the value of A's texture is < the value of B's texture
    * Helps put all texures into one batch if they are all the same
    * Keeps us from using more batches then we need.
*/
bool FunkyEngine::SpriteBatch::compareTexture(const Glyph* a, const Glyph* b) {
    return (a->texture < b->texture);
}