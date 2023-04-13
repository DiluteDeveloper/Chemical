# setting up chemical project

cmake_minimum_required(VERSION 3.15)

project(Chemical VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(C_STANDARD 11)
set(C_STANDARD_REQUIRED True)

set(CHEMICAL chemical)
set(CHEMICAL_DIR ${CMAKE_SOURCE_DIR}/chemical)

set(VS_STARTUP_PROJECT Chemical)

# glad setup

set(GLAD_DIR ${CHEMICAL_DIR}/vendor/glad)
add_library(glad ${GLAD_DIR}/glad.c)

# glfw setup

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${CMAKE_SOURCE_DIR}/glfw)


# setting up chemical subproject

set(CHEMICAL_SOURCES
    ${CHEMICAL_DIR}/src/main.cpp
    ${CHEMICAL_DIR}/src/engine.cpp
    ${CHEMICAL_DIR}/src/filestream.cpp
    ${CHEMICAL_DIR}/src/graphics/Mesh.cpp
    ${CHEMICAL_DIR}/src/logger.cpp
)
set(CHEMICAL_HEADERS
    ${CHEMICAL_DIR}/vendor
    ${CHEMICAL_DIR}/src
)
set(CHEMICAL_LIBS
   glfw
   glad
)
add_executable(${CHEMICAL} ${CHEMICAL_SOURCES})
target_include_directories(${CHEMICAL} PRIVATE ${CHEMICAL_HEADERS})
target_link_libraries(${CHEMICAL} ${CHEMICAL_LIBS})

target_compile_definitions(${CHEMICAL} PRIVATE GLFW_INCLUDE_NONE) # might not work

# setting includes and precompiled header

target_precompile_headers(${CHEMICAL} PRIVATE
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


