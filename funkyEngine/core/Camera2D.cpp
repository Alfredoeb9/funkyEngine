#include "Camera2D.h"
#include <glm/glm.hpp>

/**
 * function: Camera2D
 * @description:
 * - The constructor for the Camera2D class initializes the camera's position, camera matrix, orthographic matrix, scale, and screen dimensions. It sets the initial position to (0, 0), initializes the camera and orthographic matrices to identity matrices, sets the scale to 1.0f, and marks that the camera matrix needs to be updated. The screen width and height are set to default values of 500 pixels each.
 * @return: void
 * 
 */
FunkyEngine::Camera2D::Camera2D() : 
    _position(0.0f, 0.0f), 
    _cameraMatrix(1.0f), 
    _orthoMatrix(1.0f), 
    _scale(1.0f), 
    _needsMatrixUpdate(true),
    _screenWidth(500), 
    _screenHeight(500) {

}

FunkyEngine::Camera2D::~Camera2D() {
    // Empty
}

/**
 * function: init
 * @description:
 * - Initializes the camera with the specified screen width and height, setting up the orthographic projection matrix based on these dimensions. The orthographic projection is used to convert world coordinates to screen coordinates, allowing for 2D rendering. 
 *      The function also sets the initial position and scale of the camera, and marks that the camera matrix needs to be updated.
 * @param screenWidth: The width of the screen in pixels.
 * @param screenHeight: The height of the screen in pixels.
 * @return: void
 */
void FunkyEngine::Camera2D::init(int screenWidth, int screenHeight) {
    _screenWidth = screenWidth;
    _screenHeight = screenHeight;
    // need OrthoMatrix to updated  // L: Left side of the window, Right: screenWidth
    _orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);
    
}

/**
 * function: update
 * @description:
 * - Updates the camera matrix if it has been marked as needing an update. This involves translating the world coordinates to align with the camera's position and applying the camera's scale.
 * @return: void
 */
void FunkyEngine::Camera2D::update() {
    // if we need to change current camera matrix and return camera
    if (_needsMatrixUpdate) {
        
        // Move the world opposite to the camera, and offset by half the screen to center it
        glm::vec3 translate(
            -_position.x + _screenWidth / 2.0f, 
            -_position.y + _screenHeight / 2.0f, 
            1.0f
        );

        _cameraMatrix = glm::translate(_orthoMatrix, translate);
        
        // Camera Scale (Z MUST be 1.0f to preserve depth!)
        glm::vec3 scale(_scale, _scale, 1.0f);

        // Multiply scale on the left. 
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
/**
 * function: convertScreenCoordsToWorld
 * @description:
 * - Converts screen coordinates to world coordinates by inverting the y-axis, translating the coordinates to center around the middle of the screen, scaling them according to the camera's scale, 
 *      and then translating them based on the camera's position. This is useful for tasks like projectile shooting, 
 *      where you need to convert mouse click positions (in screen coordinates) to corresponding positions in the game world.
 * @param screenCoords: The screen coordinates to convert, typically in pixels.
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
    const float MIN_DISTANCE_Y = dimensions.y / 2.0f + scaledScreenDimensions.y / 2.0f;

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