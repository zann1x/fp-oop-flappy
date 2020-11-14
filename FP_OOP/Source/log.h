#pragma once

#include <array>

enum class LogLevel {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
};

class Log {
public:
    static void log(const LogLevel level, const char* file, const int line, const char* format, ...);

private:
    static const std::array<const char*, 6> level_names;
    static const std::array<const char*, 6> level_colors;
};

#define LOG_TRACE(...)      Log::log(LogLevel::TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_DEBUG(...)      Log::log(LogLevel::DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_INFO(...)       Log::log(LogLevel::INFO, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARN(...)       Log::log(LogLevel::WARN, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...)      Log::log(LogLevel::ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_FATAL(...)      Log::log(LogLevel::FATAL, __FILE__, __LINE__, __VA_ARGS__)
