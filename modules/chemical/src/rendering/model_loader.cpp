#include "model_loader.hpp"

#include "assimp/material.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "spdlog/spdlog.h"
#include "util/resources.hpp"

namespace Chemical {

  Assimp::Importer ModelLoader::importer;

  Mesh ProcessMesh(aiMesh *mesh);
  void ProcessNode(aiNode *node, const aiScene *loaded_scene, Model &model);

  std::optional<Model>
  ModelLoader::LoadModel(const std::string_view &file_path) {

    const aiScene *loaded_scene = ModelLoader::importer.ReadFile(
        GetResourceDirectory(file_path.data()),
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals |
            aiProcess_FlipWindingOrder | aiProcess_OptimizeMeshes);

    if (!loaded_scene || loaded_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !loaded_scene->mRootNode) {
      SPDLOG_ERROR("Error occurred loading assimp scene: {}",
                   importer.GetErrorString());
      return std::nullopt;
    }
    Model model;
    ProcessNode(loaded_scene->mRootNode, loaded_scene, model);
    return model;
  }
  void ProcessNode(aiNode *node, const aiScene *loaded_scene, Model &model) {

    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
      aiMesh *mesh = loaded_scene->mMeshes[node->mMeshes[i]];
      aiMaterial *material = loaded_scene->mMaterials[mesh->mMaterialIndex];
      model.meshes.emplace_back(std::move(ProcessMesh(mesh)));

      aiColor3D color;
      material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
      model.material.diffuse = glm::vec3(color.r, color.g, color.b);
      // material->Get(AI_MATKEY_COLOR_AMBIENT, color);
      model.material.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
      material->Get(AI_MATKEY_COLOR_SPECULAR, color);
      model.material.specular = glm::vec3(color.r, color.g, color.b);
      float shininess = 0.0f;
      material->Get(AI_MATKEY_SHININESS, shininess);
      model.material.shininess = shininess;
      SPDLOG_INFO("Material: {}", model.material.ToString());
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
      ProcessNode(node->mChildren[i], loaded_scene, model);
    }

    // need to process materials somewhere here
  }

  Mesh ProcessMesh(aiMesh *mesh) {
    Mesh real_mesh;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
      Vertex &v = real_mesh.vertices.emplace_back();
      v.position.x = mesh->mVertices[i].x;
      v.position.y = mesh->mVertices[i].z;
      v.position.z = mesh->mVertices[i].y;
      // glm::vec2 tex = glm::vec2(0, 0);
      // if (mesh->mTextureCoords[0]) {
      //   tex.x = mesh->mTextureCoords[0][i].x;
      //   tex.y = mesh->mTextureCoords[0][i].y;
      // }

      v.normal.x = mesh->mNormals[i].x;
      v.normal.y = mesh->mNormals[i].z;
      v.normal.z = mesh->mNormals[i].y;
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
      aiFace face = mesh->mFaces[i];
      for (unsigned int j = 0; j < face.mNumIndices; j++) {
        real_mesh.indices.emplace_back(face.mIndices[j]);
      }
    }
    return real_mesh;
  }
} // namespace Chemical
