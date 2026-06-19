#include "AudioEngine.h"
#include "Logger.h"

namespace funkyEngine {

    AudioEngine::AudioEngine() {

    }

    AudioEngine::~AudioEngine() {
        destroy();
    }

    /**
     * function: AudioEngine::init
     * @description: Initializes the audio system. This function should be called before any sound effects
     * or music are loaded or played. It uses the Mix_OpenAudio function from SDL_mixer to initialize the audio system with a frequency of 44100 Hz, the default audio format, 2 channels (stereo), and a chunk size of 2048 bytes.
     * @returns: void
     */
    void AudioEngine::init() {
        // Take in parameters of bitwise operations to specify which audio formats to support (MP3 and OGG in this case)
        // MTX_INIT_MP3 and MIX_INIT_OGG are flags defined in SDL_mixer.h that indicate support for MP3 and OGG audio formats, respectively
        if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == -1) {
            // Handle initialization error
            FunkyEngine::Logger::log(FunkyEngine::LogLevel::CRITICAL, "Failed to initialize audio formats." + std::string(Mix_GetError()));
            return;
        }

        // Initialize the audio system with a frequency of 44100 Hz, the default audio format, 2 channels (stereo), and a chunk size of 2048 bytes
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
            FunkyEngine::Logger::log(FunkyEngine::LogLevel::CRITICAL, "Failed to open audio device." + std::string(Mix_GetError()));
            return;
        } else {
            FunkyEngine::Logger::log(FunkyEngine::LogLevel::INFO, "Audio system initialized successfully.");
        }

        m_isInitialized = true;
    }

    /**
     * function: AudioEngine::destroy
     * @description: Shuts down the audio system and frees any resources that were allocated. This function should be called when the audio system is no longer needed, such as when the game is closing. It uses the Mix_CloseAudio function from SDL_mixer to shut down the audio system.
     * @returns: void
     */
    void AudioEngine::destroy() {
        if (m_isInitialized) {
            Mix_CloseAudio();
            m_isInitialized = false;
            Mix_Quit();
            FunkyEngine::Logger::log(FunkyEngine::LogLevel::INFO, "Audio system shut down successfully.");
        } else {
            FunkyEngine::Logger::log(FunkyEngine::LogLevel::WARNING, "Audio system is not initialized. No need to shut down.");
        }
    }

    /**
     * function: AudioEngine::loadSoundEffect
     * @description: Loads a sound effect from the specified file path. It creates a new
     * SoundEffect object and returns a pointer to it. The SoundEffect constructor will handle loading the sound effect data from the file.
     * @param filePath: The file path of the sound effect to load.
     * @returns: A pointer to the loaded SoundEffect object.
     */
    SoundEffect* AudioEngine::loadSoundEffect(const std::string& filePath) {
        return new SoundEffect(filePath.c_str());
    }

    /**
     * function: AudioEngine::loadMusic
     * @description: Loads music from the specified file path. It creates a new Music object
     * and returns a pointer to it. The Music constructor will handle loading the music data from the file.
     * @param filePath: The file path of the music to load.
     * @returns: A pointer to the loaded Music object.
     */
    Music* AudioEngine::loadMusic(const std::string& filePath) {
        return new Music(filePath.c_str());
    }

};