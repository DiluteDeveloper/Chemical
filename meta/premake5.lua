workspace "Chemical"
    location "../"
    configurations { "Debug", "Release"}
    platforms "Win64"
    startproject "Chemical"
    architecture "x86_64"
    language "C"
    systemversion "latest"
    debugformat "c7"

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

project "GLAD"
    kind "StaticLib"


    location (THIRD_PARTY_DIR)
    files { GLAD_SRC_DIR .. "/**.c", GLAD_SRC_DIR .. "/**.h", GLAD_SRC_DIR .."/**.hpp"}
    includedirs(GLAD_INC_DIR .. "/GLAD/")
    targetdir (BIN_DIR)
    objdir (INT_DIR)

project "GLFW"
    kind "StaticLib"

    defines "_GLFW_WIN32"


    location (THIRD_PARTY_DIR)
    files { GLFW_SRC_DIR .. "/**.c", GLFW_SRC_DIR .."/**.h", GLFW_SRC_DIR .."/**.hpp"}
    targetdir (BIN_DIR)
    objdir (INT_DIR)

project "Chemical"
    kind "ConsoleApp"

    defines "GLFW_INCLUDE_NONE"

    location (CHEMICAL_DIR)
    files {CHEMICAL_DIR .. "/resources/**", CHEMICAL_DIR .. "/src/**"}
    includedirs {CHEMICAL_DIR .. "/src/", CHEMICAL_DIR .. "/vendor/", GLAD_INC_DIR, GLFW_INC_DIR}

    links {"GLFW", "GLAD"}

    targetdir (BIN_DIR)
    objdir (INT_DIR)

    filter "configurations:Debug"
        defines "CHEMICAL_DEBUG"