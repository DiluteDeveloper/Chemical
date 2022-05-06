#include "game.h"
#include "engine/window.h"
#include "engine/events.h"
#include "engine/mesh.h"

#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <vendor/IMGUI/imgui.h>
#include <iostream>

void Game::keyPress(int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_G && action == GLFW_PRESS) {
        overlaySelected = !overlaySelected;

        if (overlaySelected)
            Window::setInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else
            Window::setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

}

void Game::windowResize(int x, int y) {
    scene.camera.proj = glm::mat4(glm::perspective(glm::radians(90.0f), (float)x / (float)y, 0.1f, 100.0f));
}


Game::Game() {


    Window::setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    KeyPressDispatcher::subscribe([this](int a, int b, int c, int d) { keyPress(a, b, c, d); });
    WindowResizeDispatcher::subscribe([this](int a, int b){ windowResize(a, b); });

    renderer.bindScene(scene);
}

void Game::update() {

    if(!overlaySelected)
        scene.camera.update();

    renderer.render();

    ImGui::Begin("Performance Stats", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize 
        | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
    renderer.renderSceneGUI();
}