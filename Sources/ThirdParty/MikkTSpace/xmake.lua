-- xmake.lua

target("MikkTSpace")
    set_kind("static")
    add_files("*.c")
    add_headerfiles("*.h")
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
