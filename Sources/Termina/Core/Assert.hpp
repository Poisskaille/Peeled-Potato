#pragma once

#include "Logger.hpp"

namespace Termina {
    class Assert {
    public:
        /// Reports a failure assertion with the given condition, file, line, and format string.
        static void ReportFailure(const char* condition, const char* file, int line, const char* format, ...);
    };
}

#ifdef NDEBUG
    #define TN_ASSERT(condition, ...) if (condition) {}
#else
    #define TN_ASSERT(condition, ...) \
        do { \
            if (!(condition)) { \
                ::Termina::Assert::ReportFailure(#condition, __FILE__, __LINE__, ##__VA_ARGS__); \
            } \
        } while (false)
#endif
