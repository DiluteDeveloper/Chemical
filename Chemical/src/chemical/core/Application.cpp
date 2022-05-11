#include "Application.h"
#include "Window.h"
#include "Layer.h"
#include "AssetImporter.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

#include <glad/glad.h>

#include <vendor/IMGUI/imgui.h>
#include <vendor/IMGUI/imgui_impl_opengl3.h>
#include <vendor/IMGUI/imgui_impl_glfw.h>

namespace Chemical {

    namespace Core {

        Application::Application() {
            std::cout << "Application constructor called." << std::endl;

            window = std::make_unique<Window>(AccessKey<Application>());
            assetImporter = std::make_unique<AssetImporter>(AccessKey<Application>());
            time = std::make_unique<Time>(AccessKey<Application>(), GetWindow());
            layerStack = std::make_unique<LayerStack>(AccessKey<Application>());

            stbi_set_flip_vertically_on_load(true);

        }

        void Application::Run() {
            while (!window->Close()) {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();

                time->Update(AccessKey<Application>());
                layerStack->Update(AccessKey<Application>());

                ImGui::Render();
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

                window->SwapBuffers();

                window->PollEvents();
            }
        }
    }

}
