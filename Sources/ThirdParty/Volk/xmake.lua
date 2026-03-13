-- xmake.lua

target("Volk")
    set_kind("static")
    add_files("*.c")
    add_headerfiles("*.h")
    add_deps("Vulkan")
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
