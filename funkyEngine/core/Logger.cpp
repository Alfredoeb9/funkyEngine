#include "Logger.h"
#include <SDL.h>
#include <iomanip>
#include <chrono>

namespace FunkyEngine {
    // Helper function to convert LogLevel enum to string representation
    std::ofstream Logger::_logFile;
    std::mutex Logger::_logMutex;
    bool Logger::_isInitialized = false;

    /**
     * function: init
     * @description:
     * - Initializes the logger by opening the log file
     * - Uses a mutex to ensure thread safety when initializing the
     * logger, preventing race conditions in multi-threaded environments. If the logger is already initialized, it logs a warning message and returns without reinitializing.
     * The log file is opened in output mode with truncation, meaning that any existing content will be cleared when the logger is initialized. The function also logs a message indicating whether the initialization was successful or if it failed.
     * @param filePath: The path to the log file where log messages will be written. If not provided, it defaults to "game.log".
     */
    void Logger::init(const std::string& filePath) {
        // Ensure thread safety when initializing the logger
        std::lock_guard<std::mutex> lock(_logMutex);

        // Check if the logger is already initialized
        if (_isInitialized) {
            log(LogLevel::WARNING, "Logger is already initialized.");
            return;
        }
        // Open the log file in output mode with truncation (clears existing content)
        _logFile.open(filePath, std::ios::out | std::ios::trunc);
        _isInitialized = _logFile.is_open();

        // Log the initialization status
        if (_isInitialized) {
            log(LogLevel::INFO, "Logger initialized successfully.");
        } else {
            log(LogLevel::CRITICAL, "Failed to initialize logger.");
        }
    }

    /**
     * function: shutdown
     * @description:
     * - Shuts down the logger by closing the log file and marking it as uninitialized
     */
    void Logger::shutdown() {
        // Ensure thread safety when shutting down the logger
        std::lock_guard<std::mutex> lock(_logMutex);

        // Log shutdown message before closing the file
        if (_logFile.is_open()) {
            log(LogLevel::INFO, "Shutting down logger.");
            _logFile.close();
        }

        _isInitialized = false;
    }

    /**
     * function: getLevelString
     * @description:
     * - Returns the string representation of a log level
    */
    std::string Logger::getLevelString(LogLevel level) {
        switch (level) {
            case LogLevel::INFO:            return "[INFO]";
            case LogLevel::DEBUG:           return "[DEBUG]";
            case LogLevel::WARNING:         return "[WARNING]";
            case LogLevel::RENDER_ERROR:    return "[RENDER_ERROR]";
            case LogLevel::CRITICAL:        return "[CRITICAL]";
            default:                        return "[UNKNOWN]";
        }
    }

    void Logger::log(LogLevel level, const std::string& message) {
        // Ensure thread safety when logging messages
        std::lock_guard<std::mutex> lock(_logMutex);

        if (!_isInitialized) {
            std::cerr << "Logger not initialized. Message: " << message << std::endl;
            return;
        }

        // Get the current time for timestamping log messages
        // auto now = std::chrono::system_clock::now();
        // auto in_time_t = std::chrono::system_clock::to_time_t(now);
        // auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        auto now = std::chrono::system_clock::now();
        auto timeTime = std::chrono::system_clock::to_time_t(now);
        auto localTime = std::localtime(&timeTime);

        // Format: [HH:MM:SS] [LEVEL] Message
        std::stringstream ss;
        ss << std::put_time(localTime, "[%H:%M:%S]") << " " << getLevelString(level) << " " << message << "\n";
        std::string formattedMessage = ss.str();

        // Output to Console via standard streams based on log level
        if (level == LogLevel::CRITICAL || level == LogLevel::RENDER_ERROR) {
            std::cerr << formattedMessage;
        } else {
            std::cout << formattedMessage;
        }

        // Output to File
        if (_isInitialized) {
            _logFile << formattedMessage;
            _logFile.flush(); // Force write to disk so it saves even during a crash
        }

        // Mirror to SDL's internal system logs
        // This ensures the OS platform handles it cleanly if compiled for release distributions
        SDL_Log("%s", formattedMessage.c_str());
    }
}