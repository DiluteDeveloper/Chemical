#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "scene_objects/mesh_3d.h"

namespace Chemical {
	class ModelImporter {
	public:

		std::unique_ptr<Scene::Mesh3D> ImportModel(const std::string& file_path);
	private:

		std::unique_ptr<Scene::Mesh3D> ProcessMesh(aiMesh* mesh);
		Assimp::Importer importer;
	};
}
