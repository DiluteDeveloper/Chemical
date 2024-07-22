#pragma once

#include "data_types/mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

namespace Chemical {
	class ModelImporter {
	public:

		// imports single meshes for now
		std::optional<Mesh> ImportModel(const std::string& file_path);
	private:

		Mesh ProcessMesh(aiMesh* mesh);
		Assimp::Importer importer;
	};
}
