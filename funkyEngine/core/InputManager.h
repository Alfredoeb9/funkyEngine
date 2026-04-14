#pragma once

#include <unordered_map>

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

        private:

            std::unordered_map<unsigned int, bool> _keyMap;
    };
};