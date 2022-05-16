#pragma once

#include <iostream>
#include <unordered_map>
#include <filesystem>

#include "chemical/render/Mesh.h"
#include "chemical/render/Shader.h"
#include "chemical/render/Material.h"

#include "chemical/render/Scene.h"

#include "chemical/core/LayerStack.h"
#include "chemical/core/Layer.h"

namespace Chemical {

	namespace Core {

		class AssetImporterLayer : public Layer {

			friend class LayerStack;

			std::unordered_map<std::string, Render::Mesh> meshes;
			std::unordered_map<std::string, Render::Shader> shaders;
			std::unordered_map<std::string, Render::Material> materials;
			std::unordered_map<std::string, Render::Scene> scenes;

			std::unordered_map<std::string, std::vector<std::filesystem::path>> resourceStructure; // should be std::filesystem::path for both but bugged

			AssetImporterLayer(const AssetImporterLayer&) = delete;
			AssetImporterLayer(AssetImporterLayer&&) = delete;

			AssetImporterLayer();
			~AssetImporterLayer() override = default;

			Render::Scene LoadSceneFromFile(const std::filesystem::path& filePath);

			// return value ptr will be null if filePath mesh doesnt exist.
			Render::Mesh& GetMesh(const std::string& filePath);

			// return value ptr will be null if filePath material doesnt exist.
			Render::Material& GetMaterial(const std::string& filePath);

			// return value ptr will be null if filePath shader doesnt exist.
			Render::Shader& GetShader(const std::string& filePath);

		public:

			Render::Scene& GetScene(const std::string& filePath);
		};
	}


}

