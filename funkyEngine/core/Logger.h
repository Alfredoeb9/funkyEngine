#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <mutex>

namespace FunkyEngine {
    /**
     * enum: LogLevel
     * @description:
     * - Defines different log levels for categorizing log messages, such as INFO, DEBUG, WARNING, RENDER_ERROR, and CRITICAL.
     * - This allows for better organization and filtering of log output based on severity or type.
     * - Enums turns log levels into readable strings for easier debugging and log analysis.
     */
    enum class LogLevel {
        INFO,
        DEBUG,
        WARNING,
        RENDER_ERROR,
        CRITICAL
    };

    /**
     * class: Logger
     * @description:
     * - Provides a logging utility for the game, allowing for logging messages with different severity levels to a file.
     * - Uses a mutex to ensure thread safety when writing to the log file, preventing race conditions in multi-threaded environments.
     * - The init() function initializes the log file, and the shutdown() function closes it properly. The log() function is used to write messages to the log file with a specified log level.
     * - This class helps with debugging and monitoring the game's behavior by providing a centralized logging mechanism
     */
    class Logger {
        private:
            static std::ofstream _logFile;
            static std::mutex _logMutex;
            static bool _isInitialized;

            static std::string getLevelString(LogLevel level);

        public:
            static void init(const std::string& filepath = "game.log");
            static void shutdown();

            // Variadic template for logging or clean concatenated formatting
            static void log(LogLevel level, const std::string& message);

        
    };
}