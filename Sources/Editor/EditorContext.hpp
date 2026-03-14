#pragma once

#include <Termina/Core/IInspectable.hpp>
#include <Termina/Core/Project.hpp>

class ContentViewerPanel;

struct EditorContext
{
    Termina::IInspectable* ItemToInspect = nullptr;
    ContentViewerPanel* ContentViewer = nullptr;
    Termina::Project CurrentProject;
};
