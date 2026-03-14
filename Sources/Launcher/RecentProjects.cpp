#include "RecentProjects.hpp"

#include <Termina/Core/FileSystem.hpp>
#include <JSON/json.hpp>

std::string RecentProjects::GetStoragePath() const
{
    return "recent_projects.json";
}

void RecentProjects::Load()
{
    m_Projects.clear();
    std::string path = GetStoragePath();
    if (!Termina::FileSystem::FileExists(path))
        return;

    nlohmann::json json = Termina::FileSystem::ReadJSONFromDisk(path);
    if (!json.contains("projects"))
        return;

    for (const auto& entry : json["projects"])
    {
        RecentProject proj;
        proj.Name = entry["name"];
        proj.Path = entry["path"];
        m_Projects.push_back(proj);
    }
}

void RecentProjects::Save() const
{
    std::string path = GetStoragePath();

    nlohmann::json json;
    json["projects"] = nlohmann::json::array();
    for (const auto& proj : m_Projects)
    {
        nlohmann::json entry;
        entry["name"] = proj.Name;
        entry["path"] = proj.Path;
        json["projects"].push_back(entry);
    }

    Termina::FileSystem::WriteJSONToDisk(path, json);
}

void RecentProjects::Add(const std::string& name, const std::string& path)
{
    // Remove existing entry with the same path (deduplication)
    Remove(path);

    // Prepend
    m_Projects.insert(m_Projects.begin(), { name, path });

    Save();
}

void RecentProjects::Remove(const std::string& path)
{
    m_Projects.erase(
        std::remove_if(m_Projects.begin(), m_Projects.end(),
            [&](const RecentProject& p) { return p.Path == path; }),
        m_Projects.end()
    );
}
