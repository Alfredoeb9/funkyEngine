#include "FunkyEngine.h"
#include <iostream>
#include <SDL.h>
#include <GL/glew.h>

int FunkyEngine::init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL Initialization Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Tell SDL we want to double buffer (we have 2 windows)
    // 1: we draw too and the other we clear
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    return 0;
}