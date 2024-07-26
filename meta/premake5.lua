workspace "Chemical"
    location "../"
    configurations { "Debug", "Release"}
    platforms "Win64"
    startproject "Sandbox"
    architecture "x86_64"
    language "C++"
    cppdialect "C++20"
    systemversion "latest"
    debugformat "c7"
    --toolset "clang"

    filter "configurations:Debug"
        symbols "On"
        optimize "Off"

    filter "configurations:Release"
        symbols "Off"
        optimize "On"

BIN_DIR = "%{wks.location}/bin/%{prj.name}/%{cfg.platform}-%{cfg.buildcfg}"
INT_DIR = BIN_DIR .. "/int"

CHEMICAL_DIR = "%{wks.location}/projects/chemical"

THIRD_PARTY_DIR = "%{wks.location}/3rdparty"

GLAD_SRC_DIR = "%{wks.location}/3rdparty/glad/src"
GLAD_INC_DIR = "%{wks.location}/3rdparty/glad/include"

GLFW_SRC_DIR = "%{wks.location}/3rdparty/glfw/src"
GLFW_INC_DIR = "%{wks.location}/3rdparty/glfw/include"

IMGUI_DIR = "%{wks.location}/3rdparty/imgui/imgui-1.90.9"

ASSIMP_LIB_DIR = "%{wks.location}/3rdparty/assimp/lib/x64"

COPY_RESOURCES_COMMAND = "%{wks.location}meta\\copy_resources.bat %{wks.location}resources %{wks.location}bin\\%{prj.name}\\%{cfg.platform}-%{cfg.buildcfg}\\resources"
project "GLAD"
    kind "StaticLib"


    location (THIRD_PARTY_DIR)
    files { GLAD_SRC_DIR .. "/**.c", GLAD_SRC_DIR .. "/**.h", GLAD_SRC_DIR .. "/**.cpp", GLAD_SRC_DIR .."/**.hpp", GLAD_SRC_DIR .. "/**.m"}
    includedirs(GLAD_INC_DIR .. "/GLAD/")
    targetdir (BIN_DIR)
    objdir (INT_DIR)

project "GLFW"
    kind "StaticLib"

    defines "_GLFW_WIN32"


    location (THIRD_PARTY_DIR)
    files { GLFW_SRC_DIR .. "/**.c", GLFW_SRC_DIR .."/**.h", GLFW_SRC_DIR .. "/**.cpp", GLFW_SRC_DIR .."/**.hpp", GLFW_SRC_DIR .. "/**.m"}
    targetdir (BIN_DIR)
    objdir (INT_DIR)

project "IMGUI"
    kind "StaticLib"


    location (THIRD_PARTY_DIR)
    files {IMGUI_DIR .. "/**.c", IMGUI_DIR .."/**.h", IMGUI_DIR .. "/**.cpp", IMGUI_DIR .."/**.hpp", IMGUI_DIR .. "/**.m"}
    includedirs(GLFW_INC_DIR, IMGUI_DIR)
    targetdir (BIN_DIR)
    objdir (INT_DIR)

project "Chemical"
    kind "ConsoleApp"

    pchheader "pch.h"
    pchsource ("../projects/chemical/src/pch.cpp") -- relative to this script: has to be changed manually

    defines "GLFW_INCLUDE_NONE"

    location (CHEMICAL_DIR)
    files {CHEMICAL_DIR .. "/resources/**", CHEMICAL_DIR .. "/src/**"}
    includedirs {CHEMICAL_DIR .. "/src/", CHEMICAL_DIR .. "/vendor/", GLAD_INC_DIR, GLFW_INC_DIR, IMGUI_DIR, ASSIMP_INC_DIR}
    libdirs {ASSIMP_LIB_DIR}
    links {"GLFW", "GLAD", "IMGUI", "assimp-vc143-mt.lib"}
    targetdir (BIN_DIR)
    objdir (INT_DIR)

    filter "configurations:Debug"
        defines "CHEMICAL_DEBUG"