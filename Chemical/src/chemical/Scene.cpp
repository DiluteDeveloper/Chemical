#include "Scene.h"

#include <iostream>

namespace Chemical {
    void DisablePointLight(PointLight* pl) {
        pl->ambient = glm::vec3(0.0f);
        pl->diffuse = glm::vec3(0.0f);
        pl->specular = glm::vec3(0.0f);
        pl->transform = Transform{};
    }

    void EnablePointLight(PointLight* pl) {
        pl->ambient = glm::vec3(0.5f);
        pl->diffuse = glm::vec3(0.5f);
        pl->specular = glm::vec3(0.5f);
        pl->transform = Transform{ glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.2f) };
    }

    Renderable* Scene::CreateRenderable() {
        return &renderables.emplace_back();
    }
    Renderable* Scene::GetRenderable(unsigned int index) {
        return &renderables[index];
    }
    void Scene::RemoveRenderableIndex(unsigned int index) {
        renderables.erase(renderables.begin() + index);
    }

    PointLight* Scene::CreatePointLight() {
        if (pointLightsSize >= MAX_POINT_LIGHTS) {
            std::cout << "Max point lights reached in createPointLight()" << std::endl;
            return nullptr;
        }
        EnablePointLight(&pointLights[pointLightsSize]);
        pointLightsSize++;
        return &pointLights[pointLightsSize - 1];
    }
    PointLight* Scene::GetPointLight(unsigned int index) {
        return &pointLights[index];
    }

    void Scene::RemovePointLightIndex(unsigned int index) {
        if (index > pointLightsSize - 1) {
            std::cout << "removePointLightIndex too large." << std::endl;
            return;
        }

        DisablePointLight(&pointLights[index]);

        for (size_t i = index + 1; i < pointLightsSize; i++)
        {
            pointLights[i - 1] = pointLights[i];
        }
        pointLightsSize--;
    }
}
