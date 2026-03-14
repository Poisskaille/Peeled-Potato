#include "LaunchProcess.hpp"
#include "Core/Logger.hpp"

#if !defined(TRMN_WINDOWS)

#include <unistd.h>
#include <sys/wait.h>

namespace Termina {
    int LaunchProcess::Launch(const std::string& executable, const std::vector<std::string>& arguments)
    {
        // Use fork and exec to launch the process
        pid_t pid = fork();
        if (pid == -1) {
            TN_ERROR("Failed to fork process");
            return -1;
        } else if (pid == 0) {
            // In child process
            std::vector<char*> args;
            args.push_back(const_cast<char*>(executable.c_str()));
            for (const auto& arg : arguments) args.push_back(const_cast<char*>(arg.c_str()));
            args.push_back(nullptr);
            execvp(args[0], args.data());
            TN_ERROR("Failed to exec process");
            return -1;
        } else {
            int status;
            waitpid(pid, &status, 0);
            return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
        }
    }

    void LaunchProcess::LaunchDetached(const std::string& executable, const std::vector<std::string>& arguments)
    {
        // Double-fork: the grandchild is re-parented to init so it never becomes a zombie.
        pid_t pid = fork();
        if (pid == -1) {
            TN_ERROR("Failed to fork process");
            return;
        } else if (pid == 0) {
            // Intermediate child
            if (fork() == 0) {
                // Grandchild — detach from terminal and exec
                setsid();
                std::vector<char*> args;
                args.push_back(const_cast<char*>(executable.c_str()));
                for (const auto& arg : arguments) args.push_back(const_cast<char*>(arg.c_str()));
                args.push_back(nullptr);
                execvp(args[0], args.data());
                _exit(1);
            }
            _exit(0); // Intermediate child exits immediately
        } else {
            // Parent reaps intermediate child instantly
            waitpid(pid, nullptr, 0);
        }
    }
}

#endif
