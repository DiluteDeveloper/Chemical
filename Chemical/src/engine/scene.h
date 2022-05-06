#pragma once

#include "renderable.h"
#include "camera.h"
#include "mesh.h"
#include "point_light.h"

#include <vector>
#include <memory>

#define MAX_POINT_LIGHTS 10

class Scene {
	std::vector<Renderable> renderables;

	PointLight pointLights[MAX_POINT_LIGHTS];
	std::unique_ptr<Mesh> defaultMesh;

	unsigned int pointLightsSize = 0;

public:

	Camera camera;

	Scene();

	Mesh getDefaultMesh() const { return *defaultMesh; }

	unsigned int getRenderablesSize() const;
	Renderable* createRenderable();
	Renderable* getRenderable(unsigned int index);
	void removeRenderableIndex(unsigned int index);

	unsigned int getPointLightsSize() const;
	PointLight* createPointLight();
	PointLight* getPointLight(unsigned int index);
	void removePointLightIndex(unsigned int index);

	Scene(const Scene& scene) = delete;
	Scene(Scene&& scene) = delete;
};

