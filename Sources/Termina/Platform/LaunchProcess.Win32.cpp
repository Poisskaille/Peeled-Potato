#include "LaunchProcess.hpp"
#include "Core/Logger.hpp"

#if defined(TRMN_WINDOWS)

#include <windows.h>

namespace Termina {
    int LaunchProcess::Launch(const std::string& executable, const std::vector<std::string>& arguments)
    {
        // Use CreateProcess to launch the process
        STARTUPINFOA si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        std::string cmdLine = executable;
        for (const auto& arg : arguments) {
            cmdLine += " " + arg;
        }

        if (CreateProcessA(
            nullptr,
            const_cast<LPSTR>(cmdLine.c_str()),
            nullptr,
            nullptr,
            FALSE,
            0,
            nullptr,
            nullptr,
            &si,
            &pi
        )) {
            WaitForSingleObject(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            return 0;
        } else {
            TN_ERROR("Failed to create process");
            return -1;
        }
    }

    void LaunchProcess::LaunchDetached(const std::string& executable, const std::vector<std::string>& arguments)
    {
        STARTUPINFOA si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        std::string cmdLine = executable;
        for (const auto& arg : arguments) {
            cmdLine += " " + arg;
        }

        if (CreateProcessA(
            nullptr,
            const_cast<LPSTR>(cmdLine.c_str()),
            nullptr,
            nullptr,
            FALSE,
            DETACHED_PROCESS,
            nullptr,
            nullptr,
            &si,
            &pi
        )) {
            // Don't wait — just close handles and return
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        } else {
            TN_ERROR("Failed to create detached process");
        }
    }
}

#endif
