#pragma once

#include "scene_object.h"

namespace Chemical {

	/*template<typename T>
	void Update() {
		T.object.Update();
	}*/
	// pseudocode above for composition idea

	// Will not be hashing any values or optimizing this class for the time being.
	class Scene {
	public:

		//void AddMesh(std::shared_ptr<Mesh> mesh);
		void AddScene(const Scene& scene);

		void RemoveMesh(unsigned int index);
		void RemoveScene(unsigned int index);

		std::vector<SceneObject> objects;
	};
}
