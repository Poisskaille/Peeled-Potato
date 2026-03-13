-- xmake.lua

target("GameAssembly")
    set_kind("shared")
    add_files("**.cpp")
    add_headerfiles("**.hpp")
    add_deps("Termina")

    if is_plat("windows") then
        set_symbols("none")
    end
