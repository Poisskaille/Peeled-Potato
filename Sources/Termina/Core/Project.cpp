#include "Project.hpp"

#include <JSON/json.hpp>
#include <Termina/Core/FileSystem.hpp>

namespace Termina {
    void Project::NewProject(const std::string& name, const std::string& path)
    {
        // To get asset path, remove the project name from the path
        std::string assetPath = path.substr(0, path.find_last_of('/'));
        // Then add the project name to get the asset path
        assetPath += "/Assets";
        // Get the project path
        std::string projectPath = path.substr(0, path.find_last_of('/'));

        nlohmann::json projectData;
        projectData["name"] = name;
        projectData["path"] = projectPath;
        projectData["lastOpenedScene"] = "";
        projectData["firstSceneToLoad"] = "";
        projectData["assetPath"] = assetPath;

        FileSystem::WriteJSONToDisk(path + ".terminaproj", projectData);
    }

    void Project::LoadProject(const std::string& path)
    {
        // Read the project data from the file
        nlohmann::json json = FileSystem::ReadJSONFromDisk(path);
        // Load the project data
        Name = json["name"];
        Path = json["path"];
        LastOpenedScene = json["lastOpenedScene"];
        FirstSceneToLoad = json["firstSceneToLoad"];
        AssetPath = json["assetPath"];
    }
}
