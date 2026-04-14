#include "Timing.h"

#include <SDL.h>

FunkyEngine::FPSLimiter::FPSLimiter() {

}

void FunkyEngine::FPSLimiter::init(float maxFPS) {
    setMaxFPS(maxFPS);
}

void FunkyEngine::FPSLimiter::setMaxFPS(float maxFPS) {
    _maxFPS = maxFPS;
}

void FunkyEngine::FPSLimiter::beginFrame() {
    _startTicks = SDL_GetTicks64();
}

// end will return the current FPS
float FunkyEngine::FPSLimiter::end() {
    calculateFPS();

    float frameTicks = SDL_GetTicks64() - _startTicks;
    static const float TARGET_FRAME_TIME = 1000.0f / _maxFPS;

    // Limit FPS to max FPS
    if (TARGET_FRAME_TIME > frameTicks) {
        SDL_Delay(TARGET_FRAME_TIME - frameTicks);
    }

    return _fps;
}

void FunkyEngine::FPSLimiter::calculateFPS() {
    // Target time per frame for 60 FPS (approx 16.66ms)
    static const float TARGET_FRAME_TIME = 1000.0f / _maxFPS;
    // num of frames we want to avg
    static const int NUM_SAMPLES = 10;
    static float frameTimes[NUM_SAMPLES];
    static int currentFrame = 0;

    // Get current ticks and previous ticks
    // Get the frequency of the high-res counter
    static Uint64 frequency = SDL_GetPerformanceFrequency();

    static Uint64 prevTicks = SDL_GetPerformanceCounter();
    Uint64 currentTicks = SDL_GetPerformanceCounter();

    // Calculate frame time in milliseconds as a float
    _frameTime = ((currentTicks - prevTicks) * 1000.0f) / frequency;
    
    // --- NEW: DELAY TO LOCK AT 60 FPS ---
    if (_frameTime < TARGET_FRAME_TIME) {
        SDL_Delay((Uint32)(TARGET_FRAME_TIME - _frameTime));
        
        // Recalculate current ticks and frame time after the delay
        // FIX: Must use GetPerformanceCounter and the frequency formula again!
        currentTicks = SDL_GetPerformanceCounter();
        _frameTime = ((currentTicks - prevTicks) * 1000.0f) / frequency;
    }
    // ------------------------------------

    // Circular Buffer Algorithm
    frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;

    // Update prevTicks to the new currentTick
    prevTicks = currentTicks;

    // The number we are averaging
    int count;

    currentFrame++;

    if (currentFrame < NUM_SAMPLES) {
        count = currentFrame;
    } else {
        count = NUM_SAMPLES;
    }

    // Calculate the Average Frame from the sample
    float frameTimeAverage = 0;
    for (int i = 0; i < count; i++) {
        frameTimeAverage += frameTimes[i];
    }

    frameTimeAverage /= count;

    if (frameTimeAverage > 0) {
        _fps = 1000.0f / frameTimeAverage;
    } else {
        _fps = 60.0f;
    }
}