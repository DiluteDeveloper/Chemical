#include "chemical/core.h"

#include "chemical/resource_manager.h"
#include "chemical/scene.h"
#include "chemical/window.h"

#include <fstream>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>
#include <stdexcept>
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/sinks/stdout_color_sinks-inl.h>
#include <spdlog/spdlog.h>
#include <stb_image/stb_image.h>

namespace Chemical {

  using namespace std::placeholders;

  void Core::ConfigureSpdlog() const {
    spdlog::set_pattern("%^[%s] [%!] [%#] %$%v");
  }
  Core::Core(const std::string_view& window_title, const glm::vec2& window_size) {
    std::cout << "================================= " << window_title
              << " ====================================================" << std::endl;
    ConfigureSpdlog();

    SPDLOG_INFO("Initialising GLFW");
    if (!glfwInit()) {
      SPDLOG_CRITICAL("Failed to initialise Chemical : GLFW initialisation failed");
      throw std::runtime_error("GLFW initialisation failed");
    }
    SPDLOG_INFO("Configuring GLFW window hints");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    std::optional<Window> opt_window = CreateNewWindow(window_title, window_size.x, window_size.y);

    if (!opt_window.has_value()) {
      SPDLOG_CRITICAL("Failed to initialise Chemical : window creation failed");
      throw std::runtime_error("Window creation failed");
    }

    window = opt_window.value();

    SPDLOG_INFO("Initialising Input");
    input = std::make_unique<Input>(window);

    SPDLOG_INFO("Initialising Renderer");
    renderer = std::make_unique<GL_Renderer>();

    ShaderTraits traits;
    std::ifstream file("/mnt/storage/Chemical/Chemical/modules/chemical/res/shaders/default_shader.vs");

    if (!file) {
      SPDLOG_ERROR(R"(Failed to read file "" : returning 0)");
    }

    traits.vs_source = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    file.close();

    std::ifstream file2("/mnt/storage/Chemical/Chemical/modules/chemical/res/shaders/default_shader.fs");

    if (!file2) {
      SPDLOG_ERROR(R"(Failed to read file "" : returning 0)");
    }

    traits.fs_source = std::string((std::istreambuf_iterator<char>(file2)), std::istreambuf_iterator<char>());

    file2.close();
    renderer->RegisterShader("default", traits);
    //
    // std::optional<Util::Image> img =
    //     Util::LoadImage("/mnt/storage/Chemical/Chemical/modules/chemical/res/textures/arrow.png", 3);
    //
    // MaterialTraits m_traits(img.value());
    // m_traits.tint = glm::ivec3(255, 10, 10);
    // renderer->RegisterMaterial("default", m_traits);

    SPDLOG_INFO("Initialising Resource Manager");
    ResourceManager::SetResourceMasterPath("/mnt/storage/Chemical/Chemical/modules/chemical/res/");

    // resource_manager->LoadMaterialsJSONFile(
    //     std::bind(&GL_Renderer::RegisterMaterial, renderer.get(), _1, _2));

    SPDLOG_INFO("Initialising Scene");
    active_scene = std::make_unique<Scene>();
    ResourceManager::LoadAndRegisterScene("scenes/scene.json", *active_scene);
    ResourceManager::LoadAndRegisterProject(std::string(window_title.data(), ".json"), *renderer);

    // active_scene->RegisterTransform("default", Transform());
    // Sprite sprite;
    // sprite.transform_id = "default";
    // sprite.material_id = "default";
    //
    // active_scene->RegisterSprite("default", sprite);

    // std::optional<Scene> scene =
    //     LoadSceneFromJSONFile("/mnt/storage/Chemical/Chemical/modules/chemical/res/json/scene.json");
    //
    // if (scene) {
    //   active_scene = std::make_unique<Scene>(std::move(scene.value()));
    // }

    SPDLOG_INFO("Configuring stbi_image");
    stbi_set_flip_vertically_on_load(true);
  }
  Core::~Core() {
    SPDLOG_INFO("Deleting Scene");
    delete active_scene.release();
    SPDLOG_INFO("Deleting Renderer");
    delete renderer.release();
    SPDLOG_INFO("Destroying window");
    DestroyWindow(window);
    SPDLOG_INFO("Terminating GLFW");
    glfwTerminate();
  }

  void Core::StartGameLoop() {

    while (!WindowShouldClose(window)) {

      glClear(GL_COLOR_BUFFER_BIT);
      renderer->RenderScene(*active_scene);

      glfwSwapBuffers(window);

      glfwPollEvents();
    }
  }

  void Core::SetBackgroundColour(const glm::vec3& colour) {
    SPDLOG_INFO("Setting OpenGL clear colour to [{}, {}, {}]", colour.r, colour.g, colour.b);
    glClearColor(colour.r / 255.0f, colour.g / 255.0f, colour.b / 255.0f, 1.0f);
  }

} // namespace Chemical
