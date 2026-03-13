#include "Logger.hpp"

#include <iostream>
#include <fstream>
#include <cstdarg>
#include <mutex>
#include <cstdio>

namespace Termina {
    ELogLevel g_LogLevel = ELogLevel::kInfo;
    std::ofstream g_LogFile;
    std::mutex g_LogMutex;

    const char* LevelToString(ELogLevel level) {
        switch (level) {
            case ELogLevel::kDebug:   return "DEBUG";
            case ELogLevel::kTrace:   return "TRACE";
            case ELogLevel::kInfo:    return "INFO";
            case ELogLevel::kWarning: return "WARN";
            case ELogLevel::kError:   return "ERROR";
            case ELogLevel::kFatal:   return "FATAL";
            default:                  return "UNKNOWN";
        }
    }

    // Extract just the file name from the full path
    const char* GetFilename(const char* path) {
        const char* file = path;
        while (*path) {
            if (*path == '/' || *path == '\\') {
                file = path + 1;
            }
            path++;
        }
        return file;
    }

    void Logger::Initialize(const std::string& logPath, ELogLevel logLevel) {
        g_LogLevel = logLevel;
        if (!logPath.empty()) {
            g_LogFile.open(logPath, std::ios::out | std::ios::trunc);
        }
    }

    void Logger::Flush() {
        if (g_LogFile.is_open()) {
            g_LogFile.flush();
        }
        std::cout.flush();
        std::cerr.flush();
    }

    void Logger::Output(ELogLevel level, const char* file, int line, const char* message) {
        const char* levelStr = LevelToString(level);
        const char* shortFile = GetFilename(file);

        // Format: [LEVEL] File.cpp:123 - Message
        if (level >= ELogLevel::kError) {
            std::cerr << "[" << levelStr << "] " << shortFile << ":" << line << " - " << message << "\n";
        } else {
            std::cout << "[" << levelStr << "] " << shortFile << ":" << line << " - " << message << "\n";
        }

        if (g_LogFile.is_open()) {
            g_LogFile << "[" << levelStr << "] " << shortFile << ":" << line << " - " << message << "\n";
        }
    }

    // Helper macro to handle variadic arguments
    #define LOG_FORMAT_IMPL(level) \
    if (level < g_LogLevel) return; \
    va_list args; \
    va_start(args, format); \
    char buffer[4096]; \
    std::vsnprintf(buffer, sizeof(buffer), format, args); \
    va_end(args); \
    Output(level, file, line, buffer);

    void Logger::Debug(const char* file, int line, const char* format, ...) {
        LOG_FORMAT_IMPL(ELogLevel::kDebug)
    }

    void Logger::Trace(const char* file, int line, const char* format, ...) {
        LOG_FORMAT_IMPL(ELogLevel::kTrace)
    }

    void Logger::Info(const char* file, int line, const char* format, ...) {
        LOG_FORMAT_IMPL(ELogLevel::kInfo)
    }

    void Logger::Warning(const char* file, int line, const char* format, ...) {
        LOG_FORMAT_IMPL(ELogLevel::kWarning)
    }

    void Logger::Error(const char* file, int line, const char* format, ...) {
        LOG_FORMAT_IMPL(ELogLevel::kError)
    }

    void Logger::Fatal(const char* file, int line, const char* format, ...) {
        LOG_FORMAT_IMPL(ELogLevel::kFatal)
    }
} // namespace Termina
