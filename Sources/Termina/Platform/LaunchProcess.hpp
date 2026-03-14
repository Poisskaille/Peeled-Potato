#pragma once

#include <vector>
#include <string>

namespace Termina {
    /// Represents a process to be launched.
    class LaunchProcess
    {
    public:
        /// Launches a process with the given executable and arguments, and waits for it to finish.
        /// \param executable The path to the executable to launch.
        /// \param arguments The arguments to pass to the executable.
        /// \return The exit code of the process, or -1 if an error occurred.
        static int Launch(const std::string& executable, const std::vector<std::string>& arguments);

        /// Launches a process detached from the current process (fire-and-forget).
        /// Returns immediately without waiting for the child to exit.
        /// \param executable The path to the executable to launch.
        /// \param arguments The arguments to pass to the executable.
        static void LaunchDetached(const std::string& executable, const std::vector<std::string>& arguments);
    };
}
