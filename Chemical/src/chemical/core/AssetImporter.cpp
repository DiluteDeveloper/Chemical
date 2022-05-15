#include "AssetImporter.h"

#include <filesystem>

namespace Chemical {

    namespace Core {

        std::vector<std::filesystem::path> RecursiveDirectories(std::filesystem::path path) {
            std::vector<std::filesystem::path> dirs;
            for (const std::filesystem::directory_entry& file : std::filesystem::recursive_directory_iterator(path)) {
                dirs.emplace_back(file.path());
            }
            return dirs;
        }

        AssetImporter::AssetImporter() {
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

            std::vector<std::filesystem::path> vertexShaderPaths;
            std::vector<std::filesystem::path> fragmentShaderPaths;

            std::vector<std::filesystem::path> resources = RecursiveDirectories("res/");

            for (auto& path : resources) {

                if (path.parent_path() != "res") {

                    resourceStructure[path.parent_path().generic_string().c_str()].emplace_back(path);
                }
                else {
                    resourceStructure["res"].emplace_back(path);
                }

                std::string ext(path.extension().generic_string()); // path.extension().c_str() returns wchar_t*;

                if (path.extension() == ".v")
                    vertexShaderPaths.emplace_back(path);
                else if (path.extension() == ".f")
                    fragmentShaderPaths.emplace_back(path);
            }

            for (const std::filesystem::path& vsp : vertexShaderPaths) {
                for (const std::filesystem::path& fsp : fragmentShaderPaths) {
                    if (vsp.filename() == fsp.filename()) {
                        shaders.emplace_back(vsp, fsp);
                    }
                }
            }
        }

        /*std::tuple<const Mesh*, bool> AssetImporter::GetMesh(const std::string& filePath) const {
            for (const Mesh& m : meshes) {
                if (m.filePath == filePath)
                    return std::make_tuple<const Mesh*, bool>(&m, false);
            }
            std::cout << "Mesh \"" << filePath << "\" doesnt exist." << std::endl;
            return std::make_tuple<const Mesh*, bool>(nullptr, true);
        }
        std::tuple<const Material*, bool> AssetImporter::GetMaterial(const std::string& filePath) const {

            std::weak_ptr<Material> m(std::make_shared<Material>(&materials[0]));
            auto ma = m.lock();
            auto b = ma.get();
            b->ambient = glm::vec3(1.0f);
            for (const Material& m : materials) {

                if (m.filePath == filePath)
                    return std::make_tuple<const Material*, bool>(&m, false);;
            }
            std::cout << "Material \"" << filePath << "\" doesnt exist." << std::endl;
            return std::make_tuple<const Material*, bool>(nullptr, true);
        }
        std::tuple<const Shader*, bool> AssetImporter::GetShader(const std::string& filePath) const {
            for (const Shader& s : shaders) {
                if (s.vertexPath == filePath)
                    return std::make_tuple<const Shader*, bool>(&s, false);
                else if (s.fragmentPath == filePath)
                    return std::make_tuple<const Shader*, bool>(nullptr, true);
            }
            std::cout << "Shader \"" << filePath << "\" doesnt exist." << std::endl;
            return std::make_tuple<const Shader*, bool>(nullptr, true);
        }*/

        const std::vector<Mesh>& AssetImporter::GetMeshes() const {
            return meshes;
        }
        const std::vector<Material>& AssetImporter::GetMaterials() const {
            return materials;
        }
        const std::vector<Shader>& AssetImporter::GetShaders() const {
            return shaders;
        }

        const std::unordered_map<std::string, std::vector<std::filesystem::path>>& AssetImporter::GetResourceStructure() const {
            return resourceStructure;
        }
    }


}
