
target("Editor")
    set_kind("binary")
    set_group("Termina")

    add_files("**.cpp")
    add_headerfiles("**.hpp")
    add_includedirs(".")
    add_deps("Termina")

    if is_mode("debug") then
        set_symbols("debug")
        set_optimize("none")
    elseif is_mode("releasedbg") then
        set_symbols("debug")
        set_optimize("fastest")
    else
        set_symbols("hidden")
        set_optimize("fastest")
        
    end
