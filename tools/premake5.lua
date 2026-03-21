workspace "computer-scripts"
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

builddir = "%{cfg.buildcfg}_%{cfg.system}_%{cfg.architecture}"

project "compiler"
    location "compiler"
    kind "ConsoleApp"

    targetdir ("%{wks.location}/../bin/%{prj.name}/" .. builddir)
    objdir ("%{wks.location}/../bin_int/%{prj.name}/" .. builddir)

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

    targetdir ("%{wks.location}/../bin/%{prj.name}/" .. builddir)
    objdir ("%{wks.location}/../bin_int/%{prj.name}/" .. builddir)

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

    targetdir ("%{wks.location}/../bin/%{prj.name}/" .. builddir)
    objdir ("%{wks.location}/../bin_int/%{prj.name}/" .. builddir)

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

project "cpu_instr_gen"
    location "cpu_instr_gen"
    kind "ConsoleApp"

    targetdir ("%{wks.location}/../bin/%{prj.name}/" .. builddir)
    objdir ("%{wks.location}/../bin_int/%{prj.name}/" .. builddir)

    files {
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.h"
    }
    includedirs {
        "%{prj.name}/src"
    }
    
project "file-system_manager"
    location "file-system_manager"
    kind "ConsoleApp"

    targetdir ("%{wks.location}/../bin/%{prj.name}/" .. builddir)
    objdir ("%{wks.location}/../bin_int/%{prj.name}/" .. builddir)

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

    targetdir ("%{wks.location}/../bin/%{prj.name}/" .. builddir)
    objdir ("%{wks.location}/../bin_int/%{prj.name}/" .. builddir)

    files {
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.h"
    }
    includedirs {
        "%{prj.name}/src"
    }
    