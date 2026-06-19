#include "AudioEngine.h"

namespace funkyEngine {

    AudioEngine::AudioEngine() {

    }

    AudioEngine::~AudioEngine() {

    }

    void AudioEngine::init() {
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    }

    void AudioEngine::destroy() {
        Mix_CloseAudio();
    }

};