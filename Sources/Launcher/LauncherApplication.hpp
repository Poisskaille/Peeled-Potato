#pragma once

#include <Termina/Core/Application.hpp>

#include "RecentProjects.hpp"

class LauncherApplication : public Termina::Application
{
public:
    LauncherApplication();
    ~LauncherApplication() = default;

    void OnPreUpdate(float dt) override;

private:
    void DrawMainWindow();
    void DrawNewProjectModal();

    void CreateProject();
    void OpenProject(const std::string& projFilePath);

    RecentProjects m_RecentProjects;

    bool m_ShowNewProjectModal = false;
    char m_NewProjectName[256] = {};
    char m_NewProjectDir[512]  = {};
};
