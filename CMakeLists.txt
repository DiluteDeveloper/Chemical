# Dependencies (native, web)
    # cmake (sudo apt install cmake)
# Dependencies (native)
    # For Dawn
        # sudo apt-get install libxrandr-dev libxinerama-dev libxcursor-dev mesa-common-dev libx11-xcb-dev pkg-config nodejs npm
# Dependencies (web)
    # Emscripten >= 6.0.13 (apt repo version is too old)

# All in one compile commands
    # Native (in root directory)
        # cmake -S . -B build/ -DUSE_MOLD=false -DCMAKE_BUILD_TYPE=Release && cd build && cmake --build . -j$(nproc)
    # Web (in root directory)
        # emcmake cmake -S . -B build/ -DUSE_MOLD=false -DCMAKE_BUILD_TYPE=Release && cd build && cmake --build . -j$(nproc) && python3 -m http.server

# Development Quick Commands (Linux) (Mainly for personal development use)
    # Dependencies (native, web)
        # mold linker (sudo apt install mold) for faster link times
        # bear (sudo apt install bear) for lsp symbols
        # ninja (sudo apt install generate-ninja) for faster compilation

    # Configure (web)
    # emcmake cmake -S . -B build_es/ -G Ninja -DCMAKE_BUILD_TYPE=Debug -DUSE_MOLD=true
    # Configure (native)
    # cmake -S . -B build/ -DCMAKE_CXX_FLAGS="-DFORCE_X11" -G Ninja -DCMAKE_BUILD_TYPE=Debug -DUSE_MOLD=true
    # Build and Run (native)
        # bear -- cmake --build . -j$(nproc) && mv compile_commands.json ../ && ./ChemicalMC_PK
    # Build and Run (web)
        # bear -- cmake --build . -j$(nproc) && mv compile_commands.json ../ && python3 -m http.server
    # Build
        # bear -- cmake --build . -j$(nproc) && mv compile_commands.json ../
    # Emscripten latest version
        # source ~/dev/emsdk/emsdk_env.sh
    # Copy to windows build (personal)
    # cp -r ../src /mnt/c/Users/Dilute/Documents/ChemicalMC_PK && cp -r ../res /mnt/c/Users/Dilute/Documents/ChemicalMC_PK/build/Debug

cmake_minimum_required(VERSION 4.2.3)
project(ChemicalMC_PK
    VERSION 0.1.1
    LANGUAGES CXX C
)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

# Breaks MSVC as different flags exist ------
if(NOT WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

if(USE_MOLD)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread -fuse-ld=mold")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread -fuse-ld=mold")
else()
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")
endif()

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

include(FetchContent)

# if(NOT EMSCRIPTEN)
#     FetchContent_Declare(glfw
#         GIT_REPOSITORY https://github.com/glfw/glfw.git
#         GIT_TAG        3.5.1
#         GIT_SHALLOW    FALSE
#     )
# endif()

FetchContent_Declare(glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.1
    GIT_SHALLOW    TRUE
)

