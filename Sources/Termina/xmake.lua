-- xmake.lua

target("Termina")
    set_kind("shared")
    -- On Windows, DLL symbols aren't exported by default (unlike Unix).
    -- This rule inspects the compiled objects and auto-generates a .def file
    -- that exports every public symbol, so GameAssembly can link against
    -- Termina.dll without manual __declspec(dllexport) on every class.
    if is_plat("windows") then
        add_rules("utils.symbols.export_all", {export_classes = true})
    end
    set_group("Termina")

    if is_mode("debug") then
        add_defines("TRMN_DEBUG", {public = true})
        set_symbols("debug")
        set_optimize("none")
    elseif is_mode("releasedbg") then
        add_defines("TRMN_RELEASE", {public = true})
        set_symbols("debug")
        set_optimize("fastest")
    else
        add_defines("TRMN_RETAIL", {public = true})
        
        set_optimize("fastest")
        
    end

    add_files("**.cpp")
    if is_plat("macosx") then
        add_files("**.mm")
    end

    add_headerfiles("**.hpp")
    add_includedirs(".")
    add_deps(
        "CGLTF",
        "GLFW",
        "GLM",
        "ImGui",
        "Jolt",
        "JSON",
        "MikkTSpace",
        "MiniAudio",
        "stb",
        "VMA",
        "Volk",
        "Vulkan"
    )

    if is_plat("macosx") then
        add_deps("MetalShaderConverter")
    end

    after_link(function (target)
        local destdir = path.join("$(builddir)", "$(plat)", "$(arch)", "$(mode)")
        
        local bindir = ""
        if is_plat("windows") then
            bindir = "Binaries/Windows/*"
        elseif is_plat("macosx") then
            bindir = "Binaries/Mac/*"
        elseif is_plat("linux") then
            bindir = "Binaries/Linux/*"
        end
        
        if bindir ~= "" then
            for _, f in ipairs(os.files(bindir)) do
                local dest = path.join(destdir, path.filename(f))
                if not os.isfile(dest) then
                    os.cp(f, dest)
                end
            end
        end
    end)
