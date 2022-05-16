workspace "Chemical"
    location "../"
    configurations { "Debug", "Release" }
    platforms { "Win64", "Win32" }
    startproject "Sandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.platform}"

project "Chemical"
    kind "StaticLib"
    language "C++"
    system "Windows"
    cppdialect "C++20"
    systemversion "latest"
    debugformat "c7"

    defines "GLFW_INCLUDE_NONE"

    location "../Chemical/"
    files { "%{prj.location}/src/**.cpp", "%{prj.location}/src/**.h", "%{prj.location}/src/**.c"}
    includedirs "%{prj.location}/src/"
    links "%{cfg.buildcfg}/glfw3.lib"
    libdirs "%{prj.location}/lib/"
    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}/")
    objdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}/int/")

    filter "platforms:Win32"
        architecture "x86"

    filter "platforms:Win64"
        architecture "x86_64"

    filter "configurations:Debug"
        symbols "On"
        optimize "Off"

    filter "configurations:Release"
        symbols "Off"
        optimize "On"

project "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    systemversion "latest"
    debugformat "c7"

    location "../Sandbox/"
    files { "%{prj.location}/src/**.cpp", "%{prj.location}/src/**.h", "%{prj.location}/src/**.c" }
    includedirs { "%{prj.location}/src/", "../Chemical/src/" }
    links "Chemical"
    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}/")
    objdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}/int/")

    filter "platforms:Win32"
        architecture "x86"

    filter "platforms:Win64"
        architecture "x86_64"

    filter "configurations:Debug"
        symbols "On"
        optimize "Off"

    filter "configurations:Release"
        symbols "Off"
        optimize "On"

    filter { "platforms:Win64", "configurations:Debug"}
        postbuildcommands "$(SolutionDir)generate_solution/copy_res_debug_x86_64.bat"
    filter { "platforms:Win32", "configurations:Debug"}
        postbuildcommands "$(SolutionDir)generate_solution/copy_res_debug_x86.bat"
    filter { "platforms:Win64", "configurations:Release"}
        postbuildcommands "$(SolutionDir)generate_solution/copy_res_release_x86_64.bat"
    filter { "platforms:Win32", "configurations:Debug"}
        postbuildcommands "$(SolutionDir)generate_solution/copy_res_release_x86.bat"