FetchContent_Declare(spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.17.0
    GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(glm spdlog)

add_executable(${PROJECT_NAME} 
    src/main.cpp 
    src/app.cpp
    src/utils/webgpu_utils.cpp
    src/input.cpp
    src/camera.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm spdlog::spdlog)

if(EMSCRIPTEN)
    target_compile_options(${PROJECT_NAME} PRIVATE --use-port=emdawnwebgpu)
    target_link_options(${PROJECT_NAME} PRIVATE --use-port=emdawnwebgpu --use-port=contrib.glfw3 -sASYNCIFY --preload-file ${CMAKE_SOURCE_DIR}/res@/res)
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
else()
    FetchContent_Declare(dawn
        GIT_REPOSITORY https://github.com/google/dawn.git
        GIT_TAG v20261104.212356
        GIT_SHALLOW true
        GIT_SUBMODULES ""
    )
    set(DAWN_ENABLE_DESKTOP_GL OFF CACHE BOOL "" FORCE)
    set(DAWN_ENABLE_OPENGLES   OFF CACHE BOOL "" FORCE)
    set(DAWN_ENABLE_NULL       OFF CACHE BOOL "" FORCE)
    set(TINT_BUILD_GLSL_WRITER OFF CACHE BOOL "" FORCE)
    set(TINT_BUILD_GLSL_VALIDATOR OFF CACHE BOOL "" FORCE)
    set(DAWN_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
    set(TINT_BUILD_CMD_TOOLS OFF CACHE BOOL "" FORCE)
    set(TINT_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(DAWN_FETCH_DEPENDENCIES ON CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(dawn)

    target_link_libraries(${PROJECT_NAME} PRIVATE dawn::webgpu_dawn dawn_glfw glfw)

    # target_compile_definitions(${PROJECT_NAME} GLFW_INCLUDE_NONE)
endif()


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/res
            ${CMAKE_CURRENT_BINARY_DIR}/res)

# ---------------------------------------------------------------------------
# Dependencies
# ---------------------------------------------------------------------------
# include(FetchContent)

# Native target only ///////////////////////////////////////

# FetchContent_Declare(glfw
#     GIT_REPOSITORY https://github.com/glfw/glfw.git
#     GIT_TAG        3.5.1
#     GIT_SHALLOW    FALSE
# )
#
# FetchContent_Declare(glad_generator
#     GIT_REPOSITORY https://github.com/Dav1dde/glad.git
#     GIT_TAG        v2.0.8
#     GIT_SHALLOW    TRUE
#     SOURCE_SUBDIR  cmake
# )


# //////////////////////////////////////////////////




#
# set(JSON_BuildTests OFF CACHE INTERNAL "")
# FetchContent_Declare(nlohmann_json
#     GIT_REPOSITORY https://github.com/nlohmann/json.git
#     GIT_TAG        v3.11.3
#     GIT_SHALLOW    TRUE
# )

# FetchContent_Declare(stb
#     GIT_REPOSITORY https://github.com/nothings/stb.git
#     GIT_TAG        master
#     GIT_SHALLOW    TRUE
# )
#
# FetchContent_Declare(imgui
#     GIT_REPOSITORY https://github.com/ocornut/imgui.git
#     GIT_TAG        v1.91.9
#     GIT_SHALLOW    TRUE
# )

# if(DEFINED EMSCRIPTEN)
#     set(OPENGL_MAJOR_VERSION 3)
#     set(OPENGL_MINOR_VERSION 0)
#     FetchContent_MakeAvailable(glm spdlog nlohmann_json stb imgui)
#
# else()
#     set(OPENGL_MAJOR_VERSION 4)
#     set(OPENGL_MINOR_VERSION 5)
#     FetchContent_MakeAvailable(glm spdlog nlohmann_json stb imgui glfw glad_generator)
#     glad_add_library(glad_gl_core STATIC LOADER API gl:core=${OPENGL_MAJOR_VERSION}.${OPENGL_MINOR_VERSION})
# endif()
#
# add_library(stb INTERFACE)
# target_include_directories(stb INTERFACE ${stb_SOURCE_DIR})
# # Dear ImGui: no CMake of its own, so build it with the GLFW + OpenGL3 backends.
# add_library(imgui STATIC
#     ${imgui_SOURCE_DIR}/imgui.cpp
#     ${imgui_SOURCE_DIR}/imgui_demo.cpp
#     ${imgui_SOURCE_DIR}/imgui_draw.cpp
#     ${imgui_SOURCE_DIR}/imgui_tables.cpp
#     ${imgui_SOURCE_DIR}/imgui_widgets.cpp
#     ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
#     ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
# )
# target_link_libraries(imgui PRIVATE glfw)
# target_include_directories(imgui PUBLIC
#     ${imgui_SOURCE_DIR}
#     ${imgui_SOURCE_DIR}/backends
# )
#
# if(DEFINED EMSCRIPTEN)
#     target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_ES3)
#     target_compile_options(imgui PUBLIC -sUSE_GLFW=3)
# endif()
#
# # ---------------------------------------------------------------------------
# # Main target
# # ---------------------------------------------------------------------------
# set(SOURCE_FILES
#     src/core/core.cpp
#     # src/core/resource_manager.cpp
#
#     src/graphics/opengl/buffer.cpp
#     src/graphics/opengl/misc.cpp
#     src/graphics/opengl/shader_program.cpp
#     src/graphics/opengl/texture.cpp
#     src/graphics/opengl/vertex_array.cpp
#     src/graphics/renderer.cpp
#
#     src/transform/transform.cpp
#
#     src/util/filestream.cpp
#     src/util/image_loader.cpp
#
#     src/world/chunk/block_grid.cpp
#     src/world/chunk/height_map.cpp
#     src/world/chunk/loader.cpp
#     src/world/chunk/mesh.cpp
#     src/world/chunk/threaded.cpp
#     # src/chunk_system.cpp
# )
#
# if (DEFINED EMSCRIPTEN)
#     set(TARGET ${PROJECT_NAME}_wasm)
#     add_executable(${TARGET} ${SOURCE_FILES})
#     target_link_options(${TARGET} PRIVATE
#         -sWASM=1
#         -sUSE_WEBGL2=1
#         -sUSE_GLFW=3
#         -sFULL_ES3=1
#         --preload-file ${CMAKE_SOURCE_DIR}/res@/res
#     )
#
#     target_link_libraries(${TARGET} PRIVATE GL glm::glm spdlog::spdlog nlohmann_json::nlohmann_json imgui stb)
#
#     target_compile_definitions(${TARGET} PRIVATE
#         GLM_ENABLE_EXPERIMENTAL
#         # CHEMICAL_USE_IMGUI
#         OPENGL_MAJOR_VERSION=${OPENGL_MAJOR_VERSION}
#         OPENGL_MINOR_VERSION=${OPENGL_MINOR_VERSION}
#         GLFW_INCLUDE_NONE
#     )
#     set_target_properties(${TARGET} PROPERTIES SUFFIX ".js")
# else()
#     set(TARGET ${PROJECT_NAME}_native)
#     add_executable(${TARGET} ${SOURCE_FILES})
#
#     target_link_libraries(${TARGET} PRIVATE GL glm::glm spdlog::spdlog nlohmann_json::nlohmann_json imgui stb glfw glad_gl_core)
#     target_compile_definitions(${TARGET} PRIVATE
#         GLM_ENABLE_EXPERIMENTAL
#         # CHEMICAL_USE_IMGUI
#         GLAD
#         OPENGL_MAJOR_VERSION=${OPENGL_MAJOR_VERSION}
#         OPENGL_MINOR_VERSION=${OPENGL_MINOR_VERSION}
#         GLFW_INCLUDE_NONE
#     )
#     add_custom_command(TARGET ${TARGET} POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E copy_directory
#                 ${CMAKE_CURRENT_SOURCE_DIR}/res
#                 ${CMAKE_CURRENT_BINARY_DIR}/res)
# endif()
#
# target_include_directories(${TARGET} PRIVATE src/)
#
# target_compile_options(${TARGET} PRIVATE
#     -O0
#     -g
# )
#
