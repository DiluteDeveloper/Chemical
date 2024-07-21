#include <pch.h>
#include "model_importer.h"

#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace Chemical {
	
	bool ModelImporter::import_success = true;
	Mesh ModelImporter::imported_mesh;

	Assimp::Importer ModelImporter::importer;


	Mesh ProcessMesh(aiMesh* in_mesh) {

		Mesh mesh;
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

			mesh.vertices.emplace_back(Vertex(pos, normal));
		}

		for (unsigned int i = 0; i < in_mesh->mNumFaces; i++)
		{
			aiFace face = in_mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				mesh.indices.emplace_back(face.mIndices[j]);
			}
		}
		return mesh;

	}

	void ModelImporter::ImportModel(const std::string& file_path) {
		
		const aiScene* scene = importer.ReadFile(file_path, 
			aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_OptimizeMeshes | aiProcess_MakeLeftHanded);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			CHEMICAL_DEBUG_CALL(LOGGER_CONSOLE_CUSTOM_WARNING("ImportModel() failed with path: {}", file_path));
			import_success = false;
			return;
		}

		aiMesh* ai_mesh = scene->mMeshes[0];
		imported_mesh = ProcessMesh(ai_mesh);
		import_success = true;
	}
}