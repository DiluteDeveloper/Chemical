workspace("Chemical")
location("../")
configurations({ "Debug", "Release" })
platforms("Win64")
startproject("Chemical")
architecture("x86_64")
language("C++")
cppdialect("C++23")
systemversion("latest")
debugformat("c7")
toolset("clang")

filter("configurations:Debug")
symbols("On")
optimize("Off")

filter("configurations:Release")
symbols("Off")
optimize("On")

BIN_DIR = "%{wks.location}/bin/%{prj.name}/%{cfg.platform}-%{cfg.buildcfg}"
INT_DIR = BIN_DIR .. "/int"

MODULES_DIR = "%{wks.location}/modules"
IMPORTED_MODULES_DIR = MODULES_DIR .. "/imported"

CHEMICAL_DIR = MODULES_DIR .. "/chemical"
CHEMICAL_INCLUDE_DIR = CHEMICAL_DIR .. "/include"
CHEMICAL_SOURCE_DIR = CHEMICAL_DIR .. "/src"

SANDBOX_DIR = MODULES_DIR .. "/sandbox"
SANDBOX_SOURCE_DIR = SANDBOX_DIR .. "/src"

GLFW_LIB_DIR = IMPORTED_MODULES_DIR .. "/glfw"
GLAD_LIB_DIR = IMPORTED_MODULES_DIR .. "/glad"
GLFW_LIB = "glfw3"
GLAD_LIB = "glad"

project("Chemical")
kind("StaticLib")

-- pchheader("pch.h")
-- pchsource("../modules/chemical/src/pch.cpp") -- relative to this script: has to be changed manually

defines("GLFW_INCLUDE_NONE")

location(CHEMICAL_DIR)
files({ CHEMICAL_DIR .. "/src/**", CHEMICAL_DIR .. "/include/**" })
includedirs({ IMPORTED_MODULES_DIR, CHEMICAL_SOURCE_DIR, CHEMICAL_INCLUDE_DIR })

libdirs({ GLFW_LIB_DIR, GLAD_LIB_DIR })
links({ GLFW_LIB, GLAD_LIB })

targetdir(BIN_DIR)
objdir(INT_DIR)

project("Sandbox")
kind("ConsoleApp")

location(SANDBOX_DIR)
files({ SANDBOX_SOURCE_DIR .. "/**" })

-- Imported modules inclusion is a quick-fix for glm
includedirs({ SANDBOX_SOURCE_DIR, CHEMICAL_INCLUDE_DIR, IMPORTED_MODULES_DIR })

libdirs({ GLFW_LIB_DIR, GLAD_LIB_DIR })
links({ "Chemical", GLFW_LIB, GLAD_LIB })

targetdir(BIN_DIR)
objdir(INT_DIR)
