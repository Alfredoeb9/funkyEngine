#pragma once

#include <unordered_map>
#include <glm/glm.hpp>

namespace FunkyEngine {
    class InputManager {
        public:
            InputManager();
            ~InputManager();

            // Fix (abstracts): wrap all of the STL keys in own enum class
            //                  to give the ability to pick the bindings myself incase we use GLFW insead of SDL
            void pressKey(unsigned int keyID);
            void releaseKey(unsigned int keyID);
            bool isKeyPressed(unsigned int keyID);

            // Setter
            void setMouseCoords(float x, float y);

            // Getters
            glm::vec2 getMouseCoords() const { return _mouseCoords; }

        private:

            std::unordered_map<unsigned int, bool> _keyMap;
            glm::vec2 _mouseCoords;
    };
};