#include "DLL.hpp"
#include "Core/Logger.hpp"

#if defined(TRMN_WINDOWS)

#include <windows.h>

namespace Termina {
    DLL::DLL(const char* path)
    {
        Load(path);
    }

    DLL::~DLL()
    {
        if (m_Handle) {
            FreeLibrary((HMODULE)m_Handle);
            m_Handle = nullptr;
        }
        if (!m_TempPath.empty()) {
            DeleteFileA(m_TempPath.c_str());
            m_TempPath.clear();
        }
    }

    void DLL::Load(const char* path)
    {
        // Resolve full path relative to the executable directory
        std::string executablePath(MAX_PATH, '\0');
        GetModuleFileNameA(NULL, &executablePath[0], MAX_PATH);
        size_t lastSlash = executablePath.find_last_of("\\/");
        if (lastSlash != std::string::npos)
            executablePath = executablePath.substr(0, lastSlash);

        std::string fullPath = executablePath + "\\" + path;

        // Build a shadow copy path (e.g. GameAssembly_hot.dll) so the
        // original file stays unlocked and xmake can overwrite it while
        // the editor is running.
        std::string tempPath = fullPath;
        size_t dotPos = tempPath.rfind('.');
        if (dotPos != std::string::npos)
            tempPath.insert(dotPos, "_hot");
        else
            tempPath += "_hot";

        // Release the previous shadow copy before writing a new one
        if (m_Handle) {
            FreeLibrary((HMODULE)m_Handle);
            m_Handle = nullptr;
        }
        if (!m_TempPath.empty()) {
            DeleteFileA(m_TempPath.c_str());
            m_TempPath.clear();
        }

        if (!CopyFileA(fullPath.c_str(), tempPath.c_str(), FALSE)) {
            TN_ERROR("Failed to shadow-copy DLL '%s' -> '%s': %lu", path, tempPath.c_str(), GetLastError());
            return;
        }

        m_TempPath = tempPath;
        m_Handle = LoadLibraryA(tempPath.c_str());
        if (!m_Handle) {
            TN_ERROR("Failed to load DLL '%s': %lu", tempPath.c_str(), GetLastError());
        }
    }

    void* DLL::GetSymbol(const char* name)
    {
        if (m_Handle) {
            return GetProcAddress((HMODULE)m_Handle, name);
        }
        return nullptr;
    }
}

#endif
