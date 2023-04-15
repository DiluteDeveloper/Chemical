workspace "Chemical"
    location "../"
    configurations { "Debug", "Release"}
    platforms {"Win64"}
    startproject "Sandbox"
    architecture "x86_64"
    toolset "clang"

    filter "configurations:Debug"
        symbols "On"
        optimize "Off"

    filter "configurations:Release"
        symbols "Off"
        optimize "On"

    filter "platforms:Win64"
        architecture "x86_64"

BIN_DIR = "%{wks.location}/bin/%{prj.name}/%{cfg.platform}/%{cfg.buildcfg}"
INT_DIR = BIN_DIR .. "/int"
RES_SRC_DIR = "%{wks.location}/resources"
RES_DEST_DIR = BIN_DIR .. "/resources"
LIB_DIR = "%{wks.location}/libs/%{cfg.platform}/%{cfg.buildcfg}"

CHEMICAL_DIR = "%{wks.location}/chemical"
CHEMICAL_LIBS = {"glfw3.lib", "glad.lib"}
SANDBOX_DIR = "%{wks.location}/sandbox"

PCH_SRC = "../chemical/src/pch.c"

COPY_RESOURCES_COMMAND = "%{wks.location}\\meta\\copy_resources.bat %{wks.location}\\resources " .. string.gsub(BIN_DIR, "/", "\\") .. "\\resources"


project "Chemical"
    kind "StaticLib"
    language "C"
    systemversion "latest"
    debugformat "c7"

    pchheader "pch.h"
    pchsource (PCH_SRC)

    defines "GLFW_INCLUDE_NONE"

    location (CHEMICAL_DIR)
    files { CHEMICAL_DIR .. "/src/**.c", CHEMICAL_DIR .."/src/**.h",
    CHEMICAL_DIR .. "/vendor/**.h", CHEMICAL_DIR ..  "/vendor/**.hpp"}
    includedirs {CHEMICAL_DIR .. "/src/", CHEMICAL_DIR .. "/vendor/"}
    links {CHEMICAL_LIBS}
    libdirs (LIB_DIR)
    targetdir (BIN_DIR)
    objdir (INT_DIR)

project "Sandbox"
    kind "ConsoleApp"
    language "C"
    systemversion "latest"
    debugformat "c7"

    --pchsource (PCH_SRC)

    location (SANDBOX_DIR)
    files { SANDBOX_DIR .. "/src/**.c", SANDBOX_DIR .. "/src/**.h", 
    SANDBOX_DIR .. "/vendor/**.h", SANDBOX_DIR .. "/vendor/**.hpp"}
    includedirs { SANDBOX_DIR .. "/src/", SANDBOX_DIR .. "/vendor/", CHEMICAL_DIR .. "/src/API/" }
    links {CHEMICAL_LIBS, "Chemical"}
    libdirs (LIB_DIR)
    targetdir (BIN_DIR)
    objdir (INT_DIR)

    postbuildcommands (COPY_RESOURCES_COMMAND)
