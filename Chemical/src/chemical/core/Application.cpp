#include "Application.h"
#include "LayerStack.h"

#define STB_IMAGE_IMPLEMENTATION
#include <vendor/stb_image/stb_image.h>

#include <vendor/glad/glad.h>
#include <vendor/glfw/glfw3.h>

#include <vendor/IMGUI/imgui.h>
#include <vendor/IMGUI/imgui_impl_opengl3.h>
#include <vendor/IMGUI/imgui_impl_glfw.h>

namespace Chemical {

    namespace Core {

        Application::Application() { // Should have an interface for glad in the future; glad funcs wont work without a window

            layerStack = new LayerStack();

            SettingsLayer& s = layerStack->PushLayer<SettingsLayer>();
            w = &layerStack->PushLayer<WindowLayer>(s.windowSettings);

            AssetImporterLayer& a = layerStack->PushLayer<AssetImporterLayer>();

            layerStack->PushLayer<Render::RendererLayer>(a);

            t = &layerStack->PushLayer<Util::TimeLayer>(*w);


            stbi_set_flip_vertically_on_load(true);
        }

        void Application::Run() {
            while (!w->ShouldClose()) {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();

                layerStack->Update();
               

                ImGui::Render();

                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

                w->SwapBuffers();

                w->PollEvents();
            }
        }

        LayerStack& Application::GetLayerStack() {
            return *layerStack;
        }

        Util::TimeLayer& Application::GetTime() {
            return *t;
        }
    }

}
