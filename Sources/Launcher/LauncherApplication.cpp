#include <iostream>

#include "LauncherApplication.hpp"

#include "ImGui/imgui.h"
#include "Termina/Input/InputSystem.hpp"

#include <Termina/Core/Project.hpp>
#include <Termina/Platform/FileDialog.hpp>
#include <Termina/Platform/LaunchProcess.hpp>
#include <Termina/Renderer/Renderer.hpp>
#include <Termina/Renderer/UIUtils.hpp>
#include <Termina/Shader/ShaderManager.hpp>
#include <Termina/World/WorldSystem.hpp>
#include <Termina/World/ComponentRegistry.hpp>

#include <filesystem>

LauncherApplication::LauncherApplication()
    : Application("LAUNCHER")
{
    m_SystemManager.AddSystem<Termina::InputSystem>(m_Window->GetHandle());
    m_SystemManager.AddSystem<Termina::WorldSystem>();
    auto renderer = m_SystemManager.AddSystem<Termina::RendererSystem>(m_Window);
    m_SystemManager.AddSystem<Termina::ShaderManager>();

    renderer->SetShouldImGuiClear(true);

    Termina::ComponentRegistry::Get().Report();

    m_RecentProjects.Load();

    
    ImGui::SetCurrentContext((ImGuiContext*)renderer->GetImGuiContext());
    void* allocFunc;
    void* freeFunc;
    void* userData;
    renderer->GetImGuiAllocator(&allocFunc, &freeFunc, &userData);
    ImGui::SetAllocatorFunctions((ImGuiMemAllocFunc)allocFunc, (ImGuiMemFreeFunc)freeFunc, userData);


}

void LauncherApplication::OnPreUpdate(float dt)
{
    DrawMainWindow();
}

void LauncherApplication::DrawMainWindow()
{
    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(io.DisplaySize);

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoCollapse
        | ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::Begin("##Launcher", nullptr, flags);

    // Header
    Termina::UIUtils::PushStylized();
    Termina::UIUtils::DrawStylizedText("Termina Engine");
    Termina::UIUtils::PopStylized();

    ImGui::Separator();
    ImGui::Spacing();

    // Left panel — actions
    ImGui::BeginChild("##Actions", ImVec2(200, 0), true);

    if (Termina::UIUtils::Button("New Project", ImVec2(-1, 0)))
    {
        memset(m_NewProjectName, 0, sizeof(m_NewProjectName));
        memset(m_NewProjectDir, 0, sizeof(m_NewProjectDir));
        m_ShowNewProjectModal = true;
    }

    ImGui::Spacing();

    if (Termina::UIUtils::Button("Open Project", ImVec2(-1, 0)))
    {
        std::string path = Termina::FileDialog::OpenFile();
        if (!path.empty())
            OpenProject(path);
    }

    ImGui::EndChild();

    ImGui::SameLine();

    // Right panel — recent projects
    ImGui::BeginChild("##RecentProjects", ImVec2(0, 0), true);

    ImGui::Text("Recent Projects");
    ImGui::Separator();
    ImGui::Spacing();

    const auto& projects = m_RecentProjects.GetAll();
    if (projects.empty())
    {
        ImGui::TextDisabled("No recent projects.");
    }
    else
    {
        for (const auto& proj : projects)
        {
            ImGui::PushID(proj.Path.c_str());

            ImGui::Text("%s", proj.Name.c_str());
            ImGui::SameLine();
            ImGui::TextDisabled("  %s", proj.Path.c_str());

            ImGui::SameLine(ImGui::GetContentRegionAvail().x - 60);
            bool open = Termina::UIUtils::Button("Open", ImVec2(60, 0));
            std::string pathToOpen;
            if (open)
                pathToOpen = proj.Path;

            ImGui::PopID();
            ImGui::Separator();

            if (open)
            {
                OpenProject(pathToOpen);
                break;
            }
        }
    }

    ImGui::EndChild();

    DrawNewProjectModal();

    ImGui::End();
}

