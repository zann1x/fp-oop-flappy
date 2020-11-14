workspace "FP_OOP"
    architecture "x86_64"
    configurations {
        "Debug",
        "Release"
    }
    startproject "FP_OOP"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

targetdir ("Binaries/" .. outputdir .. "/%{prj.name}")
objdir ("Intermediate/" .. outputdir .. "/%{prj.name}")

IncludeDir = {}
IncludeDir["Glad"] = "FP_OOP/ThirdParty/glad/include"
IncludeDir["GLFW"] = "FP_OOP/ThirdParty/glfw-3.3.2/include"
IncludeDir["stb"] = "FP_OOP/ThirdParty/stb"

group "Dependencies"
    include "FP_OOP/ThirdParty/glfw-3.3.2"
    include "FP_OOP/ThirdParty/glad"
group ""

project "FP_OOP"
    location "FP_OOP"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    files {
        "%{prj.name}/Source/**.cpp",
        "%{prj.name}/Source/**.h",

        "%{IncludeDir.stb}/stb_image.cpp"
    }

    includedirs {
        "%{prj.name}/Source",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.stb}"
    }

    links {
        "Glad",
        "GLFW",
        "opengl32.lib"
    }

    defines {
        "GLFW_INCLUDE_NONE"
        ,"_RUN_FUNCTIONAL"
    }

    filter "system:windows"
        systemversion "latest"

        defines {
            "_CRT_SECURE_NO_WARNINGS",
            "WIN32_LEAN_AND_MEAN"
        }

    filter "configurations:Debug"
        defines { "_DEBUG", "_LIB" }
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines { "NDEBUG", "_LIB" }
        runtime "Release"
        optimize "on"
