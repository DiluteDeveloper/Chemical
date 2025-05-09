workspace "Chemical"
    location "../"
    configurations { "Debug", "Release"}
    platforms "Win64"
    startproject "Chemical"
    architecture "x86_64"
    language "C++"
    cppdialect "C++20"
    systemversion "latest"
    debugformat "c7"
    toolset "clang"

    filter "configurations:Debug"
        symbols "On"
        optimize "Off"

    filter "configurations:Release"
        symbols "Off"
        optimize "On"


BIN_DIR = "%{wks.location}/bin/%{prj.name}/%{cfg.platform}-%{cfg.buildcfg}"
INT_DIR = BIN_DIR .. "/int"

MODULES_DIR = "%{wks.location}/modules"
IMPORTED_MODULES_DIR = MODULES_DIR .. "/imported"



CHEMICAL_DIR = MODULES_DIR .. "/chemical"
CHEMICAL_INCLUDE_DIR = CHEMICAL_DIR .. "/include"
CHEMICAL_SOURCE_DIR = CHEMICAL_DIR .. "/src"

SANDBOX_DIR = MODULES_DIR .. "/sandbox"
SANDBOX_SOURCE_DIR = SANDBOX_DIR .. "/src"

filter "configurations:Debug"
    GLFW_LIB_DIR = IMPORTED_MODULES_DIR .. "/glfw/lib/debug"
filter "configurations:Release"
    GLFW_LIB_DIR = IMPORTED_MODULES_DIR .. "/glfw/lib/release"

GLFW_LIB = "glfw3"

project "Chemical"
    kind "StaticLib"

    pchheader "pch.h"
    pchsource "pch.cpp" -- relative to this script: has to be changed manually

    defines "GLFW_INCLUDE_NONE"

    location (CHEMICAL_DIR)
    files {CHEMICAL_DIR .. "/**"}
    includedirs {IMPORTED_MODULES_DIR, CHEMICAL_SOURCE_DIR, CHEMICAL_INCLUDE_DIR}

    libdirs (GLFW_LIB_DIR)
    links (GLFW_LIB)

    targetdir (BIN_DIR)
    objdir (INT_DIR)


project "Sandbox"
    kind "ConsoleApp"

    location (SANDBOX_DIR)
    files {SANDBOX_SOURCE_DIR .. "/**"}
    includedirs {SANDBOX_SOURCE_DIR, CHEMICAL_INCLUDE_DIR}

    links ("Chemical")

    targetdir (BIN_DIR)
    objdir (INT_DIR)
