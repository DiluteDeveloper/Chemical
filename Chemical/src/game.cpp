#include "game.h"
#include "engine/window.h"
#include "engine/events.h"

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
    proj = new glm::mat4(glm::perspective(glm::radians(90.0f), (float)x / (float)y, 0.1f, 100.0f));
}


Game::Game() :
textureShader("res/shaders/textureShader.v", "res/shaders/textureShader.f"),
colourShader("res/shaders/colourShader.v", "res/shaders/colourShader.f"),
camera(5.0f, 0.1f)
{
		std::vector<float> vertices{
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, -1.0f, // BACK
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f, 0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, -1.0f,

        
        -0.5f, -0.5f, 0.5f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f, // FRONT
         0.5f, -0.5f, 0.5f,   1.0f, 0.0f,   0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, 0.5f,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, 0.5f,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, 0.5f,   0.0f, 1.0f,   0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  -1.0f, 0.0f, 0.0f, // LEFT
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,

         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f, // RIGHT
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
         

        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f, -1.0f, 0.0f, // BOTTOM
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f,  0.0f, -1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f,  0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f, -1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f, // TOP
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f
	};

	std::vector<unsigned int> indices{
		0,1,2,
		3,4,5,
        6,7,8,
        9,10,11,
        12,13,14,
        15,16,17,
        18,19,20,
        21,22,23,
        24,25,26,
        27,28,29,
        30,31,32,
        33,34,35
	};

    Window::setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    KeyPressDispatcher::subscribe([this](int a, int b, int c, int d) { keyPress(a, b, c, d); });
    WindowResizeDispatcher::subscribe([this](int a, int b){ windowResize(a, b); });

    glm::vec2 windowSize = Window::getSize();
    proj = new glm::mat4(glm::perspective(glm::radians(90.0f), (float)windowSize.x / (float)windowSize.y, 0.1f, 100.0f));

    light.scale = glm::vec3(0.2f);
    light.position = glm::vec3(1.2f, 1.0f, 2.0f);

    cube = new Mesh(vertices, indices);
    floor.transform.scale = glm::vec3(5);
    floor.transform.position.y = -3;
}

void Game::updateGUI() {

    // --------------------------------------------------------------

    ImGui::Begin("Editor");
    ImGui::PushItemWidth(200.0f);
    if (ImGui::BeginCombo("##box", "Box")) {
        if (ImGui::BeginCombo("##box_transform", "Transform")) {
            ImGui::DragFloat3("Position", &box.transform.position[0], .02f, -9999.0f, 9999.0f, "%.2f");
            ImGui::DragFloat3("Rotation", &box.transform.rotation[0], .5f, -9999.0f, 9999.0f, "%.2f");
            ImGui::DragFloat3("Scale", &box.transform.scale[0], .02f, -9999.0f, 9999.0f, "%.2f");
            ImGui::EndCombo();
        }
        if (ImGui::BeginCombo("##box_material", "Material")) {
            ImGui::DragFloat3("Diffuse", &box.material.diffuse[0], .02f, 0.0f, 1.0f, "%.2f");
            ImGui::DragFloat3("Specular", &box.material.specular[0], .02f, 0.0f, 1.0f, "%.2f");
            ImGui::DragFloat3("Ambient", &box.material.ambient[0], .02f, 0.0f, 1.0f, "%.2f");
            ImGui::DragFloat("Shininess", &box.material.shininess, .02f, 0.0f, 256.0f, "%.1f");
            ImGui::EndCombo();
        }

        ImGui::EndCombo();
    }
    if (ImGui::BeginCombo("##floor", "Floor")) {
        if (ImGui::BeginCombo("##floor_transform", "Transform")) {
            ImGui::DragFloat3("Position", &floor.transform.position[0], .02f, -9999.0f, 9999.0f, "%.2f");
            ImGui::DragFloat3("Rotation", &floor.transform.rotation[0], .5f, -9999.0f, 9999.0f, "%.2f");
            ImGui::DragFloat3("Scale", &floor.transform.scale[0], .02f, -9999.0f, 9999.0f, "%.2f");
            ImGui::EndCombo();
        }
        if (ImGui::BeginCombo("##floor_material", "Material")) {
            ImGui::DragFloat3("Diffuse", &floor.material.diffuse[0], .02f, 0.0f, 1.0f, "%.2f");
            ImGui::DragFloat3("Specular", &floor.material.specular[0], .02f, 0.0f, 1.0f, "%.2f");
            ImGui::DragFloat3("Ambient", &floor.material.ambient[0], .02f, 0.0f, 1.0f, "%.2f");
            ImGui::DragFloat("Shininess", &floor.material.shininess, .02f, 0.0f, 256.0f, "%.1f");
            ImGui::EndCombo();
        }

        ImGui::EndCombo();
    }

    if (ImGui::BeginCombo("##light", "Light")) {

        if (ImGui::BeginCombo("##light_transform", "Transform")) {
            ImGui::DragFloat3("Position", &light.position[0], .02f, -9999.0f, 9999.0f, "%.2f");
            ImGui::DragFloat3("Rotation", &light.rotation[0], .5f, -9999.0f, 9999.0f, "%.2f");
            ImGui::DragFloat3("Scale", &light.scale[0], .02f, -9999.0f, 9999.0f, "%.2f");
            ImGui::EndCombo();
        }
        ImGui::DragFloat3("Colour", &lightColour[0], .02f, 0.0f, 1.0f, "%.2f");
        ImGui::EndCombo();
    }

    ImGui::PopItemWidth();
    ImGui::End();

    // --------------------------------------------------------------
}

void Game::update() {

    if(!overlaySelected)
        camera.update();

    updateGUI();

    textureShader.bind();

    textureShader.setMatrix4fv("v_proj", *proj);
    textureShader.setVector3("f_lightCol", lightColour);
    textureShader.setMatrix4fv("v_view", glm::inverse(camera.transform.toMat4()));
    textureShader.setVector3("f_viewPos", camera.transform.position);
    textureShader.setVector3("f_lightPos", light.position);

    textureShader.setMatrix4fv("v_model", box.transform.toMat4());

    textureShader.setMaterial(box.material);

    cube->draw();

    textureShader.setMatrix4fv("v_model", floor.transform.toMat4());

    textureShader.setMaterial(floor.material);

    cube->draw();

    colourShader.bind();
    colourShader.setMatrix4fv("v_proj", *proj);
    colourShader.setVector3("f_colour", lightColour);
    colourShader.setMatrix4fv("v_view", glm::inverse(camera.transform.toMat4()));
    colourShader.setMatrix4fv("v_model", light.toMat4());

    cube->draw();
}

Game::~Game() {
    delete cube;
    delete proj;
}