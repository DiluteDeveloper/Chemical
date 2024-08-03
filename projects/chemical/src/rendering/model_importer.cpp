#include <pch.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <spdlog/spdlog.h>

#include "model_importer.h"


namespace Chemical {

	GeneratedMesh3D ModelImporter::ProcessMesh(aiMesh* f_mesh) {

		GeneratedMesh3D mesh;
		for (unsigned int i = 0; i < f_mesh->mNumVertices; i++)
		{
			glm::vec3 pos;
			pos.x = f_mesh->mVertices[i].x;
			pos.y = f_mesh->mVertices[i].y;
			pos.z = f_mesh->mVertices[i].z;
			//glm::vec2 tex = glm::vec2(0, 0);
			//if (in_mesh->mTextureCoords[0]) {
			//	tex.x = in_mesh->mTextureCoords[0][i].x;
			//	tex.y = in_mesh->mTextureCoords[0][i].y;
			//}
			glm::vec3 normal;
			normal.x = f_mesh->mNormals[i].x;
			normal.y = f_mesh->mNormals[i].y;
			normal.z = f_mesh->mNormals[i].z;

			mesh.vertices.emplace_back(GeneratedMesh3D::Vertex(pos, normal));
		}

		for (unsigned int i = 0; i < f_mesh->mNumFaces; i++)
		{
			aiFace face = f_mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				mesh.indices.emplace_back(face.mIndices[j]);
			}
		}
		mesh.GenerateMesh();
		return mesh;

	}

	std::unique_ptr<GeneratedMesh3D> ModelImporter::ImportModel(const std::string& filePath) {
		
		const aiScene* scene = importer.ReadFile(filePath, 
			aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_OptimizeMeshes | aiProcess_MakeLeftHanded);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			spdlog::error("ImportModel failed with file path {0}", filePath);
			return nullptr;
		}

		aiMesh* aiMesh = scene->mMeshes[0];
		return std::make_unique<GeneratedMesh3D>(ProcessMesh(aiMesh));
	}
}