#pragma once

#include <vector>
#include <iostream>
#include <unordered_map>
#include <filesystem>

#include "chemical/Mesh.h"
#include "chemical/Shader.h"
#include "chemical/Material.h"

namespace Chemical {

	namespace Core {

		class AssetImporter {
			friend class Application;

			std::vector<Mesh> meshes;
			std::vector<Shader> shaders;
			std::vector<Material> materials;

			std::unordered_map<std::string, std::vector<std::filesystem::path>> resourceStructure; // should be std::filesystem::path for both but bugged

			AssetImporter(const AssetImporter&) = delete;
			AssetImporter(AssetImporter&&) = delete;

			AssetImporter();
			~AssetImporter() = default;
		public:
			const std::vector<Mesh>& GetMeshes() const;
			const std::vector<Material>& GetMaterials() const;
			const std::vector<Shader>& GetShaders() const;
			const std::unordered_map<std::string, std::vector<std::filesystem::path>>& GetResourceStructure() const;

			// return value ptr will be null if filePath mesh doesnt exist.
			/*std::tuple<const Mesh*, bool> GetMesh(const std::string& filePath) const;

			// return value ptr will be null if filePath material doesnt exist.
			std::tuple<const Material*, bool> GetMaterial(const std::string& filePath) const;

			// return value ptr will be null if filePath shader doesnt exist.
			std::tuple<const Shader*, bool> GetShader(const std::string& filePath) const;*/
		};
	}


}

