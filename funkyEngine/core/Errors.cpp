#include "Errors.h"
#include <cstdlib>
#include <SDL.h>
#include <string>

#include<iostream>

namespace EngineErrors {
    void fatalError(const std::string& errorString) {
        std::cerr << "FATAL ERROR: " << errorString << std::endl;
        std::cout << "Enter any key to quit...";
        int tmp;
        std::cin >> tmp;
        
        SDL_Quit();
        exit(1); // Exits the C++ application immediately
    }

    void warning(const std::string& warningString) {
        std::cout << "WARNING: " << warningString << std::endl;
    }
}