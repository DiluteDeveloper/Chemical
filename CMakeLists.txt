# setting up chemical project

cmake_minimum_required(VERSION 3.15)

project(Chemical VERSION 1.0)

set(CMAKE_CONFIGURATION_TYPES
Debug;Release;RelWithDebInfo;MinSizeRel
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(C_STANDARD 11)
set(C_STANDARD_REQUIRED True)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_SHARED_LIBS OFF CACHE BOOL "" FORCE)

set(PROJECT ${PROJECT_NAME})
set(PROJECT_DIR ${CMAKE_SOURCE_DIR})
set(ENGINE engine)
set(ENGINE_DIR ${CMAKE_SOURCE_DIR}/engine)
set(SANDBOX sandbox)
set(SANDBOX_DIR ${CMAKE_SOURCE_DIR}/sandbox)
set(GLAD glad)
set(GLAD_DIR ${CMAKE_SOURCE_DIR}/vendor/glad)
set(GLFW glfw)
set(GLFW_DIR ${CMAKE_SOURCE_DIR}/glfw)

set(VS_STARTUP_PROJECT ${SANDBOX})

set(ENGINE_SOURCES
    "engine/src/main.cpp"
    "engine/src/filestream.cpp"
    "engine/src/logger.cpp"
    "engine/src/api/mesh.cpp"
)
set(ENGINE_HEADERS
    ${PROJECT_DIR}/vendor
    ${ENGINE_DIR}/src
)
set(ENGINE_LIBS
   ${GLFW}
   ${GLAD}
)
set(SANDBOX_SOURCES
    "sandbox/src/main.cpp"
)
set(SANDBOX_HEADERS

)
set(SANDBOX_LIBS
    ${ENGINE}
)

add_library(${GLAD} ${GLAD_DIR}/glad.c)
add_library(${ENGINE} ${ENGINE_SOURCES})
add_executable(${SANDBOX} ${SANDBOX_SOURCES})
add_subdirectory(${GLFW_DIR})



target_include_directories(${ENGINE} PRIVATE ${ENGINE_HEADERS})
target_include_directories(${SANDBOX} PRIVATE ${SANDBOX_HEADERS})




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
${ENGINE_DIR}/src/api/entry_point.h
${ENGINE_DIR}/src/api/mesh.h
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


