
#include "character_controller.h"
#include "input.h"
#include "block.h"
#include "level_editor.h"

#include <glm/gtc/matrix_transform.hpp>
#include <spdlog/spdlog.h>
#include <webgpu/webgpu_cpp.h>
#include <webgpu/webgpu_glfw.h>

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif

#include "app.h"

#include "renderers/renderer.h"
#include "utils/webgpu_utils.h"

namespace {

    GLFWwindow* SetupGLFW();
}
namespace Chemical {

    App::App() : window(SetupGLFW()) {
        spdlog::set_level(spdlog::level::trace);

        Input::Initialise(window);

        registry.ctx().emplace<CharacterController>();

        BlockData::AddTestDataToRegistry(registry);
        BoxCollider::AddTestCollidersToRegistry(registry);

        Renderer::Initialise(window, registry);

        LevelEditor::Initialise(registry);
        old_time = clock::now();



#ifdef __EMSCRIPTEN__
        emscripten_set_main_loop_arg(std::bind_front(&App::Loop, this), nullptr, 0, true);
#else
        while(!glfwWindowShouldClose(window)) {
            Loop();
        }
#endif
    }
    App::~App() {
        Renderer::Shutdown();
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void App::Loop() {

        using namespace std::chrono;
        clock::time_point new_time = clock::now();
        delta_time = std::chrono::duration<float>(new_time - old_time).count();
        old_time = new_time;

        registry.ctx().get<CharacterController>().Update(registry.view<BoxCollider>(), delta_time, Renderer::init.queue, Renderer::view_buffer);
        Renderer::Render(registry);

        glfwPollEvents();

    }

}
namespace {
    GLFWwindow* SetupGLFW() {

        glfwSetErrorCallback([](int code, const char* message) {
                spdlog::error("GLFW error: {} - {}", code, message);
        });

        // Wayland is used by default but only X11 works on WSL
#ifdef FORCE_X11
        glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
#endif

        glfwInit();

        if(!glfwInit())
            throw std::runtime_error("Failed to initialise GLFW!");

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        GLFWwindow* glfw_window = glfwCreateWindow(1920, 1080, "ChemicalMC_PK", nullptr, nullptr);

        if(!glfw_window) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window!");
        }
#ifndef __EMSCRIPTEN__
    #ifdef WSL
        glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    #else
        glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    #endif
#endif
        // if(glfwRawMouseMotionSupported())
        //     glfwSetInputMode(glfw_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        return glfw_window;
    }

}
