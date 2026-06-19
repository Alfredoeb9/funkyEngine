#pragma once

#include <SDL_mixer.h>

namespace funkyEngine {

    class AudioEngine {
        public:
            AudioEngine();
            ~AudioEngine();

            void init();
            void destroy();
    };

};