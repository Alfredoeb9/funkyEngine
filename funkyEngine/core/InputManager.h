#pragma once

#include <unordered_map>
#include <glm/glm.hpp>

namespace FunkyEngine {
    class InputManager {
        private:
            std::unordered_map<unsigned int, bool> _keyPressedMap; // Map to track keys that were pressed in the current frame
            std::unordered_map<unsigned int, bool> _keyMap;         // Map to track the current state of keys (pressed or not)
            glm::vec2 _mouseCoords;


        public:
            InputManager();
            ~InputManager();

            void updateKeyMaps(); // Helper function to update key maps at the end of each frame

            // Fix (abstracts): wrap all of the STL keys in own enum class
            //                  to give the ability to pick the bindings myself incase we use GLFW insead of SDL
            
            // 
            void pressKey(unsigned int keyID);

            // Tells the input manager that a key was released
            void releaseKey(unsigned int keyID);

            // Returns true if the key was pressed in the current frame
            bool isKeyPressed(unsigned int keyID);
            
            // returns true if the key is currently down (held down or just pressed)
            bool isKeyDown(unsigned int keyID);

            // Setter
            void setMouseCoords(float x, float y);

            // Getters
            glm::vec2 getMouseCoords() const { return _mouseCoords; }

        
    };
};