workspace("Chemical")
location("../")
configurations({ "Debug", "Release" })
startproject("Chemical")
architecture("x86_64")
language("C")
cdialect("C99")
systemversion("latest")
toolset("clang")

filter("configurations:Debug")
symbols("On")
optimize("Off")

filter("configurations:Release")
symbols("Off")
optimize("On")

BIN_DIR = "%{wks.location}/bin/%{prj.name}/%{cfg.system}-%{cfg.buildcfg}"
INT_DIR = BIN_DIR .. "/int"

MODULES_DIR = "%{wks.location}/modules"
IMPORTED_MODULES_DIR = MODULES_DIR .. "/imported"

CHEMICAL_DIR = MODULES_DIR .. "/chemical"
CHEMICAL_SOURCE_DIR = CHEMICAL_DIR .. "/src"

GLFW_LIB_DIR = IMPORTED_MODULES_DIR .. "/glfw"
GLAD_LIB_DIR = IMPORTED_MODULES_DIR .. "/glad"
GLFW_LIB = "glfw3"
GLAD_LIB = "glad"

project("Chemical")
kind("ConsoleApp")

defines("GLFW_INCLUDE_NONE")

location(CHEMICAL_DIR)
files({
	CHEMICAL_SOURCE_DIR .. "/**",
})
includedirs({ IMPORTED_MODULES_DIR, CHEMICAL_SOURCE_DIR })

libdirs({ GLFW_LIB_DIR, GLAD_LIB_DIR })

filter("system:windows")
links({ "OpenGL32", GLFW_LIB, GLAD_LIB })

filter("system:linux")
links({ "m", GLFW_LIB, GLAD_LIB })

targetdir(BIN_DIR)
objdir(INT_DIR)
