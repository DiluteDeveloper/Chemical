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
RES_SRC_DIR = "%{wks.location}/resources"
RES_DEST_DIR = BIN_DIR .. "/resources"
LIB_DIR = "%{wks.location}/libs/%{cfg.platform}-%{cfg.buildcfg}"

CHEMICAL_DIR = "%{wks.location}/projects/chemical"

--SANDBOX_DIR = "%{wks.location}/projects/sandbox"
--SANDBOX_SRC_DIR = "%{wks.location}/projects/sandbox"

THIRD_PARTY_DIR = "%{wks.location}/3rdparty"
GLAD_SRC_DIR = "%{wks.location}/3rdparty/glad/src"
GLAD_INC_DIR = "%{wks.location}/3rdparty/glad/include"
GLFW_SRC_DIR = "%{wks.location}/3rdparty/glfw/src"
GLFW_INC_DIR = "%{wks.location}/3rdparty/glfw/include"
IMGUI_SRC_DIR = "%{wks.location}/3rdparty/imgui"
IMGUI_INC_DIR = "%{wks.location}/3rdparty/imgui"

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
files { IMGUI_SRC_DIR .. "/*.c", IMGUI_SRC_DIR .."/*.h", IMGUI_SRC_DIR .. "/*.cpp", IMGUI_SRC_DIR .."/*.hpp", IMGUI_SRC_DIR .. "/*.m",
        IMGUI_SRC_DIR .. "/backends/imgui_impl_glfw.cpp", IMGUI_SRC_DIR .. "/backends/imgui_impl_opengl3.cpp"}
includedirs {IMGUI_INC_DIR, GLFW_INC_DIR}
targetdir (BIN_DIR)
objdir (INT_DIR)

project "Chemical"
    kind "ConsoleApp"

    pchheader "pch.h"
    pchsource ("../projects/chemical/src/pch.cpp") -- relative to script: has to be changed manually

    defines "GLFW_INCLUDE_NONE"

    location (CHEMICAL_DIR)
    files {CHEMICAL_DIR .. "/include/**", CHEMICAL_DIR .. "/resources/**", CHEMICAL_DIR .. "/src/**"}
    includedirs {CHEMICAL_DIR .. "/src/", CHEMICAL_DIR .. "/vendor/", GLAD_INC_DIR, GLFW_INC_DIR, IMGUI_INC_DIR}
    links {"GLFW", "GLAD", "IMGUI"}
    targetdir (BIN_DIR)
    objdir (INT_DIR)

    filter "configurations:Debug"
        defines "CHEMICAL_DEBUG"

    -- Copy resources to project directory and to bin
    postbuildcommands {
        "call %{wks.location}/meta/copy_resources.bat " .. string.gsub(CHEMICAL_DIR, "/", "\\") .. "\\resources " .. string.gsub(BIN_DIR, "/", "\\") .. "\\resources"                
    }

--[[project "Sandbox"
    kind "ConsoleApp"

    location (SANDBOX_DIR)
    files { SANDBOX_DIR .. "/src/**.cpp", SANDBOX_DIR .. "/src/**.h", 
    SANDBOX_DIR .. "/vendor/**.h", SANDBOX_DIR .. "/vendor/**.hpp"}
    includedirs { SANDBOX_DIR, CHEMICAL_INC_DIR}
    links {"GLFW", "GLAD", "Chemical"}
    targetdir (BIN_DIR)
    objdir (INT_DIR)

    postbuildcommands ("%{wks.location}meta\\copy_resources.bat %{wks.location}resources " .. string.gsub(BIN_DIR, "/", "\\") .. "\\resources")]]--
