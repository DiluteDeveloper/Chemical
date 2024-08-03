#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "rendering/generated_mesh_3d.h"

namespace Chemical {
	class ModelImporter {
	public:

		// returns a unique pointer as operation may fail
		std::unique_ptr<GeneratedMesh3D> ImportModel(const std::string& filePath);
	private:

		GeneratedMesh3D ProcessMesh(aiMesh* mesh);
		Assimp::Importer importer;
	};
}
