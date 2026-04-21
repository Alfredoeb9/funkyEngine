#include "SpriteBatch.h"
#include <algorithm>
#include <iostream>
#include <vector>

FunkyEngine::SpriteBatch::SpriteBatch() : _vbo(0), _vao(0) {

}

FunkyEngine::SpriteBatch::~SpriteBatch() {

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

    // Delete all allocated glyphs
    for (int i = 0; i < _glyphs.size(); i++) {
        delete _glyphs[i];
    }
    
    _glyphs.clear();
}
 
/*
    Sort the glyphs and generate batches form the sorted glyphs
*/
void FunkyEngine::SpriteBatch::end() {
    sortGlyphs();
    createRenderBatches();
}

// Add to batch
void FunkyEngine::SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Vertex::ColorRGBA& colorRGBA ) {
    Glyph* newGlyph = new Glyph;

    newGlyph->texture = texture;
    newGlyph->depth = depth;

    // Vertices
    newGlyph->topLeft.color = colorRGBA;
    newGlyph->topLeft.setPosition(destRect.x, destRect.y + destRect.w);    // the y element of is in uvRect.w or uvRect[3]
    newGlyph->topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

    newGlyph->bottomLeft.color = colorRGBA;
    newGlyph->bottomLeft.setPosition(destRect.x, destRect.y); 
    newGlyph->bottomLeft.setUV(uvRect.x, uvRect.y);

    newGlyph->bottomRight.color = colorRGBA;
    newGlyph->bottomRight.setPosition(destRect.x + destRect.z, destRect.y); 
    newGlyph->bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

    newGlyph->topRight.color = colorRGBA;
    newGlyph->topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);    // the y element of is in uvRect.w or uvRect[3]
    newGlyph->topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

    _glyphs.push_back(newGlyph);
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

/*
    Create batches
        * Loop through our array of glyphs for each glyph add it to a batch 
            and if we get a glyph with a new texture we need to make a new batch
*/
void FunkyEngine::SpriteBatch::createRenderBatches() {
    std::vector<Vertex> vertices;

    // Tell our vector how much we think our size is ahead of time
    // Remember vectors can allocate more and take away
    vertices.resize(_glyphs.size() * 6);

    // No batches to create
    if (_glyphs.empty()) return;

    int offset = 0;
    int currentVertex = 0;

    // Makes a intermediate batch on the stack
    // RenderBatch myBatch(0, 6, _glyphs[0]->texture);
    // push_back() makes a copy of the batch into _renderBatches
    _renderBatches.emplace_back(offset, 6, _glyphs[0]->texture);

    vertices[currentVertex] = _glyphs[0]->topLeft;
    currentVertex++;    // Continue to the next vertex element

    vertices[currentVertex] = _glyphs[0]->bottomLeft;
    currentVertex++;    // Continue to the next vertex element

    vertices[currentVertex] = _glyphs[0]->bottomRight;
    currentVertex++;    // Continue to the next vertex element

    vertices[currentVertex] = _glyphs[0]->bottomRight;
    currentVertex++;    // Continue to the next vertex element

    vertices[currentVertex] = _glyphs[0]->topRight;
    currentVertex++;    // Continue to the next vertex element

    vertices[currentVertex] = _glyphs[0]->topLeft;
    currentVertex++;    // Continue to the next vertex element

    offset += 6;

    // Loop through the rest of the glyphs and emplace a new render back
    for (int currentGlyph = 1; currentGlyph < _glyphs.size(); currentGlyph++) {
        // If current texture is different from previous texture then emplace a new render back
        // else increase the size of the current renderBatch
        if (_glyphs[currentGlyph]->texture != _glyphs[currentGlyph - 1]->texture) {
            _renderBatches.emplace_back(offset, 6, _glyphs[currentGlyph]->texture);
        } else {
            _renderBatches.back().numVertices += 6;     // back() to get last element and add 6 more vertices
        }

        vertices[currentVertex] = _glyphs[currentGlyph]->topLeft;
        currentVertex++;    // Continue to the next vertex element

        vertices[currentVertex] = _glyphs[currentGlyph]->bottomLeft;
        currentVertex++;    // Continue to the next vertex element

        vertices[currentVertex] = _glyphs[currentGlyph]->bottomRight;
        currentVertex++;    // Continue to the next vertex element

        vertices[currentVertex] = _glyphs[currentGlyph]->bottomRight;
        currentVertex++;    // Continue to the next vertex element

        vertices[currentVertex] = _glyphs[currentGlyph]->topRight;
        currentVertex++;    // Continue to the next vertex element

        vertices[currentVertex] = _glyphs[currentGlyph]->topLeft;
        currentVertex++;    // Continue to the next vertex element

        offset += 6;
    }

    // Bind and Upload our vbo
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    // This will orphan the buffer
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
    // Upload the data
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

    // Clean the buffer
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
        std::stable_sort(_glyphs.begin(), _glyphs.end(), compareBackToFront);
        return;
    }

    if (_sortType == GlyphSortType::FRONT_TO_BACK) {
        std::stable_sort(_glyphs.begin(), _glyphs.end(), compareFrontToBack);
        return;
    }

    if (_sortType == GlyphSortType::TEXTURE) {
        std::stable_sort(_glyphs.begin(), _glyphs.end(), compareTexture);
        return;
    }

    if (_sortType == GlyphSortType::NONE) return;
}

/*
    Return true if the value of A's depth is < the value of B's depth
*/
bool FunkyEngine::SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b) {
    return (a->depth < b->depth);
}

/*
    Return true if the value of A's depth is > the value of B's depth
*/
bool FunkyEngine::SpriteBatch::compareBackToFront(Glyph* a, Glyph* b) {
    return (a->depth > b->depth);
}

/*
    Return true if the value of A's texture is < the value of B's texture
    * Helps put all texures into one batch if they are all the same
    * Keeps us from using more batches then we need.
*/
bool FunkyEngine::SpriteBatch::compareTexture(Glyph* a, Glyph* b) {
    return (a->texture < b->texture);
}