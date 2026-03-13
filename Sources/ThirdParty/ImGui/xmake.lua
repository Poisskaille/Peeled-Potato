-- xmake.lua

target("ImGui")
    if is_plat("windows") then
        set_kind("shared")
        add_rules("utils.symbols.export_all", {export_classes = true})
    else
        set_kind("static")
    end
    add_files("*.cpp")
    add_headerfiles("*.h")
    add_deps("GLFW")

    add_defines("IMGUI_IMPL_GLFW_DISABLE_X11")
    set_group("Third Party")

    if is_mode("debug") then
        set_symbols("debug")
        set_optimize("none")
    elseif is_mode("releasedbg") then
        set_symbols("debug")
        set_optimize("fastest")
    else
        set_symbols("hidden")
        set_optimize("fastest")
        set_strip("all")
    end
