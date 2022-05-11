#include "AssetImporter.h"

#include <filesystem>
#include <iostream>

namespace Chemical {

    namespace Core {

        std::vector<std::filesystem::path> RecursiveDirectories(std::filesystem::path path) {
            std::vector<std::filesystem::path> dirs;
            for (const std::filesystem::directory_entry& file : std::filesystem::recursive_directory_iterator(path)) {
                dirs.emplace_back(file.path());
            }
            return dirs;
        }

        AssetImporter::AssetImporter(AccessKey<Application>) {
            std::cout << "AssetImporter constructor called." << std::endl;
            // Temporarily manually loading materials and meshes as the files for these are subject to change drastically

            std::vector<float> vertices{
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, -1.0f, // BACK
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f, 0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f, 0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f, 0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, -1.0f,


            -0.5f, -0.5f, 0.5f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f, // FRONT
             0.5f, -0.5f, 0.5f,   1.0f, 0.0f,   0.0f, 0.0f, 1.0f,
             0.5f,  0.5f, 0.5f,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f,
             0.5f,  0.5f, 0.5f,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f,
            -0.5f,  0.5f, 0.5f,   0.0f, 1.0f,   0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  -1.0f, 0.0f, 0.0f, // LEFT
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,

             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f, // RIGHT
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f,


            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f, -1.0f, 0.0f, // BOTTOM
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 1.0f,  0.0f, -1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 1.0f,  0.0f, -1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  0.0f, -1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f, -1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f, // TOP
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f
            };

            std::vector<unsigned int> indices{
                0,1,2,
                3,4,5,
                6,7,8,
                9,10,11,
                12,13,14,
                15,16,17,
                18,19,20,
                21,22,23,
                24,25,26,
                27,28,29,
                30,31,32,
                33,34,35
            };

            materials.emplace_back("res/materials/DefaultMaterial.mat");
            meshes.emplace_back(vertices, indices, "res/meshes/DefaultMesh.mesh");

            std::vector<std::string> vertexShaderPaths;
            std::vector<std::string> fragmentShaderPaths;

            std::vector<std::filesystem::path> resources = RecursiveDirectories("res/");

            for (auto& path : resources) {

                std::string ext(path.extension().generic_string()); // path.extension().c_str() returns wchar_t*
                std::string filePath(path.generic_string());

                if (ext.compare(".v") == 0)
                    vertexShaderPaths.emplace_back(filePath);
                else if (ext.compare(".f") == 0)
                    fragmentShaderPaths.emplace_back(filePath);
                else if (ext.compare("") == 0)
                    folders.emplace_back(filePath);
            }

            for (const std::string& vsp : vertexShaderPaths) {
                for (const std::string& fsp : fragmentShaderPaths) {
                    std::string vspNoEXT = vsp.substr(0, vsp.length() - 1);
                    std::string fspNoEXT = fsp.substr(0, fsp.length() - 1);
                    if (vspNoEXT.compare(fspNoEXT) == 0) {
                        shaders.emplace_back(vsp, fsp);
                    }
                }
            }
        }

        Mesh* AssetImporter::GetMesh(const std::string& filePath) {
            for (Mesh& m : meshes) {
                if (m.filePath == filePath)
                    return &m;
            }
            std::cout << "Mesh \"" << filePath << "\" doesnt exist." << std::endl;
            return nullptr;
        }
        Material* AssetImporter::GetMaterial(const std::string& filePath) {
            for (Material& m : materials) {
                if (m.filePath == filePath)
                    return &m;
            }
            std::cout << "Material \"" << filePath << "\" doesnt exist." << std::endl;
            return nullptr;
        }
        Shader* AssetImporter::GetShader(const std::string& filePath) {
            for (Shader& s : shaders) {
                if (s.vertexPath == filePath)
                    return &s;
                else if (s.fragmentPath == filePath)
                    return &s;
            }
            std::cout << "Shader \"" << filePath << "\" doesnt exist." << std::endl;
            return nullptr;
        }
    }


}
