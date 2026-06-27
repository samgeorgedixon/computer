workspace "computer-tools"
    architecture "x64"
    language "C++"
    cppdialect "C++17"

    startproject "toolchain"
    
    configurations { "debug", "release" }

    filter "system:windows"
        systemversion "latest"
        defines "PLATFORM_WINDOWS"
        staticruntime "on"
    filter "configurations:debug"
        defines "DEBUG"
        symbols "on"
    filter "configurations:release"
        defines "RELEASE"
        optimize "on"

build = "%{cfg.buildcfg}-%{cfg.system}"
bin = "%{wks.location}/../bin/tools/"
bin_int = "%{wks.location}/../bin/bin-int/tools/"

project "toolchain"
    location "toolchain"
    kind "ConsoleApp"

    targetdir (bin .. "%{prj.name}/" .. build)
    objdir (bin_int .. "%{prj.name}/" .. build)

    files {
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.h"
    }
    includedirs {
        "%{prj.name}/src",
    }

project "cpu-instr-gen"
    location "cpu-instr-gen"
    kind "ConsoleApp"

    targetdir (bin .. "%{prj.name}/" .. build)
    objdir (bin_int .. "/%{prj.name}/" .. build)

    files {
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.h"
    }
    includedirs {
        "%{prj.name}/src"
    }
    
project "file-system-manager"
    location "file-system-manager"
    kind "ConsoleApp"

    targetdir (bin .. "%{prj.name}/" .. build)
    objdir (bin_int .. "/%{prj.name}/" .. build)


    files {
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.h"
    }
    includedirs {
        "%{prj.name}/src"
    }
