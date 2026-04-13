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
            glm::vec2 getPosition() { return _position; }
            float getScale() { return _scale; }
            glm::mat4 getCameraMatrix() { return _cameraMatrix; }

        private:
            int _screenWidth, _screenHeight;
            bool _needsMatrixUpdate;
            float _scale;
            glm::vec2 _position;     // 2 floats same as GLS vec2
            glm::mat4 _cameraMatrix;  // 4x4 matrix
            glm::mat4 _orthoMatrix;  // Hold on to our matrix
    };
};
