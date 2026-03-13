#include "ContentViewerPanel.hpp"

#include "ImGui/imgui.h"
#include "Termina/Renderer/UIUtils.hpp"

#include <filesystem>
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;

const char* ContentViewerPanel::GetFileIcon(const fs::path& path) const
{
    std::string ext = path.extension().string();
    if (ext == ".png" || ext == ".jpg" || ext == ".jpeg" || ext == ".bmp")
        return "[IMG]";
    if (ext == ".hlsl" || ext == ".hlsli")
        return "[SHD]";
    if (ext == ".mp3" || ext == ".wav" || ext == ".ogg")
        return "[SND]";
    if (ext == ".ttf" || ext == ".otf")
        return "[FNT]";
    if (ext == ".trw")
        return "[WLD]";
    return "[FILE]";
}

void ContentViewerPanel::OnImGuiRender()
{
    Termina::UIUtils::BeginEditorWindow(m_Name.c_str(), &m_Open);

    // Breadcrumb navigation
    {
        fs::path root("Assets");
        fs::path rel = fs::relative(m_CurrentPath, root.parent_path());
        std::vector<fs::path> parts;
        for (const auto& part : rel)
            parts.push_back(part);

        fs::path accumulated = root.parent_path();
        for (size_t i = 0; i < parts.size(); ++i)
        {
            accumulated /= parts[i];
            if (i > 0)
                ImGui::SameLine();
            ImGui::PushID((int)i);
            if (ImGui::SmallButton(parts[i].string().c_str()))
                m_CurrentPath = accumulated;
            ImGui::PopID();
            if (i + 1 < parts.size())
            {
                ImGui::SameLine();
                ImGui::TextDisabled(">");
            }
        }
    }

    ImGui::Separator();

    if (!fs::exists(m_CurrentPath) || !fs::is_directory(m_CurrentPath))
    {
        ImGui::TextDisabled("(directory not found)");
        Termina::UIUtils::EndEditorWindow();
        return;
    }

    // Collect entries: folders first, then files
    std::vector<fs::directory_entry> folders, files;
    for (const auto& entry : fs::directory_iterator(m_CurrentPath))
    {
        if (entry.is_directory())
            folders.push_back(entry);
        else
            files.push_back(entry);
    }
    std::sort(folders.begin(), folders.end(), [](const auto& a, const auto& b) {
        return a.path().filename() < b.path().filename();
    });
    std::sort(files.begin(), files.end(), [](const auto& a, const auto& b) {
        return a.path().filename() < b.path().filename();
    });

    // Render folders
    for (const auto& entry : folders)
    {
        std::string name = entry.path().filename().string();
        std::string label = "[DIR]  " + name;
        bool selected = (m_SelectedEntry == entry.path().string());

        if (ImGui::Selectable(label.c_str(), selected, ImGuiSelectableFlags_AllowDoubleClick))
        {
            m_SelectedEntry = entry.path().string();
            if (ImGui::IsMouseDoubleClicked(0))
                m_CurrentPath = entry.path();
        }
    }

    // Render files
    for (const auto& entry : files)
    {
        std::string name = entry.path().filename().string();
        const char* icon = GetFileIcon(entry.path());
        std::string label = std::string(icon) + "  " + name;
        bool selected = (m_SelectedEntry == entry.path().string());

        if (ImGui::Selectable(label.c_str(), selected))
            m_SelectedEntry = entry.path().string();

        // Drag source for asset picking
        Termina::UIUtils::AssetPickerSource(entry.path().string());
    }

    Termina::UIUtils::EndEditorWindow();
}
