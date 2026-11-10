#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#ifdef EMSCRIPTEN
    #include <emscripten.h>
#endif

#include <spdlog/spdlog.h>
#include <glm/gtc/matrix_transform.hpp>
#include <random>
#include <memory>
#include <utility>

#include "graphics/gl.h"

#include <GLFW/glfw3.h>

#include "core.h"

using namespace Chemical;

namespace Chemical {

    Core::Core() {

        // offset of 5,5,5
        // minpos 5,0,5

        // std::vector<BlockFaceGroup> bfgs;
        // bfgs.emplace_back(glm::ivec3(5,0,5),glm::ivec3(10,5,10), 
        //         BlockType::Dirt, BlockFaceOrientation::POSITIVE_Z);
        // bfgs.emplace_back(glm::ivec3(5,0,5),glm::ivec3(10,5,10), 
        //         BlockType::Stone, BlockFaceOrientation::POSITIVE_Z);
        // bfgs.emplace_back(glm::ivec3(5,0,5),glm::ivec3(10,5,10), 
        //         BlockType::Stone, BlockFaceOrientation::POSITIVE_Z);
        // bfgs.emplace_back(glm::ivec3(5,0,5),glm::ivec3(10,5,10), 
        //         BlockType::Air, BlockFaceOrientation::POSITIVE_Z);
        // bfgs.emplace_back(glm::ivec3(5,0,5),glm::ivec3(10,5,10), 
        //         BlockType::Stone, BlockFaceOrientation::POSITIVE_Z);
        // bfgs.emplace_back(glm::ivec3(5,0,5),glm::ivec3(10,5,10), 
        //         BlockType::Dirt, BlockFaceOrientation::POSITIVE_Z);
        //
        //
        // std::vector<uint32_t> vaos;
        // Graphics::Renderer::ConvertBlockFaceGroupsToVAOs_Inplace(bfgs, vaos);

        // GLFW INITIALIZATION ----------------------------
        
        spdlog::set_level(spdlog::level::trace);

        spdlog::trace("Initialising GLFW");
        // glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);

        if (!glfwInit()) {
            spdlog::trace("GLFW initialization failed.");
            throw std::exception();
        }

        // GLFW INITIALIZATION ----------------------------

        // GLFW WINDOW HINTS ----------------------------------

        spdlog::trace("Setting GLFW window hints");

        const char* glsl_version = "#version 300 es";

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_MAJOR_VERSION);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_MINOR_VERSION);
#if defined(EMSCRIPTEN)
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(GLAD) 
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

        spdlog::trace("Creating GLFW window");

        window = glfwCreateWindow(1920, 1080, "Chemical", NULL, NULL);

        if (!window) {
            spdlog::info("GLFW window creation failed.");
            throw std::exception();
        }

        spdlog::trace("Setting GLFW input mode to cursor disabled");

        glfwSetMouseButtonCallback(window, [](GLFWwindow* w, int button, int action, int mods) {
            glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        });
        // if (glfwRawMouseMotionSupported())
        //     glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

        SetupImGui(glsl_version);

        spdlog::trace("Setting OpenGL context to GLFW window");

        glfwMakeContextCurrent(window);

#ifdef GLAD
        if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
            spdlog::critical("Failed to load GL loader into GLFW");
            throw std::exception();
        }
