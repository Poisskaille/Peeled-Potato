#pragma once

#include <string>
#include <vector>

struct RecentProject
{
    std::string Name;
    std::string Path;
};

class RecentProjects
{
public:
    void Load();
    void Save() const;

    void Add(const std::string& name, const std::string& path);
    void Remove(const std::string& path);

    const std::vector<RecentProject>& GetAll() const { return m_Projects; }

private:
    std::string GetStoragePath() const;

    std::vector<RecentProject> m_Projects;
};
