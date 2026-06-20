#include "AudioEngine.h"
#include "Logger.h"

namespace funkyEngine {
    
    /**
     * function: SoundEffect::play
     * @description: Plays the sound effect. The loops parameter specifies how many times to loop
     * the sound effect. A value of 0 means play once, -1 means loop indefinitely, and any positive value means loop that many times. It uses the Mix_PlayChannel function from SDL_mixer to play the sound effect on the first available channel.
     * @param loops: The number of times to loop the sound effect.
     * @returns: void
     */
    void SoundEffect::play(int loops) {
        // Check if the sound effect is loaded before attempting to play it
        // We can store the variable of Mix_PlayChannel 
        if ( Mix_PlayChannel(-1, m_chunk, loops)) {
           FunkyEngine::Logger::log(FunkyEngine::LogLevel::CRITICAL, "Failed to play sound effect: " + std::string(Mix_GetError()));
        }
    }

    /**
     * function: Music::play
     * @description: Plays the music. The loops parameter specifies how many times to loop the music.
     * A value of -1 means loop indefinitely, and any positive value means loop that many times. It uses the Mix_PlayMusic function from SDL_mixer to play the music.
     * @param loops: The number of times to loop the music.
     * @returns: void
     */
    void Music::play(int loops) {
        // Check if the music is loaded before attempting to play it
        if (Mix_PlayMusic(m_music, loops) == -1) {
            FunkyEngine::Logger::log(FunkyEngine::LogLevel::CRITICAL, "Failed to play music: " + std::string(Mix_GetError()));
        }
    }

    /**
     * function: Music::pause
     * @description: Pauses the currently playing music. If no music is playing, this
     * function does nothing. It uses the Mix_PauseMusic function from SDL_mixer to pause the music.
     * @returns: void
     */
    void Music::pause() {
        // Pause whatever music is currently playing. If no music is playing, this function does nothing.
        Mix_PauseMusic();
    }

    /**
     * function: Music::resume
     * @description: Resumes the currently paused music. If no music is paused, this
     * function does nothing. It uses the Mix_ResumeMusic function from SDL_mixer to resume the music.
     * @returns: void
     */
    void Music::resume() {
        // Resume whatever music is currently paused. If no music is paused, this function does nothing.
        Mix_ResumeMusic();
    }

    /**
     * function: Music::stop
     * @description: Stops the currently playing music. If no music is playing, this
     * function does nothing. It uses the Mix_HaltMusic function from SDL_mixer to stop the music.
     * @returns: void
     */
    void Music::stop() {
        // Stop whatever music is currently playing. If no music is playing, this function does nothing.
        Mix_HaltMusic();
    }

    /**
     * function: AudioEngine::AudioEngine
     * @description: Constructor for the AudioEngine class. Initializes the audio system.
     * @returns: void
     */
    AudioEngine::AudioEngine() {
        // Empty
    }

    /**
     * function: AudioEngine::~AudioEngine
     * @description: Destructor for the AudioEngine class. Cleans up the audio system.
     * @returns: void
     */
    AudioEngine::~AudioEngine() {
        destroy();

        // Clean up cached sound effects
        for (auto& pair : m_soundEffectCache) {
            delete pair.second;
        }
        m_soundEffectCache.clear();

        // Clean up cached music
        for (auto& pair : m_musicCache) {
            delete pair.second;
        }
        m_musicCache.clear();
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
        auto it = m_soundEffectCache.find(filePath);

        // The effect we are returning to the caller, which will be either a new sound effect that we loaded from the file or an existing sound effect that we found in the cache. We initialize it to nullptr for now, and we will assign it a value later in the function.
        SoundEffect* soundEffect = nullptr;

        // Failed to find the sound effect in the cache, so we need to load it and add it to the cache
        if (it != m_soundEffectCache.end()) {
            FunkyEngine::Logger::log(FunkyEngine::LogLevel::INFO, "Sound effect not found in cache.");
            
            // Load the sound effect using Mix_LoadWAV from SDL_mixer
            // Mix_LoadWAV is a function that loads a sound effect from a WAV file and returns a pointer to a Mix_Chunk struct that contains the sound effect data. 
            // The filePath.c_str() converts the std::string filePath to a C-style string (const char*) that is required by the Mix_LoadWAV function.
            Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());

            // Check if the sound effect was loaded successfully
            if (chunk == nullptr) {
                FunkyEngine::Logger::log(FunkyEngine::LogLevel::CRITICAL, "Failed to load sound effect chunk: " + filePath + ". " + std::string(Mix_GetError()));
            }

            // funkyEngine::SoundEffect soundEffect;
            soundEffect->m_chunk = chunk;
            m_soundEffectCache[filePath] = chunk;

            
        } else {
            // Sound effect found in cache, so we can just return it
            soundEffect->m_chunk = it->second;
            FunkyEngine::Logger::log(FunkyEngine::LogLevel::INFO, "Sound effect found in cache.");
        }
        

        return soundEffect;
    }

    /**
     * function: AudioEngine::loadMusic
     * @description: Loads music from the specified file path. It creates a new Music object
     * and returns a pointer to it. The Music constructor will handle loading the music data from the file.
     * @param filePath: The file path of the music to load.
     * @returns: A pointer to the loaded Music object.
     */
    Music* AudioEngine::loadMusic(const std::string& filePath) {
        auto it = m_musicCache.find(filePath);

        // The music we are returning to the caller, which will be either a new music that we loaded from the file or an existing music that we found in the cache. We initialize it to nullptr for now, and we will assign it a value later in the function.
        Music* music = nullptr;

        // Failed to find the music in the cache, so we need to load it and add it to the cache
        if (it != m_musicCache.end()) {
            FunkyEngine::Logger::log(FunkyEngine::LogLevel::INFO, "Music not found in cache.");
            
            // Load the music using Mix_LoadMUS from SDL_mixer
            // Mix_LoadMUS is a function that loads music from a file and returns a pointer to a Mix_Music struct that contains the music data. 
            // The filePath.c_str() converts the std::string filePath to a C-style string (const char*) that is required by the Mix_LoadMUS function.
            Mix_Music* mixMusic = Mix_LoadMUS(filePath.c_str());

            // Check if the music was loaded successfully
            if (mixMusic == nullptr) {
                FunkyEngine::Logger::log(FunkyEngine::LogLevel::CRITICAL, "Failed to load music: " + filePath + ". " + std::string(Mix_GetError()));
            }

            // make sure to assign the loaded music to the Music object and add it to the cache
            music->m_music = mixMusic;
            m_musicCache[filePath] = mixMusic;

            
        } else {
            // Music found in cache, so we can just return it
            music->m_music = it->second;
            FunkyEngine::Logger::log(FunkyEngine::LogLevel::INFO, "Music found in cache.");
        }
        

        return music;
    }

};