#pragma once

#include "Renderable.h"
#include "Camera.h"
#include "Mesh.h"
#include "PointLight.h"

#include <vector>
#include <memory>

namespace Chemical {
#define MAX_POINT_LIGHTS 10
	class Scene {
		friend class Renderer;

		std::vector<Renderable> renderables;

		PointLight pointLights[MAX_POINT_LIGHTS];
		unsigned int pointLightsSize = 0;

	public:
		Camera camera;

		Renderable* CreateRenderable();
		Renderable* GetRenderable(unsigned int index);
		void RemoveRenderableIndex(unsigned int index);

		PointLight* CreatePointLight();
		PointLight* GetPointLight(unsigned int index);
		void RemovePointLightIndex(unsigned int index);

		Scene() = default;
		Scene(const Scene&) = delete;
		Scene(Scene&&) = delete;
	};

}

