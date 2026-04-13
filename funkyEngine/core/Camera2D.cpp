#include "Camera2D.h"

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