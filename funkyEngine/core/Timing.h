#pragma once
#include <SDL.h>

namespace FunkyEngine {
    class FPSLimiter {
        public:
            FPSLimiter();

            void init(float maxFPS);

            void setMaxFPS(float maxFPS);

            void beginFrame();

            // end will return the current FPS
            float end();

        private:
            float _targetFPS;
            Uint64 _startTicks;
            float _fps;
            float _maxFPS;
            float _frameTime;
            
            void calculateFPS();
    };  
};