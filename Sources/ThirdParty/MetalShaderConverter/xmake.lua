-- xmake.lua

target("MetalShaderConverter")
    set_kind("static")
    add_files("*.mm")
    add_headerfiles("*.h")
    add_frameworks("Metal")
    set_group("Third Party")
