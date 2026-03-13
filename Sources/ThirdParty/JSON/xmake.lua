-- xmake.lua

target("JSON")
    set_kind("headeronly")
    add_headerfiles("**.hpp")
    set_group("Third Party")
