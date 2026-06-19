#pragma once

#include <SDL_mixer.h>
#include <string>
#include <map>

namespace funkyEngine {

    /**
     * class: SoundEffect
     * @description: This class represents a sound effect that can be played in the game. It uses the Mix_Chunk struct from SDL_mixer to store the sound data.
     * @functions:
     *  - SoundEffect(const char* filePath): Constructor that loads a sound effect from
     * the specified file path.
     * - ~SoundEffect(): Destructor that frees the sound effect data.
     *  - void play(int loops = 0): Plays the sound effect. The loops parameter specifies how many times to loop the sound effect. A value of 0 means play once, -1 means loop indefinitely, and any positive value means loop that many times.
     */
    class SoundEffect {
        private:
            // Mix_Chunk is a struct defined in SDL_mixer.h that represents a sound effect
            Mix_Chunk* m_chunk = nullptr;
        public:
            SoundEffect(const char* filePath);
            ~SoundEffect();

            void play(int loops = 0);
    };

    /**
     * class: Music
     * @description: This class represents music that can be played in the game. It uses
     * the Mix_Music struct from SDL_mixer to store the music data.
     * @functions:
     * - Music(const char* filePath): Constructor that loads music from the specified file path.
     * - ~Music(): Destructor that frees the music data.
     * - void play(int loops = -1): Plays the music. The loops parameter specifies
     * how many times to loop the music. A value of -1 means loop indefinitely, and any positive value means loop that many times.
     */
    class Music {
        private:
            // Mix_Music is a struct defined in SDL_mixer.h that represents music
            Mix_Music* m_music = nullptr;
        public:
            Music(const char* filePath);
            ~Music();

            void play(int loops = -1);
    };

    /**
     * class: AudioEngine
     * @description: This class is responsible for initializing and shutting down the audio system. It
     * provides functions to initialize the audio system and to clean up resources when the audio system is no longer needed.
     * @functions:
     * - AudioEngine(): Constructor that initializes the audio engine. This constructor does not do anything, but it can be used to create an instance of the AudioEngine class.
     * - ~AudioEngine(): Destructor that shuts down the audio engine. This destructor does not
     * - void init(): Initializes the audio system. This function should be called before any sound effects or music are loaded or played.
     * - void destroy(): Shuts down the audio system and frees any resources that were allocated. This function should be called when the audio system is no longer needed, such as when the game is closing.
     * 
     */
    class AudioEngine {
        private:
            bool m_isInitialized = false; // Flag to track if the audio system has been initialized
            
            // Caches for loaded sound effects and music to avoid loading the same file multiple times
            std::map<std::string, SoundEffect*> m_soundEffectCache;
            std::map<std::string, Music*> m_musicCache;
        public:
            AudioEngine();
            ~AudioEngine();

            void init();
            void destroy();

            // Cache for loaded sound effects and music to avoid loading the same file multiple times
            SoundEffect* loadSoundEffect(const std::string& filePath);
            Music* loadMusic(const std::string& filePath);

    };

};