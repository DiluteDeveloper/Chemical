#pragma once

#include "data_types/mesh.h"
#include <assimp/Importer.hpp>

namespace Chemical {
	class ModelImporter {
	public:

		// imports single meshes for now
		static void ImportModel(const std::string& file_path);

		// Updates after each ImportModel call to reflect the status
		static bool import_success;
		static Mesh imported_mesh;
	private:

		static Assimp::Importer importer;
	};
}
