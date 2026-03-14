#include "EditorApplication.hpp"

#include <string>

int main(int argc, char** argv)
{
    std::string projectPath;
    for (int i = 1; i < argc - 1; ++i)
    {
        if (std::string(argv[i]) == "--project")
            projectPath = argv[i + 1];
    }

    EditorApplication app(projectPath);
    app.Run();
    return 0;
}
