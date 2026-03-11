#include "UIUtils.hpp"
#include "Core/Logger.hpp"
#include "ImGui/imgui.h"

namespace Termina {
    UIUtils::Data UIUtils::sData;

    void UIUtils::Setup()
    {
        ImGuiIO& io = ImGui::GetIO();
        sData.RegularFont = io.Fonts->AddFontFromFileTTF("Assets/Fonts/PlayfairDisplay-Regular.ttf", 20.0f);
        sData.CapitalFont = io.Fonts->AddFontFromFileTTF("Assets/Fonts/UnifrakturMaguntia-Regular.ttf", 24.0f);

        SetTheme();
    }

    void UIUtils::DrawStylizedText(const char* text, ImVec2 offset)
    {
        if (!text || text[0] == '\0') return;

        ImGui::PushFont(sData.CapitalFont, sData.CapitalFont->LegacySize);
        ImGui::Text("%c", text[0]);
        ImGui::PopFont();

        if (text[1] != '\0')
        {
            ImGui::SameLine(0, 0); // no spacing
            ImGui::SetCursorPosY(ImGui::GetCursorPosY());
            ImGui::PushFont(sData.RegularFont, sData.RegularFont->LegacySize);
            ImGui::Text("%s", text + 1);
            ImGui::PopFont();
        }
    }

    bool UIUtils::BeginEditorWindow(const char* title, bool* open, ImGuiWindowFlags flags)
    {
        ImGui::PushFont(sData.RegularFont, sData.RegularFont->LegacySize);

        bool visible = ImGui::Begin(title, open, flags); // keep the real title bar
        float regularFontSize = 18.0f * ImGui::GetIO().FontGlobalScale;
        float capitalFontSize = 20.0f * ImGui::GetIO().FontGlobalScale;

        if (visible)
        {
            float titleBarHeight = ImGui::GetFrameHeight();
            ImVec2 windowPos = ImGui::GetWindowPos();
            float windowWidth = ImGui::GetWindowWidth();
        
            ImDrawList* dl = ImGui::GetWindowDrawList();

            dl->PushClipRect(
                windowPos,
                ImVec2(windowPos.x + windowWidth, windowPos.y + ImGui::GetWindowHeight()),
                false // false = don't intersect with current cliprect, replace it
            );
        
            dl->AddRectFilled(
                windowPos,
                ImVec2(windowPos.x + windowWidth, windowPos.y + titleBarHeight),
                IM_COL32(255, 255, 255, 255)
            );
        
            dl->AddLine(
                ImVec2(windowPos.x, windowPos.y + titleBarHeight),
                ImVec2(windowPos.x + windowWidth, windowPos.y + titleBarHeight),
                IM_COL32(90, 75, 60, 200), 1.0f
            );
        
            // Draw text directly via drawlist, bypassing cursor/clip issues
            ImVec2 textPos = ImVec2(windowPos.x + 8, windowPos.y + ImGui::GetStyle().FramePadding.y);
        
            // Capital letter with Unifraktur
            int cmdCountBefore = dl->CmdBuffer.Size;
            dl->AddText(sData.CapitalFont, capitalFontSize,
                textPos,
                IM_COL32(40, 35, 30, 255),
                title, title + 1
            );
        
            // Rest with IM Fell — advance x by the capital glyph width
            float capWidth = sData.CapitalFont->CalcTextSizeA(capitalFontSize, FLT_MAX, 0, title, title + 1).x;
            dl->AddText(sData.RegularFont, regularFontSize,
                ImVec2(textPos.x + (capWidth + 2), textPos.y),
                IM_COL32(40, 35, 30, 255),
                title + 1
            );


            dl->PopClipRect();
        
            ImGui::SetCursorPosY(titleBarHeight + ImGui::GetStyle().ItemSpacing.y);
        }

        return visible;
    }

    void UIUtils::EndEditorWindow()
    {
        ImGui::End();
        ImGui::PopFont(); // matches PushFont in BeginEditorWindow
    }

    void UIUtils::PushStylized()
    {
        sData.StylizedDepth++;
        if (sData.StylizedDepth == 1)
            ImGui::SetStylizedCapitalFont(sData.CapitalFont);
    }

    void UIUtils::PopStylized()
    {
        sData.StylizedDepth--;
        if (sData.StylizedDepth == 0)
            ImGui::SetStylizedCapitalFont(nullptr);
    }

    bool UIUtils::IsStylized()
    {
        return sData.StylizedDepth > 0;
    }

    bool UIUtils::Button(const char* label, ImVec2 size)               { return ImGui::Button(label, size); }
    bool UIUtils::CollapsingHeader(const char* label, ImGuiTreeNodeFlags flags) { return ImGui::CollapsingHeader(label, flags); }
    bool UIUtils::TreeNode(const char* label)                           { return ImGui::TreeNode(label); }
    bool UIUtils::TreeNodeEx(const char* label, ImGuiTreeNodeFlags flags) { return ImGui::TreeNodeEx(label, flags); }
    void UIUtils::TreePop()                                             { ImGui::TreePop(); }
    bool UIUtils::BeginMenuBar()                                        { return ImGui::BeginMenuBar(); }
    void UIUtils::EndMenuBar()                                          { ImGui::EndMenuBar(); }
    bool UIUtils::BeginMenu(const char* label, bool enabled)            { return ImGui::BeginMenu(label, enabled); }
    void UIUtils::EndMenu()                                             { ImGui::EndMenu(); }
    bool UIUtils::MenuItem(const char* label, const char* shortcut, bool selected, bool enabled) { return ImGui::MenuItem(label, shortcut, selected, enabled); }

