#pragma once

#include <vector>
#include <iostream>

#include "chemical/Mesh.h"
#include "chemical/Shader.h"
#include "chemical/Material.h"

#include "AccessKey.h"

namespace Chemical {

	namespace Core {
		class Application;

		class AssetImporter {

			std::vector<Mesh> meshes;
			std::vector<Shader> shaders;
			std::vector<Material> materials;

			std::vector<std::string> folders;

			AssetImporter(const AssetImporter&) = delete;
			AssetImporter(AssetImporter&&) = delete;

		public:
			AssetImporter(AccessKey<Application>);
			~AssetImporter() { std::cout << "AssetImporter destructor called." << std::endl; };

			Mesh* GetMesh(const std::string& filePath);
			Material* GetMaterial(const std::string& filePath);
			Shader* GetShader(const std::string& filePath);
		};
	}


}

