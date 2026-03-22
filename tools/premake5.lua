workspace "computer-tools"
    architecture "x64"
    language "C++"
    cppdialect "C++17"

    startproject "compiler"
    
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

project "compiler"
    location "compiler"
    kind "ConsoleApp"

    targetdir (bin .. "%{prj.name}/" .. build)
    objdir (bin_int .. "/%{prj.name}/" .. build)

    files {
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.h"
    }
    includedirs {
        "%{prj.name}/src",
        "%{wks.location}/assembler/asm_src",
        "%{wks.location}/core/src"
    }

    links { "core", "assembler" }

project "assembler"
    location "assembler/asm_src"
    kind "StaticLib"

    targetdir (bin .. "%{prj.name}/" .. build)
    objdir (bin_int .. "/%{prj.name}/" .. build)

    files {
        "%{wks.location}/assembler/asm_src/**.cpp",
        "%{wks.location}/assembler/asm_src/**.h"
    }
    includedirs {
        "%{wks.location}/assembler/asm_src",
        "%{wks.location}/core/src"
    }

project "assembler-app"
    location "assembler/app_src"
    kind "ConsoleApp"

    targetdir (bin .. "%{prj.name}/" .. build)
    objdir (bin_int .. "/%{prj.name}/" .. build)

    files {
        "%{wks.location}/assembler/app_src/**.cpp",
        "%{wks.location}/assembler/app_src/**.h"
    }
    includedirs {
        "%{wks.location}/assembler/app_src",
        "%{wks.location}/assembler/asm_src",
        "%{wks.location}/core/src"
    }

    links { "assembler", "core" }

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

project "core"
    location "core"
    kind "StaticLib"

    targetdir (bin .. "%{prj.name}/" .. build)
    objdir (bin_int .. "/%{prj.name}/" .. build)

    files {
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.h"
    }
    includedirs {
        "%{prj.name}/src"
    }
    