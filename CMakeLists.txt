# setting up chemical project

cmake_minimum_required(VERSION 3.15)

project(Chemical VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(C_STANDARD 11)
set(C_STANDARD_REQUIRED True)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(PROJECTS ${PROJECT_NAME})
set(PROJECT_DIR ${CMAKE_SOURCE_DIR})
set(ENGINE engine)
set(ENGINE_DIR ${PROJECT_DIR}/projects/engine)
set(SANDBOX sandbox)
set(SANDBOX_DIR ${PROJECT_DIR}/projects/sandbox)
set(GLAD glad)
set(GLAD_DIR ${ENGINE_DIR}/vendor/glad)
set(GLFW glfw)
set(GLFW_DIR ${PROJECT_DIR}/glfw)

# remove glob if you want compatibility
file(GLOB_RECURSE ENGINE_ALL_FILES CONFIGURE_DEPENDS
	${ENGINE_DIR}/src/*.c
	${ENGINE_DIR}/src/*.cpp
	${ENGINE_DIR}/src/*.h
	${ENGINE_DIR}/src/*.hpp
	${ENGINE_DIR}/vendor/*.h
	${ENGINE_DIR}/vendor/*.hpp

)
set(ENGINE_INCLUDE_DIRS
    ${ENGINE_DIR}/vendor
    ${ENGINE_DIR}/src
)
set(ENGINE_LIBS
   ${GLFW}
   ${GLAD}
)
# remove glob if you want compatibility
file(GLOB_RECURSE SANDBOX_ALL_FILES CONFIGURE_DEPENDS
	${SANDBOX_DIR}/src/*.c
	${SANDBOX_DIR}/src/*.cpp
	${SANDBOX_DIR}/src/*.h
	${SANDBOX_DIR}/src/*.hpp
	${SANDBOX_DIR}/vendor/*.h
	${SANDBOX_DIR}/vendor/*.hpp
)
set(SANDBOX_INCLUDE_DIRS
    ${ENGINE_DIR}/src/api
    ${SANDBOX_DIR}/vendor
    ${SANDBOX_DIR}/src
)
set(SANDBOX_LIBS
    ${ENGINE}
)

add_library(${GLAD} ${GLAD_DIR}/glad.c)
add_library(${ENGINE} ${ENGINE_ALL_FILES})
add_executable(${SANDBOX} ${SANDBOX_ALL_FILES})
add_subdirectory(${GLFW_DIR})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${SANDBOX})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_SHARED_LIBS OFF CACHE BOOL "" FORCE)



target_include_directories(${ENGINE} PRIVATE ${ENGINE_INCLUDE_DIRS})
target_include_directories(${SANDBOX} PRIVATE ${SANDBOX_INCLUDE_DIRS})

# setup VS filters

foreach(source IN LISTS ENGINE_ALL_FILES)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "${CMAKE_CURRENT_LIST_DIR}/" "" source_path_relative "${source_path}")
    string(REPLACE "/" "\\" source_path_msvc "${source_path_relative}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()

foreach(source IN LISTS SANDBOX_ALL_FILES)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "${CMAKE_CURRENT_LIST_DIR}/" "" source_path_relative "${source_path}")
    string(REPLACE "/" "\\" source_path_msvc "${source_path_relative}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()

set_target_properties(
${GLAD}
PROPERTIES FOLDER dependencies
)
set_target_properties(
${GLFW} uninstall update_mappings
PROPERTIES FOLDER dependencies/glfw
)

# rest of setup

target_link_libraries(${ENGINE} ${ENGINE_LIBS})
target_link_libraries(${SANDBOX} ${SANDBOX_LIBS})

target_compile_definitions(${ENGINE} PRIVATE GLFW_INCLUDE_NONE) # might not work

set(PCH_STD
<iostream>
<memory>
<utility>
<algorithm>
<functional>
<cstdint>

<string>
<sstream>
<array>
<vector>
<unordered_map>
<unordered_set>

<chrono>
<filesystem>
<fstream>
<initializer_list>
<type_traits>

<Windows.h>
)
set(PCH_SANDBOX

)
set(PCH_ENGINE

)

# setting includes and precompiled header
target_precompile_headers(${SANDBOX} 
PRIVATE ${PCH_STD}
PRIVATE ${PCH_SANDBOX}
)

target_precompile_headers(${ENGINE} 
PRIVATE ${PCH_STD}
PRIVATE ${PCH_ENGINE}
)


