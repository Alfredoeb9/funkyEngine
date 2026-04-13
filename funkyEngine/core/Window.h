#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <string>

// 0x1 = 1st bit, 0x2 = 2nd bit, 0x4 = 3rd bit

namespace FunkyEngine {
    enum WindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4 };

    class Window {
        public:
            Window();
            ~Window();

            int create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags);
            void swapBuffer();
            // Getters
            int getScreenWidth() { return _screenWidth; }
            int getScreenHeight() { return _screenHeight; }

        private:
            SDL_Window* _sdlWindow;
            int _screenWidth, _screenHeight;
    };
}
