#pragma once

#include "Editor/Panel.hpp"
#include <filesystem>

class ContentViewerPanel : public Panel
{
public:
    ContentViewerPanel(EditorContext& context)
        : Panel("Content Viewer", context)
        , m_CurrentPath("Assets") {}

    void OnImGuiRender() override;

private:
    std::filesystem::path m_CurrentPath;
    std::string m_SelectedEntry;

    const char* GetFileIcon(const std::filesystem::path& path) const;
};