void LauncherApplication::DrawNewProjectModal()
{
    if (m_ShowNewProjectModal)
    {
        ImGui::OpenPopup("New Project");
        m_ShowNewProjectModal = false;
    }

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(480, 0));

    if (ImGui::BeginPopupModal("New Project", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Project Name");
        ImGui::InputText("##Name", m_NewProjectName, sizeof(m_NewProjectName));

        ImGui::Spacing();
        ImGui::Text("Location");
        ImGui::InputText("##Dir", m_NewProjectDir, sizeof(m_NewProjectDir));
        ImGui::SameLine();
        if (ImGui::Button("Browse"))
        {
            std::string dir = Termina::FileDialog::OpenDirectory();
            if (!dir.empty())
                strncpy(m_NewProjectDir, dir.c_str(), sizeof(m_NewProjectDir) - 1);
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        bool canCreate = m_NewProjectName[0] != '\0' && m_NewProjectDir[0] != '\0';
        if (!canCreate)
            ImGui::BeginDisabled();

        if (Termina::UIUtils::Button("Create", ImVec2(120, 0)))
        {
            CreateProject();
            ImGui::CloseCurrentPopup();
            m_ShowNewProjectModal = false;
        }

        if (!canCreate)
            ImGui::EndDisabled();

        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0)))
        {
            ImGui::CloseCurrentPopup();
            m_ShowNewProjectModal = false;
        }

        ImGui::EndPopup();
    }
}

void LauncherApplication::CreateProject()
{
    std::string name = m_NewProjectName;
    std::string dir  = m_NewProjectDir;

    // Project lives in {dir}/{name}/
    std::string projectDir = dir + "/" + name;
    std::filesystem::create_directories(projectDir);
    std::filesystem::create_directories(projectDir + "/Assets/Materials");
    std::filesystem::create_directories(projectDir + "/Assets/Models");
    std::filesystem::create_directories(projectDir + "/Assets/Prefabs");
    std::filesystem::create_directories(projectDir + "/Assets/Sounds");
    std::filesystem::create_directories(projectDir + "/Assets/Textures");
    std::filesystem::create_directories(projectDir + "/Assets/Worlds");
    std::filesystem::create_directories(projectDir + "/GameCode");

    // Create the .terminaproj file
    Termina::Project proj;
    proj.NewProject(name, projectDir + "/" + name);

    // The .terminaproj path is projectDir/name.terminaproj
    std::string projFilePath = projectDir + "/" + name + ".terminaproj";

    // Copy engine runtime files from the engine installation directory (CWD)
    auto copyOpts = std::filesystem::copy_options::recursive
                  | std::filesystem::copy_options::overwrite_existing;

    if (std::filesystem::exists("__TERMINA__"))
        std::filesystem::copy("__TERMINA__", projectDir + "/__TERMINA__", copyOpts);

    if (std::filesystem::exists("xmake.lua"))
        std::filesystem::copy("xmake.lua", projectDir + "/xmake.lua", std::filesystem::copy_options::overwrite_existing);

    if (std::filesystem::exists("GameCode/"))
        std::filesystem::copy("GameCode/", projectDir + "/GameCode/", copyOpts);

#if defined(TRMN_MACOS)
    if (std::filesystem::exists("Binaries/Mac"))
        std::filesystem::copy("Binaries/Mac", projectDir + "/Binaries", copyOpts);
    if (std::filesystem::exists("libTermina.dylib"))
        std::filesystem::copy("libTermina.dylib", projectDir + "/libTermina.dylib", std::filesystem::copy_options::overwrite_existing);
#elif defined(TRMN_LINUX)
    if (std::filesystem::exists("Binaries/Linux"))
        std::filesystem::copy("Binaries/Linux", projectDir + "/Binaries", copyOpts);
    if (std::filesystem::exists("libTermina.so"))
        std::filesystem::copy("libTermina.so", projectDir + "/libTermina.so", std::filesystem::copy_options::overwrite_existing);
#elif defined(TRMN_WINDOWS)
    if (std::filesystem::exists("dxcompiler.dll"))
        std::filesystem::copy("dxcompiler.dll", projectDir + "/dxcompiler.dll", std::filesystem::copy_options::overwrite_existing);
    if (std::filesystem::exists("Termina.dll"))
        std::filesystem::copy("Termina.dll", projectDir + "/Termina.dll", std::filesystem::copy_options::overwrite_existing);
#endif

    m_RecentProjects.Add(name, projFilePath);

    Termina::LaunchProcess::LaunchDetached("./Editor", { "--project", projFilePath });
    Close();
}

void LauncherApplication::OpenProject(const std::string& projFilePath)
{
    Termina::Project proj;
    proj.LoadProject(projFilePath);

    m_RecentProjects.Add(proj.Name, projFilePath);

    Termina::LaunchProcess::LaunchDetached("./Editor", { "--project", projFilePath });
    Close();
}