#endif

        glClearColor(1.0f, 0.7f, 0.9f, 1.0f);

        // glEnable(GL_DEBUG_OUTPUT);
        // glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glEnable(GL_DEPTH_TEST);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);

        projection = glm::perspectiveLH(glm::radians(90.0f), 1280.0f / 720.0f, 0.1f, 10000.0f);

        std::random_device rd;
        uint32_t seed = rd();

        glfwGetCursorPos(window, &oldx, &oldy);

        renderer = Graphics::Renderer::CreateRenderer();

        if(renderer == nullptr) {
            spdlog::critical("Failed to create renderer. Terminating...");
            return;
        }

        std::vector<BlockFaceGroup> bfgs;
        bfgs.emplace_back(glm::ivec3(5,0,5),glm::ivec3(10,5,10), 
                BlockType::Dirt, BlockFaceOrientation::POSITIVE_Z);
        bfgs.emplace_back(glm::ivec3(5,0,5),glm::ivec3(10,5,10), 
                BlockType::Stone, BlockFaceOrientation::POSITIVE_Z);
        bfgs.emplace_back(glm::ivec3(5,2,5),glm::ivec3(10,5,10), 
                BlockType::Stone, BlockFaceOrientation::POSITIVE_Z);
        bfgs.emplace_back(glm::ivec3(5,0,6),glm::ivec3(10,5,10), 
                BlockType::Air, BlockFaceOrientation::POSITIVE_Z);
        bfgs.emplace_back(glm::ivec3(5,0,5),glm::ivec3(10,5,10), 
                BlockType::Stone, BlockFaceOrientation::POSITIVE_Z);
        bfgs.emplace_back(glm::ivec3(-50,-50,-50),glm::ivec3(50,50,-50), 
                BlockType::Dirt, BlockFaceOrientation::POSITIVE_Z);

        renderer->UploadBlockFaceGroups(bfgs);

        renderer->UpdateProjectionMatrix(projection);

#ifdef EMSCRIPTEN
        emscripten_set_main_loop_arg(
            [](void* arg) {
                static_cast<Core*>(arg)->Update();
            },
            this,
            0,
            true
        );
#else
        while(!glfwWindowShouldClose(window)) {
            Update();
        }
#endif
    }

    void Core::SetupImGui(const std::string_view& glsl_version) {
#ifdef CHEMICAL_USE_IMGUI
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        ImGui::StyleColorsDark();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplGlfw_InstallEmscriptenCallbacks(window, "#canvas");
        ImGui_ImplOpenGL3_Init(glsl_version.data());

        io.IniFilename = nullptr;
#endif
    }
    void Core::UpdatePlayer() {

        glm::fmat4 matrix = player_transform.GetTransform();
        glm::fvec3 right = matrix[0];
        glm::fvec3 up = matrix[1];
        glm::fvec3 forward = matrix[2];
        glm::fvec3 front = glm::normalize(glm::fvec3(forward.x, 0, forward.z));

        if (glfwGetKey(window, GLFW_KEY_W)) {
            player_transform.position += front * camSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_S)) {
            player_transform.position -= front * camSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_A)) {
            player_transform.position -= right * camSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_D)) {
            player_transform.position += right * camSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE)) {
            player_transform.position.y += camSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
            player_transform.position.y -= camSpeed;
        }

        //std::cout << player_transform.position.x << ", " << player_transform.position.y << ", " << player_transform.position.z << std::endl;

        double x, y;
        glfwGetCursorPos(window, &x, &y);

        player_transform.rotation.y += static_cast<float>(x - oldx) * sensitivity;
        player_transform.rotation.x += static_cast<float>(y - oldy) * sensitivity;

        player_transform.rotation.x = glm::clamp(player_transform.rotation.x, -85.0f, 85.0f);

        oldx = x;
        oldy = y;
    }

    void Core::Update() {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifdef CHEMICAL_USE_IMGUI
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        { // ImGui Window
            // static float f = 0.0f;
            // static int counter = 0;

            ImGui::Begin("Chemical");                          // Create a window called "Hello, world!" and append into it.

            // ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            // ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            // ImGui::Checkbox("Another Window", &show_another_window);

            // ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            // ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            // if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            //     counter++;
            // ImGui::SameLine();
            // ImGui::Text("counter = %d", counter);

            glm::fmat4 matrix = player_transform.GetTransform();
            glm::fvec3 forward = matrix[2];
            ImGui::Text("Forward: %.2f, %.2f, %.2f", forward.x, forward.y, forward.z);
            ImGuiIO& io = ImGui::GetIO();
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }
#endif
        UpdatePlayer();

        renderer->UpdateViewMatrix(glm::inverse(player_transform.GetTransform()));
        renderer->Render();

        // loader->UpdateCenter(player_transform.position);
        // RenderChunkGrid(loader->GetChunkGrid(), *chunk_shader, glm::inverse(player_transform.GetTransform()));

#ifdef CHEMICAL_USE_IMGUI
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    Core::~Core() {

#ifdef CHEMICAL_USE_IMGUI

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

#endif

        glfwDestroyWindow(window);
        glfwTerminate();
    }
}

int main(int argc, char* argv[]) {
    Core core;

    return 0;
}
