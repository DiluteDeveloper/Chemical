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

CHEMICAL_DIR = "%{wks.location}/targets/chemical"
SANDBOX_DIR = "%{wks.location}/targets/sandbox"
GLAD_DIR = "%{wks.location}/targets/glad"
GLFW_DIR = "%{wks.location}/targets/glfw"

CHEMICAL_INCLUDE_DIR = CHEMICAL_DIR .. "/include"
GLAD_INCLUDE_DIR = GLAD_DIR .. "/include"
GLFW_INCLUDE_DIR = GLFW_DIR .. "/include"

PCH_SRC = "../targets/chemical/src/pch.cpp"

COPY_RESOURCES_COMMAND = "%{wks.location}meta\\copy_resources.bat %{wks.location}resources %{wks.location}bin\\%{prj.name}\\%{cfg.platform}-%{cfg.buildcfg}\\resources"
project "GLAD"
    kind "StaticLib"


    location (GLAD_DIR)
    files { GLAD_DIR .. "/src/**.c", GLAD_DIR .."/src/**.h", GLAD_DIR .. "/src/**.cpp", GLAD_DIR .."/src/**.hpp", GLAD_DIR .. "/src/**.m"}
    includedirs {GLAD_DIR, GLAD_INCLUDE_DIR}
    targetdir (BIN_DIR)
    objdir (INT_DIR)

project "GLFW"
    kind "StaticLib"

    defines "_GLFW_WIN32"


    location (GLFW_DIR)
    files { GLFW_DIR .. "/src/**.c", GLFW_DIR .."/src/**.h", GLFW_DIR .. "/src/**.cpp", GLFW_DIR .."/src/**.hpp", GLFW_DIR .. "/src/**.m"}
    includedirs {GLFW_DIR, GLFW_INCLUDE_DIR}
    targetdir (BIN_DIR)
    objdir (INT_DIR)

project "Chemical"
    kind "StaticLib"

    pchheader "pch.h"
    pchsource (PCH_SRC)

    defines "GLFW_INCLUDE_NONE"

    location (CHEMICAL_DIR)
    files { CHEMICAL_DIR .. "/src/**.cpp", CHEMICAL_DIR .."/src/**.h", 
    CHEMICAL_DIR .. "/vendor/**.h", CHEMICAL_DIR ..  "/vendor/**.hpp"}
    includedirs {CHEMICAL_DIR, CHEMICAL_INCLUDE_DIR, GLAD_INCLUDE_DIR, GLFW_INCLUDE_DIR}
    links {"GLFW", "GLAD"}
    targetdir (BIN_DIR)
    objdir (INT_DIR)

project "Sandbox"
    kind "ConsoleApp"

    --pchsource (PCH_SRC)

    location (SANDBOX_DIR)
    files { SANDBOX_DIR .. "/src/**.cpp", SANDBOX_DIR .. "/src/**.h", 
    SANDBOX_DIR .. "/vendor/**.h", SANDBOX_DIR .. "/vendor/**.hpp"}
    includedirs { SANDBOX_DIR, CHEMICAL_INCLUDE_DIR }
    links {"GLFW", "GLAD", "Chemical"}
    targetdir (BIN_DIR)
    objdir (INT_DIR)

    postbuildcommands (COPY_RESOURCES_COMMAND)
