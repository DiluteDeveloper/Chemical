#include "Application.h"
#include "Window.h"
#include "AssetImporter.h"
#include "Time.h"
#include "LayerStack.h"

#include "chemical/layers/SettingsLayer.h"
#include "chemical/layers/GUILayer.h"

#define STB_IMAGE_IMPLEMENTATION
#include <vendor/stb_image/stb_image.h>

#include <vendor/glad/glad.h>
#include <vendor/glfw/glfw3.h>

#include <vendor/IMGUI/imgui.h>
#include <vendor/IMGUI/imgui_impl_opengl3.h>
#include <vendor/IMGUI/imgui_impl_glfw.h>

namespace Chemical {

    namespace Core {

        Application::Application() {

            layerStack = new LayerStack();

            Layers::SettingsLayer& s = layerStack->PushLayer<Layers::SettingsLayer>();
            window = new Window(s.windowSettings);
            assetImporter = new AssetImporter();
            time = new Time(glfwGetTime());

            layerStack->PushLayer<Layers::GUILayer>(assetImporter->GetResourceStructure());

            stbi_set_flip_vertically_on_load(true);

        }

        void Application::Run() {
            while (!glfwWindowShouldClose(window->glfwWindow)) {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();

                time->Update(glfwGetTime());
                layerStack->Update();

                ImGui::Render();
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

                glfwSwapBuffers(window->glfwWindow);

                glfwPollEvents();
            }
        }

        Application::~Application() {

            delete layerStack;
            delete time;
            delete assetImporter;
            delete window;
        }
    }

}
