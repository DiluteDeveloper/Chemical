cmake_minimum_required(VERSION 4.2.3)
project(ChemicalMC LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_FLAGS "-pthread")
set(CMAKE_CXX_FLAGS "-pthread -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")

# ---------------------------------------------------------------------------
# Dependencies
# ---------------------------------------------------------------------------
include(FetchContent)

# Native target only ///////////////////////////////////////

FetchContent_Declare(glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.5.1
    GIT_SHALLOW    FALSE
)

FetchContent_Declare(glad_generator
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG        v2.0.8
    GIT_SHALLOW    TRUE
    SOURCE_SUBDIR  cmake
)


# //////////////////////////////////////////////////



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

if(DEFINED EMSCRIPTEN)
    set(OPENGL_MAJOR_VERSION 3)
    set(OPENGL_MINOR_VERSION 0)
    FetchContent_MakeAvailable(glm spdlog nlohmann_json stb imgui)

else()
    set(OPENGL_MAJOR_VERSION 4)
    set(OPENGL_MINOR_VERSION 5)
    FetchContent_MakeAvailable(glm spdlog nlohmann_json stb imgui glfw glad_generator)
    glad_add_library(glad_gl_core STATIC LOADER API gl:core=${OPENGL_MAJOR_VERSION}.${OPENGL_MINOR_VERSION})
endif()

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
target_link_libraries(imgui PRIVATE glfw)
target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

if(DEFINED EMSCRIPTEN)
    target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_ES3)
    target_compile_options(imgui PUBLIC -sUSE_GLFW=3)
endif()

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

if (DEFINED EMSCRIPTEN)
    set(TARGET ${PROJECT_NAME}_wasm)
    add_executable(${TARGET} ${SOURCE_FILES})
    target_link_options(${TARGET} PRIVATE
        -sWASM=1
        -sUSE_WEBGL2=1
        -sUSE_GLFW=3
        -sFULL_ES3=1
        --preload-file ${CMAKE_SOURCE_DIR}/res@/res
    )

    target_link_libraries(${TARGET} PRIVATE GL glm::glm spdlog::spdlog nlohmann_json::nlohmann_json imgui stb)

    target_compile_definitions(${TARGET} PRIVATE
        GLM_ENABLE_EXPERIMENTAL
        # CHEMICAL_USE_IMGUI
        OPENGL_MAJOR_VERSION=${OPENGL_MAJOR_VERSION}
        OPENGL_MINOR_VERSION=${OPENGL_MINOR_VERSION}
    )
    set_target_properties(${TARGET} PROPERTIES SUFFIX ".js")
else()
    set(TARGET ${PROJECT_NAME}_native)
    add_executable(${TARGET} ${SOURCE_FILES})

    target_link_libraries(${TARGET} PRIVATE GL glm::glm spdlog::spdlog nlohmann_json::nlohmann_json imgui stb glfw glad_gl_core)
    target_compile_definitions(${TARGET} PRIVATE
        GLM_ENABLE_EXPERIMENTAL
        # CHEMICAL_USE_IMGUI
        GLAD
        OPENGL_MAJOR_VERSION=${OPENGL_MAJOR_VERSION}
        OPENGL_MINOR_VERSION=${OPENGL_MINOR_VERSION}
    )
    add_custom_command(TARGET ${TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/res
                ${CMAKE_CURRENT_BINARY_DIR}/res)
endif()

target_include_directories(${TARGET} PRIVATE src/)

target_compile_options(${TARGET} PRIVATE
    -O0
    -g
)

