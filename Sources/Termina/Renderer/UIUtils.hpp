#pragma once

#include <ImGui/imgui.h>

namespace Termina {
    class UIUtils
    {
    public:
        static void Setup();
        static void DrawStylizedText(const char* text, ImVec2 offset = ImVec2(8, 4));

        // Stylized context — widgets inside push/pop render with mixed Fraktur capital + Playfair body
        static void PushStylized();
        static void PopStylized();
        static bool IsStylized();

        // Window wrapper
        static bool BeginEditorWindow(const char* title, bool* open = nullptr, ImGuiWindowFlags flags = 0);
        static void EndEditorWindow();

        // Stylized widget wrappers (pass-through when not in stylized context)
        static bool Button(const char* label, ImVec2 size = ImVec2(0, 0));
        static bool CollapsingHeader(const char* label, ImGuiTreeNodeFlags flags = 0);
        static bool TreeNode(const char* label);
        static bool TreeNodeEx(const char* label, ImGuiTreeNodeFlags flags = 0);
        static void TreePop();
        static bool BeginMenuBar();
        static void EndMenuBar();
        static bool BeginMenu(const char* label, bool enabled = true);
        static void EndMenu();
        static bool MenuItem(const char* label, const char* shortcut = nullptr, bool selected = false, bool enabled = true);

    private:
        static void SetTheme();

        static struct Data {
            ImFont* RegularFont;
            ImFont* CapitalFont;
            int StylizedDepth = 0;
        } sData;
    };
}
