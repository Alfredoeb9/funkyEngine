#include "Camera2D.h"
#include <glm/glm.hpp>

FunkyEngine::Camera2D::Camera2D() : _position(0.0f, 0.0f), _cameraMatrix(1.0f), _orthoMatrix(1.0f), _scale(1.0f), _needsMatrixUpdate(true), _screenWidth(500), _screenHeight(500) {

}

FunkyEngine::Camera2D::~Camera2D() {

}

void FunkyEngine::Camera2D::init(int screenWidth, int screenHeight) {
    _screenWidth = screenWidth;
    _screenHeight = screenHeight;
    // need OrthoMatrix to updated  // L: Left side of the window, Right: screenWidth
    _orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);
    
}

void FunkyEngine::Camera2D::update() {
    // if we need to change current camera matrix and return camera
    if (_needsMatrixUpdate) {
        
        // 1. Move the world opposite to the camera, and offset by half the screen to center it
        glm::vec3 translate(
            -_position.x + _screenWidth / 2.0f, 
            -_position.y + _screenHeight / 2.0f, 
            1.0f
        );

        _cameraMatrix = glm::translate(_orthoMatrix, translate);
        
        // 2. Camera Scale (Z MUST be 1.0f to preserve depth!)
        glm::vec3 scale(_scale, _scale, 1.0f);

        // 3. Multiply scale on the left. 
        // Because of the ortho matrix, this scales right from the center of the screen!
        _cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * _cameraMatrix;

        _needsMatrixUpdate = false;
    }
}

/*
    BEFORE: The window top left is (0,0) the bottom right is (1270, 720)
    Solution: It would be better if when we click the middle of the screen it is (0,0) and click left would be -1,0
                like a regulat 4 quad plane, helps with scaling
*/
glm::vec2 FunkyEngine::Camera2D::convertScreenCoordsToWorld(glm::vec2 screenCoords) {
    // Invert y direction (OpenGL) (used for our projectiles)
    screenCoords.y = _screenHeight - screenCoords.y;
    
    // Convert middle of window to (0,0)
    screenCoords -= glm::vec2(_screenWidth / 2, _screenHeight / 2);
    // Scale the coords
    screenCoords /= _scale;
    // Translate with the camera position
    screenCoords += _position;

    return screenCoords;
}

// Does the same thing as isOnScreen (not sure what i want)
bool FunkyEngine::Camera2D::isBoxInView(const glm::vec2& position, const glm::vec2& dimensions) {
    glm::vec2 scaledScreenDimensions = glm::vec2(_screenWidth, _screenHeight) / (_scale);
    
    // The minimum distance before a collisions occurs
    const float MIN_DISTANCE_X = dimensions.x / 2.0f + scaledScreenDimensions.x / 2.0f;
    const float MIN_DISTANCE_Y = dimensions.x / 2.0f + scaledScreenDimensions.y / 2.0f;

    // We'll get the center distance of a agent to the center distance of a tile
    // and get the x,y components of that to get the distance of the two
    // then compare that against the radius of the Agent to determine the collision
    glm::vec2 centerPos = position + dimensions / 2.0f;     // Get the center  pos of the player
    // Center position of the camera
    glm::vec2 centerCameraPos = _position + glm::vec2(scaledScreenDimensions.x / 2.0f, scaledScreenDimensions.y / 2.0f);
    // Vector from input to the camera
    glm::vec2 distVec = centerPos - centerCameraPos;

    float xDepth = MIN_DISTANCE_X - abs(distVec.x);           // Switch these to positive
    float yDepth = MIN_DISTANCE_Y - abs(distVec.y);           // Switch these to positive

    // If true we are colliding
    if (xDepth > 0 && yDepth > 0) {
        // There was a collision
        return true;
    }

    return false;
}