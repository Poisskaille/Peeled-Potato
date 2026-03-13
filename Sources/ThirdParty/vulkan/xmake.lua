-- xmake.lua

target("Vulkan")
    set_kind("headeronly")
    add_headerfiles("*.h", "*.hpp")
    set_group("Third Party")
