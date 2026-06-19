#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // helps use orthographic projection (used to world based coord to map based)

namespace FunkyEngine {
    class Camera2D {
        public: 
            Camera2D();
            ~Camera2D();

            void init(int screenWidth, int screenHeight);

            // update cmera
            void update();

            // Take screen coordinates and convert them into world coordinates
            // helps with projectile shooting
            glm::vec2 convertScreenCoordsToWorld(glm::vec2 screenCoords);

            bool isBoxInView(const glm::vec2& position, const glm::vec2& dimensions);

            // Setters
            void setPosition(const glm::vec2& newPosition) {
                _position = newPosition;
                _needsMatrixUpdate = true;
            }
            void setScale(float newScale) {
                _scale = newScale;
                _needsMatrixUpdate = true;
            }

            // Getters
            glm::vec2 getPosition() const { return _position; }
            float getScale() const { return _scale; }
            glm::mat4 getCameraMatrix() { return _cameraMatrix; }

        private:
            int _screenWidth = 500;
            int _screenHeight = 500;
            bool _needsMatrixUpdate = true;
            float _scale = 1.0f;                            // 1.0f = normal scale, 0.5f = zoom out, 2.0f = zoom in
            glm::vec2 _position = glm::vec2(0.0f, 0.0f);    // 2 floats same as GLS vec2
            glm::mat4 _cameraMatrix = glm::mat4(1.0f);      // 4x4 matrix
            glm::mat4 _orthoMatrix = glm::mat4(1.0f);       // Hold on to our matrix
    };
};
