#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <emscripten.h>

#include <spdlog/spdlog.h>
#include <glm/gtc/matrix_transform.hpp>
#include <random>
#include <memory>
#include <utility>

#include "graphics/opengl/texture.h"
#include "graphics/opengl/shader_program.h"
#include "util/filestream.h"
#include "world/chunk/loader.h"
#include "world/chunk/renderer.h"

#include <stb_image.h>

#include "core.h"

using namespace Chemical;

namespace Chemical {

    Core::Core() {
        // GLFW INITIALIZATION ----------------------------
        
        spdlog::set_level(spdlog::level::trace);

        spdlog::trace("Initialising GLFW");

        if (!glfwInit()) {
            spdlog::trace("GLFW initialization failed.");
            throw std::exception();
        }

        // GLFW INITIALIZATION ----------------------------

        // GLFW WINDOW HINTS ----------------------------------

        spdlog::trace("Setting GLFW window hints");

        const char* glsl_version = "#version 300 es";
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

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

        SetupImGui(glsl_version);

        spdlog::trace("Setting OpenGL context to GLFW window");

        glfwMakeContextCurrent(window);

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

        // Each texture is moved into core->textures after it is set up. The GL texture name moves with it
        // (the moved-from object is left with id 0 and deletes nothing), so the unit bindings stay valid.
        textures.reserve(7);

        int width=0, height=0, channels=0;
        void* dirtdata = stbi_load("res/textures/blocks/dirt.png", &width, &height, &channels, 0);

        OpenGL::TextureStorageParameters p;
        p.internalFormat = OpenGL::TextureInternalFormat::RGB8;
        p.width = width;
        p.height = height;
        OpenGL::Texture dirt(p);

        OpenGL::TextureDataParameters param;
        param.baseFormat = OpenGL::TextureBaseFormat::RGB;
        param.dataType = OpenGL::DataType::UNSIGNED_BYTE;
        param.width = width;
        param.height = height;
        // param.textureType = OpenGL::TextureType::TEXTURE_2D;
        dirt.SetTextureData(param, dirtdata);

        stbi_image_free(dirtdata);

        dirt.GenerateMipmaps();

        dirt.BindTexture(1);

        dirt.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MIN_FILTER, GL_NEAREST);
        dirt.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MAG_FILTER, GL_NEAREST);

        textures.push_back(std::move(dirt));


        void* grassdata = stbi_load("res/textures/blocks/grass.png", &width, &height, &channels, 0);

        OpenGL::Texture grass(p);

        grass.SetTextureData(param, grassdata);

        stbi_image_free(grassdata);

        grass.GenerateMipmaps();

        grass.BindTexture(2);

        grass.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MIN_FILTER, GL_NEAREST);
        grass.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MAG_FILTER, GL_NEAREST);

        textures.push_back(std::move(grass));

        void* stonedata = stbi_load("res/textures/blocks/stone.png", &width, &height, &channels, 0);

        OpenGL::Texture stone(p);

        stone.SetTextureData(param, stonedata);

        stbi_image_free(stonedata);

        stone.GenerateMipmaps();

        stone.BindTexture(0);

        stone.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MIN_FILTER, GL_NEAREST);
        stone.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MAG_FILTER, GL_NEAREST);

        textures.push_back(std::move(stone));

        void* beddata = stbi_load("res/textures/blocks/bedrock.png", &width, &height, &channels, 0);

        OpenGL::Texture bed(p);

        bed.SetTextureData(param, beddata);

        stbi_image_free(beddata);

        bed.GenerateMipmaps();

        bed.BindTexture(3);

        bed.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MIN_FILTER, GL_NEAREST);
        bed.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MAG_FILTER, GL_NEAREST);

        textures.push_back(std::move(bed));

        void* logtopdata = stbi_load("res/textures/blocks/oaklogtop.png", &width, &height, &channels, 0);

        OpenGL::Texture logtop(p);

        logtop.SetTextureData(param, logtopdata);

        stbi_image_free(logtopdata);

        logtop.GenerateMipmaps();

        logtop.BindTexture(4);

        logtop.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MIN_FILTER, GL_NEAREST);
        logtop.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MAG_FILTER, GL_NEAREST);

        textures.push_back(std::move(logtop));

        void* logsidedata = stbi_load("res/textures/blocks/oaklogside.png", &width, &height, &channels, 0);

        OpenGL::Texture logside(p);

        logside.SetTextureData(param, logsidedata);

        stbi_image_free(logsidedata);

        logside.GenerateMipmaps();

        logside.BindTexture(5);

        logside.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MIN_FILTER, GL_NEAREST);
        logside.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MAG_FILTER, GL_NEAREST);

        textures.push_back(std::move(logside));

        OpenGL::TextureStorageParameters pleaves;
        pleaves.internalFormat = OpenGL::TextureInternalFormat::RGBA8;
        pleaves.width = width;
        pleaves.height = height;

        OpenGL::TextureDataParameters paramleaves;
        paramleaves.baseFormat = OpenGL::TextureBaseFormat::RGBA;
        paramleaves.dataType = OpenGL::DataType::UNSIGNED_BYTE;
        paramleaves.width = width;
        paramleaves.height = height;
        // paramleaves.textureType = OpenGL::TextureType::TEXTURE_2D;

        void* leavesdata = stbi_load("res/textures/blocks/leaves.png", &width, &height, &channels, 0);

        OpenGL::Texture leaves(pleaves);

        leaves.SetTextureData(paramleaves, leavesdata);

        stbi_image_free(leavesdata);

        leaves.GenerateMipmaps();

        leaves.BindTexture(6);

        leaves.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MIN_FILTER, GL_NEAREST);
        leaves.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MAG_FILTER, GL_NEAREST);

        textures.push_back(std::move(leaves));

        textures[2].BindTexture(0);
        textures[0].BindTexture(1);
        textures[1].BindTexture(2);
        textures[3].BindTexture(3);

        loader = std::make_unique<ChunkLoader>(glm::vec3{}, 300, 12);

        // The shader objects can stay local: once the program is linked it doesn't need them any more.
        OpenGL::Shader vertex_chunk_shader(Util::ReadFile("res/shaders/chunk_shader.vert").c_str(), OpenGL::ShaderType::VERTEX_SHADER);
        OpenGL::Shader fragment_chunk_shader(Util::ReadFile("res/shaders/chunk_shader.frag").c_str(), OpenGL::ShaderType::FRAGMENT_SHADER);


        OpenGL::VertexLayout vertex_layout;
        vertex_layout.AddAttribute(OpenGL::VertexAttribute{ 1, 0, OpenGL::DataType::UNSIGNED_INT, OpenGL::DataTransformation::INT });

        chunk_shader = std::make_unique<OpenGL::ShaderProgram>(std::initializer_list<
            const OpenGL::Shader*>{ &vertex_chunk_shader, & fragment_chunk_shader }, vertex_layout);

        chunk_shader->SetUniformMatrix4FV("v_proj", 1, false, &projection[0][0]);

        chunk_shader->SetUniform1I("stoneTexture", 0);
        chunk_shader->SetUniform1I("dirtTexture", 1);
        chunk_shader->SetUniform1I("grassTexture", 2);
        chunk_shader->SetUniform1I("bedrockTexture", 3);
        chunk_shader->SetUniform1I("oaklogtopTexture", 4);
        chunk_shader->SetUniform1I("oaklogsideTexture", 5);
        chunk_shader->SetUniform1I("oakleavesTexture", 6);

        emscripten_set_main_loop_arg(
            [](void* arg) {
                static_cast<Core*>(arg)->Update();
            },
            this,
            0,
            true
        );
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

        loader->UpdateCenter(player_transform.position);
        RenderChunkGrid(loader->GetChunkGrid(), *chunk_shader, glm::inverse(player_transform.GetTransform()));

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