    void UIUtils::SetTheme()
    {
        ImGuiStyle& style = ImGui::GetStyle();

        // Rounding — sharp, clinical
        style.WindowRounding    = 0.0f;
        style.ChildRounding     = 0.0f;
        style.FrameRounding     = 0.0f;
        style.PopupRounding     = 0.0f;
        style.ScrollbarRounding = 0.0f;
        style.GrabRounding      = 0.0f;
        style.TabRounding       = 0.0f;

        // Sizing
        style.WindowBorderSize  = 1.0f;
        style.FrameBorderSize   = 1.0f;
        style.PopupBorderSize   = 1.0f;
        style.FramePadding      = ImVec2(6, 4);
        style.ItemSpacing       = ImVec2(8, 4);
        style.IndentSpacing     = 14.0f;
        style.ScrollbarSize     = 10.0f;
        style.GrabMinSize       = 8.0f;

        ImVec4* c = style.Colors;

        // Aged newsprint cream — stark, high contrast
        c[ImGuiCol_WindowBg]             = ImVec4(0.96f, 0.94f, 0.91f, 1.00f);
        c[ImGuiCol_ChildBg]              = ImVec4(0.93f, 0.91f, 0.87f, 1.00f);
        c[ImGuiCol_PopupBg]              = ImVec4(0.98f, 0.97f, 0.95f, 1.00f);

        // Borders — hard black
        c[ImGuiCol_Border]               = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        c[ImGuiCol_BorderShadow]         = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

        // Title bars — white (BeginEditorWindow overdraws these anyway)
        c[ImGuiCol_TitleBg]              = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        c[ImGuiCol_TitleBgActive]        = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        c[ImGuiCol_TitleBgCollapsed]     = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

        // Frames — white fill, border provides visual weight
        c[ImGuiCol_FrameBg]              = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        c[ImGuiCol_FrameBgHovered]       = ImVec4(0.90f, 0.88f, 0.84f, 1.00f);
        c[ImGuiCol_FrameBgActive]        = ImVec4(0.80f, 0.78f, 0.74f, 1.00f);

        // Scrollbar
        c[ImGuiCol_ScrollbarBg]          = ImVec4(0.93f, 0.91f, 0.87f, 1.00f);
        c[ImGuiCol_ScrollbarGrab]        = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        c[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
        c[ImGuiCol_ScrollbarGrabActive]  = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);

        // Headers (TreeNode, CollapsingHeader, Selectable)
        c[ImGuiCol_Header]               = ImVec4(0.85f, 0.83f, 0.79f, 1.00f);
        c[ImGuiCol_HeaderHovered]        = ImVec4(0.75f, 0.73f, 0.68f, 1.00f);
        c[ImGuiCol_HeaderActive]         = ImVec4(0.60f, 0.58f, 0.53f, 1.00f);

        // Buttons — white fill, black border carries the weight
        c[ImGuiCol_Button]               = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        c[ImGuiCol_ButtonHovered]        = ImVec4(0.88f, 0.86f, 0.82f, 1.00f);
        c[ImGuiCol_ButtonActive]         = ImVec4(0.70f, 0.68f, 0.63f, 1.00f);

        // Tabs
        c[ImGuiCol_Tab]                  = ImVec4(0.88f, 0.86f, 0.82f, 1.00f);
        c[ImGuiCol_TabHovered]           = ImVec4(0.75f, 0.73f, 0.68f, 1.00f);
        c[ImGuiCol_TabActive]            = ImVec4(0.96f, 0.94f, 0.91f, 1.00f); // matches WindowBg — feels "open"
        c[ImGuiCol_TabUnfocused]         = ImVec4(0.90f, 0.88f, 0.84f, 1.00f);
        c[ImGuiCol_TabUnfocusedActive]   = ImVec4(0.93f, 0.91f, 0.87f, 1.00f);

        // Docking
        c[ImGuiCol_DockingPreview]       = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
        c[ImGuiCol_DockingEmptyBg]       = ImVec4(0.93f, 0.91f, 0.87f, 1.00f);

        // Selection / highlight — ink stamp
        c[ImGuiCol_CheckMark]            = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
        c[ImGuiCol_SliderGrab]           = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        c[ImGuiCol_SliderGrabActive]     = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);

        // Text — near-black on cream
        c[ImGuiCol_Text]                 = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
        c[ImGuiCol_TextDisabled]         = ImVec4(0.45f, 0.43f, 0.40f, 1.00f);
        c[ImGuiCol_TextSelectedBg]       = ImVec4(0.00f, 0.00f, 0.00f, 0.25f);

        // Separator — ruled line
        c[ImGuiCol_Separator]            = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        c[ImGuiCol_SeparatorHovered]     = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        c[ImGuiCol_SeparatorActive]      = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);

        // Resize grip
        c[ImGuiCol_ResizeGrip]           = ImVec4(0.00f, 0.00f, 0.00f, 0.25f);
        c[ImGuiCol_ResizeGripHovered]    = ImVec4(0.00f, 0.00f, 0.00f, 0.60f);
        c[ImGuiCol_ResizeGripActive]     = ImVec4(0.00f, 0.00f, 0.00f, 0.90f);

        // Menubar — white
        c[ImGuiCol_MenuBarBg]            = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

        // Modal overlay
        c[ImGuiCol_ModalWindowDimBg]     = ImVec4(0.04f, 0.04f, 0.04f, 0.50f);

        // Nav highlight
        c[ImGuiCol_NavHighlight]         = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
        c[ImGuiCol_NavWindowingHighlight]= ImVec4(0.04f, 0.04f, 0.04f, 0.70f);
        c[ImGuiCol_NavWindowingDimBg]    = ImVec4(0.04f, 0.04f, 0.04f, 0.20f);
    }
}
