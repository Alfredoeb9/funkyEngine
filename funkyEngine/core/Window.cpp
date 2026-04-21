#include "Window.h"
#include <iostream>
#include <string>

FunkyEngine::Window::Window() {

}

FunkyEngine::Window::~Window() {

}

int FunkyEngine::Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags) {
    Uint32 flags = SDL_WINDOW_OPENGL;

    // Bit wise operations
    // if user passed in invisible then 
    // add on SDL_WINDOW_HIDDEN INTO OUR FLAG
    if (currentFlags & INVISIBLE) {
        flags |= SDL_WINDOW_HIDDEN;
    }
    if (currentFlags & FULLSCREEN) {
        flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }
    if (currentFlags & BORDERLESS) {
        flags |= SDL_WINDOW_BORDERLESS;
    }
    
    _sdlWindow = SDL_CreateWindow(windowName.c_str(), 
                              SDL_WINDOWPOS_CENTERED, 
                              SDL_WINDOWPOS_CENTERED, 
                              screenWidth, 
                              screenHeight, 
                              flags);

    // Store all OpenGL context to the window
    SDL_GLContext glContext = SDL_GL_CreateContext(_sdlWindow);
    
    if (glContext == nullptr) {
        std::cerr << "SDL_GL context could not be created" << SDL_GetError() << std::endl;
        return false;
    }

    // Check OpenGL version
    std::printf("***   OpenGL Version: %s   ***", glGetString(GL_VERSION));

    // Enale VSync (1 = VSync on, 0, VSync off, -1 = Adapative VSync)
    // This will naturally lock to your monitor refresh rate (60 FPS, 144 FPS)
    SDL_GL_SetSwapInterval(1);

    // Initialize GLEW right after context creation
    GLenum error = glewInit();
    if (error != GLEW_OK) {
        std::cerr << "Could not initialize GLEW! " << glewGetErrorString(error) << std::endl;
        return false;
    }

    // Background colorRGBA
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

    // Enable alpha blend (transprancy)
    glEnable(GL_BLEND);
    // What kind of blend to use
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return 0;
}

void FunkyEngine::Window::swapBuffer() {
    SDL_GL_SwapWindow(_sdlWindow);
}