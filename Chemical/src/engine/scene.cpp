#include "scene.h"

#include <iostream>

Scene::Scene() {
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

    defaultMesh = std::make_unique<Mesh>(vertices, indices);
}

void disablePointLight(PointLight* pl) {
    pl->ambient = glm::vec3(0.0f);
    pl->diffuse = glm::vec3(0.0f);
    pl->specular = glm::vec3(0.0f);
    pl->transform = Transform{};
}

void enablePointLight(PointLight* pl) {
    pl->ambient = glm::vec3(0.5f);
    pl->diffuse = glm::vec3(0.5f);
    pl->specular = glm::vec3(0.5f);
    pl->transform = Transform{glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.2f)};
}

unsigned int Scene::getRenderablesSize() const {
    return renderables.size();
}

Renderable* Scene::createRenderable() {
    return &renderables.emplace_back(*defaultMesh);
}
Renderable* Scene::getRenderable(unsigned int index) {
    return &renderables[index];
}
void Scene::removeRenderableIndex(unsigned int index) {
    renderables.erase(renderables.begin() + index);
    renderables.shrink_to_fit();
}

unsigned int Scene::getPointLightsSize() const {
    return pointLightsSize;
}

PointLight* Scene::createPointLight() {
    if (pointLightsSize >= MAX_POINT_LIGHTS) {
        std::cout << "Max point lights reached in createPointLight()" << std::endl;
        return nullptr;
    }
    enablePointLight(&pointLights[pointLightsSize]);
    pointLightsSize++;
    return &pointLights[pointLightsSize - 1];
}
PointLight* Scene::getPointLight(unsigned int index) {
    return &pointLights[index];
}

void Scene::removePointLightIndex(unsigned int index) {
    if (index > pointLightsSize - 1) {
        std::cout << "removePointLightIndex too large." << std::endl;
        return;
    }

    disablePointLight(&pointLights[index]);

    for (size_t i = index + 1; i < pointLightsSize; i++)
    {
        pointLights[i - 1] = pointLights[i];
    }
    pointLightsSize--;
}