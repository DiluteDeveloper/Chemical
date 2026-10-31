#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
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

    void Core::Frame() {

        /*if (glfwGetKey(window, GLFW_KEY_T))
            loader->RemoveBlock(glm::ivec3(floor(player_transform.position.x), floor(player_transform.position.y), floor(player_transform.position.z)));
        if (glfwGetKey(window, GLFW_KEY_Y)) {
            loader->PlaceBlock(glm::ivec3(floor(player_transform.position.x), floor(player_transform.position.y), floor(player_transform.position.z)));
        }

        if (glfwGetKey(window, GLFW_KEY_6))
            loader->doChunkLoading = false;
        if (glfwGetKey(window, GLFW_KEY_7))
            loader->doChunkLoading = true;*/

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        UpdatePlayer();

        loader->UpdateCenter(player_transform.position);
        RenderChunkGrid(loader->GetChunkGrid(), *chunk_shader, glm::inverse(player_transform.GetTransform()));

        glfwSwapBuffers(window);

        glfwPollEvents();
    }
}

int main(int argc, char* argv[]) {

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

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);


    // GLFW WINDOW HINTS ----------------------------------

    // GLFW WINDOW SETUP -------------------------------------------------
    //
    spdlog::trace("Initialising Core");

    // Heap allocated and never deleted on purpose: the main loop keeps using it after main()'s
    // setup is done, so it must not live on main()'s stack.
    Core* core = new Core();

    spdlog::trace("Creating GLFW window");

    core->window = glfwCreateWindow(1920, 1080, "Chemical", NULL, NULL);

    if (!core->window) {
        spdlog::info("GLFW window creation failed.");
        throw std::exception();
    }

    spdlog::trace("Setting GLFW input mode to cursor disabled");

    glfwSetMouseButtonCallback(core->window, [](GLFWwindow* w, int button, int action, int mods) {
        if (action == GLFW_PRESS)
            glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    });

    // glfwSwapInterval(1);

    // GLFW PREFERENCES ----------------------------------

    // GLAD SETUP --------------------------------------------

    spdlog::trace("Setting OpenGL context to GLFW window");

    glfwMakeContextCurrent(core->window);

    glClearColor(1.0f, 0.2f, 0.3f, 1.0f);

    // GLAD SETUP --------------------------------------------

    // GLAD PREFERENCES -----------------------------------------

    // glEnable(GL_DEBUG_OUTPUT);
    // glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glEnable(GL_DEPTH_TEST);

    // glDebugMessageCallback(&message_callback, nullptr);
    // glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    // glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);

    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);


    // GLAD PREFERENCES -----------------------------------------

    // MORE TESTING CODE --------------------------------------------


    core->projection = glm::perspectiveLH(glm::radians(90.0f), 1280.0f / 720.0f, 0.1f, 10000.0f);

    std::random_device rd;
    uint32_t seed = rd();

    glfwGetCursorPos(core->window, &core->oldx, &core->oldy);

    // Each texture is moved into core->textures after it is set up. The GL texture name moves with it
    // (the moved-from object is left with id 0 and deletes nothing), so the unit bindings stay valid.
    core->textures.reserve(7);

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

    core->textures.push_back(std::move(dirt));


    void* grassdata = stbi_load("res/textures/blocks/grass.png", &width, &height, &channels, 0);

    OpenGL::Texture grass(p);

    grass.SetTextureData(param, grassdata);

    stbi_image_free(grassdata);

    grass.GenerateMipmaps();

    grass.BindTexture(2);

    grass.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MIN_FILTER, GL_NEAREST);
    grass.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MAG_FILTER, GL_NEAREST);

    core->textures.push_back(std::move(grass));

    void* stonedata = stbi_load("res/textures/blocks/stone.png", &width, &height, &channels, 0);

    OpenGL::Texture stone(p);

    stone.SetTextureData(param, stonedata);

    stbi_image_free(stonedata);

    stone.GenerateMipmaps();

    stone.BindTexture(0);

    stone.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MIN_FILTER, GL_NEAREST);
    stone.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MAG_FILTER, GL_NEAREST);

    core->textures.push_back(std::move(stone));

    void* beddata = stbi_load("res/textures/blocks/bedrock.png", &width, &height, &channels, 0);

    OpenGL::Texture bed(p);

    bed.SetTextureData(param, beddata);

    stbi_image_free(beddata);

    bed.GenerateMipmaps();

    bed.BindTexture(3);

    bed.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MIN_FILTER, GL_NEAREST);
    bed.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MAG_FILTER, GL_NEAREST);

    core->textures.push_back(std::move(bed));

    void* logtopdata = stbi_load("res/textures/blocks/oaklogtop.png", &width, &height, &channels, 0);

    OpenGL::Texture logtop(p);

    logtop.SetTextureData(param, logtopdata);

    stbi_image_free(logtopdata);

    logtop.GenerateMipmaps();

    logtop.BindTexture(4);

    logtop.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MIN_FILTER, GL_NEAREST);
    logtop.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MAG_FILTER, GL_NEAREST);

    core->textures.push_back(std::move(logtop));

    void* logsidedata = stbi_load("res/textures/blocks/oaklogside.png", &width, &height, &channels, 0);

    OpenGL::Texture logside(p);

    logside.SetTextureData(param, logsidedata);

    stbi_image_free(logsidedata);

    logside.GenerateMipmaps();

    logside.BindTexture(5);

    logside.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MIN_FILTER, GL_NEAREST);
    logside.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MAG_FILTER, GL_NEAREST);

    core->textures.push_back(std::move(logside));

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

    core->textures.push_back(std::move(leaves));

    core->loader = std::make_unique<ChunkLoader>(glm::vec3{}, 300, 8);

    // The shader objects can stay local: once the program is linked it doesn't need them any more.
    OpenGL::Shader vertex_chunk_shader(Util::ReadFile("res/shaders/chunk_shader.vert").c_str(), OpenGL::ShaderType::VERTEX_SHADER);
    OpenGL::Shader fragment_chunk_shader(Util::ReadFile("res/shaders/chunk_shader.frag").c_str(), OpenGL::ShaderType::FRAGMENT_SHADER);

    OpenGL::VertexLayout vertex_layout;
    vertex_layout.AddAttribute(OpenGL::VertexAttribute{ 1, 0, OpenGL::DataType::UNSIGNED_INT, OpenGL::DataTransformation::INT });

    core->chunk_shader = std::make_unique<OpenGL::ShaderProgram>(std::initializer_list<
        const OpenGL::Shader*>{ &vertex_chunk_shader, & fragment_chunk_shader }, vertex_layout);

    core->chunk_shader->SetUniformMatrix4FV("v_proj", 1, false, &core->projection[0][0]);

    emscripten_set_main_loop_arg(
        [](void* arg) {
            static_cast<Core*>(arg)->Frame();
        },
        core,
        0,
        true
    );
    return 0;
}
