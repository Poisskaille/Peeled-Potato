#include "Logger.hpp"

#include <iostream>
#include <fstream>
#include <cstdarg>
#include <mutex>
#include <cstdio>

#include <ImGui/imgui.h>

namespace Termina {
    ELogLevel              g_LogLevel = ELogLevel::kInfo;
    std::ofstream          g_LogFile;
    std::mutex             g_LogMutex;
    std::vector<LogEntry>  g_LogEntries;
    bool                   g_LogScrollToBottom = true;

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

        if (static_cast<int>(g_LogEntries.size()) >= Logger::kMaxLogEntries)
            g_LogEntries.erase(g_LogEntries.begin());
        g_LogEntries.push_back({ level, shortFile, line, message });
        g_LogScrollToBottom = true;
    }

    void Logger::ClearLog()
    {
        std::lock_guard<std::mutex> lock(g_LogMutex);
        g_LogEntries.clear();
    }

    void Logger::ShowLogWindow(bool* open)
    {
        if (!ImGui::Begin("Log", open))
        {
            ImGui::End();
            return;
        }

        static int s_FilterLevel = 0; // 0 = All
        const char* levelNames[] = { "All", "Debug", "Trace", "Info", "Warning", "Error", "Fatal" };
        ImGui::SetNextItemWidth(120.0f);
        ImGui::Combo("Level", &s_FilterLevel, levelNames, 7);
        ImGui::SameLine();
        if (ImGui::Button("Clear"))
            ClearLog();
        ImGui::SameLine();
        ImGui::Checkbox("Auto-scroll", &g_LogScrollToBottom);

        ImGui::Separator();
        ImGui::BeginChild("LogScrollRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        std::lock_guard<std::mutex> lock(g_LogMutex);

        for (const auto& entry : g_LogEntries)
        {
            if (s_FilterLevel > 0 && static_cast<int>(entry.level) != s_FilterLevel - 1)
                continue;

            ImVec4 color;
            switch (entry.level)
            {
                case ELogLevel::kDebug:   color = ImVec4(0.5f, 0.5f, 0.5f, 1.0f); break;
                case ELogLevel::kTrace:   color = ImVec4(0.3f, 0.3f, 0.3f, 1.0f); break;
                case ELogLevel::kWarning: color = ImVec4(0.7f, 0.4f, 0.0f, 1.0f); break;
                case ELogLevel::kError:   color = ImVec4(0.8f, 0.0f, 0.0f, 1.0f); break;
                case ELogLevel::kFatal:   color = ImVec4(0.6f, 0.0f, 0.0f, 1.0f); break;
                default:                  color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f); break;
            }

            if (entry.level == ELogLevel::kInfo)
                ImGui::Text("%s:%d - %s", entry.file.c_str(), entry.line, entry.message.c_str());
            else
                ImGui::TextColored(color, "[%s] %s:%d - %s",
                    LevelToString(entry.level), entry.file.c_str(), entry.line, entry.message.c_str());
        }

        if (g_LogScrollToBottom)
        {
            ImGui::SetScrollHereY(1.0f);
            g_LogScrollToBottom = false;
        }

        ImGui::EndChild();
        ImGui::End();
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
