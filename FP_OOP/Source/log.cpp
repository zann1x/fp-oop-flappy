#include "log.h"

#include <cstdarg>
#include <cstdio>
#include <ctime>

const std::array<const char*, 6> Log::level_names = {
    "TRACE",
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL"
};

// Colors are Windows console colors, see
// https://github.com/shiena/ansicolor/blob/4593a90318c32343a5cf962281026d61e96a4e70/README.md
const std::array<const char*, 6> Log::level_colors = {
    "\x1b[37m",   // white
    "\x1b[36m",   // cyan
    "\x1b[32m",   // green
    "\x1b[33m",   // yellow
    "\x1b[31m",   // red
    "\x1b[31;1m"  // bold red
};

void Log::log(LogLevel level, const char* file, int line, const char* format, ...) {
    // Get the current local time and convert it to a human readable format
    const time_t current_time = time(NULL);
    const struct tm* local_time = localtime(&current_time);
    char formatted_time[16];
    formatted_time[strftime(formatted_time, sizeof(formatted_time), "%H:%M:%S", local_time)] = '\0';

    // Log to console
    // \x1b[0m = reset attributes
    // \x1b[90m = light gray
    FILE* output_stream = level < LogLevel::WARN ? stdout : stderr;
    fprintf(output_stream, "%s %s%-5s\x1b[0m \x1b[90m%s:%d:\x1b[0m ", formatted_time,
        level_colors[static_cast<int>(level)], level_names[static_cast<int>(level)], file, line);

    va_list args;
    va_start(args, format);
    vfprintf(output_stream, format, args);
    va_end(args);
    fprintf(output_stream, "\n");
    fflush(output_stream);
}
