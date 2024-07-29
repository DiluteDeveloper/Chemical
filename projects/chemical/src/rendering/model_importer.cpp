#include <pch.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "model_importer.h"
#include "core/defines.h"


namespace Chemical {


	std::shared_ptr<Mesh> ModelImporter::ProcessMesh(aiMesh* in_mesh) {

		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
		for (unsigned int i = 0; i < in_mesh->mNumVertices; i++)
		{
			glm::vec3 pos;
			pos.x = in_mesh->mVertices[i].x;
			pos.y = in_mesh->mVertices[i].y;
			pos.z = in_mesh->mVertices[i].z;
			//glm::vec2 tex = glm::vec2(0, 0);
			//if (in_mesh->mTextureCoords[0]) {
			//	tex.x = in_mesh->mTextureCoords[0][i].x;
			//	tex.y = in_mesh->mTextureCoords[0][i].y;
			//}
			glm::vec3 normal;
			normal.x = in_mesh->mNormals[i].x;
			normal.y = in_mesh->mNormals[i].y;
			normal.z = in_mesh->mNormals[i].z;

			mesh->vertices.emplace_back(Vertex(pos, normal));
		}

		for (unsigned int i = 0; i < in_mesh->mNumFaces; i++)
		{
			aiFace face = in_mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				mesh->indices.emplace_back(face.mIndices[j]);
			}
		}
		mesh->GenerateMesh();
		return mesh;

	}

	std::optional<std::shared_ptr<Mesh>> ModelImporter::ImportModel(const std::string& file_path) {
		
		const aiScene* scene = importer.ReadFile(file_path, 
			aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_OptimizeMeshes | aiProcess_MakeLeftHanded);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			CHEMICAL_CUSTOM_PRINT(Severity::_ERROR, "ImportModel failed with file path {}", file_path);
			return std::nullopt;
		}

		aiMesh* ai_mesh = scene->mMeshes[0];
		return std::optional<std::shared_ptr<Mesh>>(ProcessMesh(ai_mesh));
	}
}