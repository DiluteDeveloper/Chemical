cmake_minimum_required(VERSION 4.2.3)
project(ChemicalMC LANGUAGES CXX)

if(NOT EMSCRIPTEN)
    message(FATAL_ERROR "This project targets WebAssembly. Configure with: emcmake cmake -B build")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")

# ---------------------------------------------------------------------------
# Dependencies
# ---------------------------------------------------------------------------
include(FetchContent)

FetchContent_Declare(glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.1
    GIT_SHALLOW    TRUE
)

FetchContent_Declare(spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.15.0
    GIT_SHALLOW    TRUE
)

set(JSON_BuildTests OFF CACHE INTERNAL "")
FetchContent_Declare(nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
    GIT_SHALLOW    TRUE
)

# Header-only / no CMake support: just download, we build targets ourselves.
FetchContent_Declare(stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG        master
    GIT_SHALLOW    TRUE
)

FetchContent_Declare(imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.91.9
    GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(glm spdlog nlohmann_json stb imgui)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${stb_SOURCE_DIR})
# Dear ImGui: no CMake of its own, so build it with the GLFW + OpenGL3 backends.
add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)
target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_ES3)
target_compile_options(imgui PUBLIC -sUSE_GLFW=3)

# ---------------------------------------------------------------------------
# Main target
# ---------------------------------------------------------------------------
set(SOURCE_FILES
    src/core/core.cpp
    # src/core/resource_manager.cpp

    src/graphics/opengl/buffer.cpp
    src/graphics/opengl/misc.cpp
    src/graphics/opengl/shader_program.cpp
    src/graphics/opengl/texture.cpp
    src/graphics/opengl/vertex_array.cpp

    src/transform/transform.cpp

    src/util/filestream.cpp
    src/util/image_loader.cpp

    src/world/chunk/block_grid.cpp
    src/world/chunk/height_map.cpp
    src/world/chunk/loader.cpp
    src/world/chunk/mesh.cpp
    src/world/chunk/threaded.cpp
    # src/chunk_system.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE src/)

set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".js")

target_link_libraries(${PROJECT_NAME} PRIVATE
    GL
    glm::glm
    spdlog::spdlog
    nlohmann_json::nlohmann_json
    imgui
    stb
)

target_compile_options(${PROJECT_NAME} PRIVATE
    -O0
    -g
)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    GLM_ENABLE_EXPERIMENTAL
    # CHEMICAL_USE_IMGUI
)

target_link_options(${PROJECT_NAME} PRIVATE
    -O0
    -g
    -sWASM=1
    -sUSE_WEBGL2=1
    -sUSE_GLFW=3
    -sFULL_ES3=1
    --preload-file ${CMAKE_SOURCE_DIR}/res@/res
)

