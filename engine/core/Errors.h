#pragma once

#include <string>

// A namespace or static class is common for global utilities
namespace EngineErrors {
    // Call this when a fatal error occurs (like a shader failing to compile)
    // It should print the error and exit the game.
    void fatalError(const std::string& errorString);
    
    // Call this for non-fatal warnings
    void warning(const std::string& warningString);
}
