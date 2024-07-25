#pragma once

#include "rendering/data_types/mesh.h"

namespace Chemical {


	// Will not be hashing any values or optimizing this class for the time being.
	class Scene {
	public:

		void AddMesh(std::shared_ptr<Mesh> mesh);
		void AddScene(const Scene& scene);

		void RemoveMesh(unsigned int index);
		void RemoveScene(unsigned int index);

		std::vector<std::shared_ptr<Mesh>> meshes;
		std::vector<Scene> scenes;
	};
}
