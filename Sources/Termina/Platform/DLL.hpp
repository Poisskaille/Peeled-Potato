#pragma once

#include <string>

namespace Termina {
    /// Represents a dynamically loaded library (DLL or shared object).
    class DLL
    {
    public:
        DLL() = default;
        DLL(const char* path);
        ~DLL();

        void Load(const char* path);
        void* GetSymbol(const char* name);

    private:
        void* m_Handle = nullptr;
        std::string m_TempPath; // Windows: path to shadow copy so the original isn't locked
    };
}
