#include "AssetImporterLayer.h"
#include "chemical/io/FileStream.h"
#include "chemical/io/ConsoleStream.h"

#include <filesystem>
#include <vendor/json/json.hpp>

namespace Chemical {

    namespace Core {

        Render::Scene AssetImporterLayer::LoadSceneFromFile(const std::filesystem::path& filePath) {
            nlohmann::json j;
            try {
               j = nlohmann::json::parse(IO::ReadFile(filePath));
            }
            catch (std::exception& e) {
                IO::LogError("Chemical::Core::AssetImporterLayer::LoadSceneFromFile({}) json does not compile.", filePath);
                throw e;
            }


            Render::Scene scene;

            for (const auto& rJ : j["renderables"].items())
            {
                try {
                    Render::Renderable r(GetMesh("mesh"), GetMaterial("material"));

                    r.transform.position.x = rJ.value()["transform"][0];
                    r.transform.position.y = rJ.value()["transform"][1];
                    r.transform.position.z = rJ.value()["transform"][2];
                    r.transform.rotation.x = rJ.value()["transform"][3];
                    r.transform.rotation.y = rJ.value()["transform"][4];
                    r.transform.rotation.z = rJ.value()["transform"][5];
                    r.transform.scale.x = rJ.value()["transform"][6];
                    r.transform.scale.y = rJ.value()["transform"][7];
                    r.transform.scale.z = rJ.value()["transform"][8];

                    Render::Shader& s = GetShader(rJ.value()["shader"]);
                    auto location = scene.renderables.find(&s);
                    if (location == scene.renderables.end()) {
                        scene.renderables.emplace(&s, std::vector<Render::Renderable>());
                    }

                    scene.renderables[&s].emplace_back(r);
                }
                catch (std::exception& e) {
                    IO::LogError("Chemical::Core::AssetImporterLayer::LoadSceneFromFile({}) failed.", filePath);
                    throw e;
                }
            }

            return scene;
        }

        std::vector<std::filesystem::path> RecursiveDirectories(std::filesystem::path path) {
            std::vector<std::filesystem::path> dirs;
            for (const std::filesystem::directory_entry& file : std::filesystem::recursive_directory_iterator(path)) {
                dirs.emplace_back(file.path());
            }
            return dirs;
        }

        AssetImporterLayer::AssetImporterLayer() {
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

            materials["material"] = Render::Material();
            Render::Mesh mesh(vertices, indices);
            meshes.emplace("mesh", mesh);

            std::vector<std::filesystem::path> vertexShaderPaths;
            std::vector<std::filesystem::path> fragmentShaderPaths;
            std::vector<std::filesystem::path> uniformPaths;
            std::vector<std::filesystem::path> scenePaths;

            std::vector<std::filesystem::path> resources = RecursiveDirectories("res/");

            for (auto& path : resources) {

                if (path.parent_path() != "res") {

                    resourceStructure[path.parent_path().generic_string().c_str()].emplace_back(path);
                }
                else {
                    resourceStructure["res"].emplace_back(path);
                }

                std::string ext(path.extension().generic_string()); // path.extension().c_str() returns wchar_t*;

                if (path.extension() == ".vs")
                    vertexShaderPaths.emplace_back(path);
                else if (path.extension() == ".fs")
                    fragmentShaderPaths.emplace_back(path);
                else if (path.extension() == ".uniforms")
                    uniformPaths.emplace_back(path);
                else if (path.extension() == ".scene")
                    scenePaths.emplace_back(path);


            }

            for (const std::filesystem::path& vsp : vertexShaderPaths) {
                for (const std::filesystem::path& fsp : fragmentShaderPaths) {
                    for (const std::filesystem::path& up : uniformPaths) {
                        if (vsp.stem() == fsp.stem() && vsp.stem() == up.stem()) {
                            shaders.emplace(vsp.generic_string(), Render::Shader(vsp, fsp, up));
                        }
                    }

                }
            }

            for (std::filesystem::path& p : scenePaths) {
                Render::Scene s = LoadSceneFromFile(p);
                scenes.emplace(p.generic_string(), s);
            }
        }

        Render::Mesh& AssetImporterLayer::GetMesh(const std::string& filePath){
            auto location = meshes.find(filePath);
            if (location != meshes.end())
                return location->second;
            IO::LogError("Chemical::Core::AssetImporter::Layer::GetMesh({}) mesh does not exist.", filePath);
            throw std::exception();
        }
        Render::Material& AssetImporterLayer::GetMaterial(const std::string& filePath){

            auto location = materials.find(filePath);
            if (location != materials.end())
                return location->second;
            IO::LogWarning("Chemical::Core::AssetImporter::Layer::GetMaterial({}) material does not exist.", filePath);
            throw std::exception();
        }
        Render::Shader& AssetImporterLayer::GetShader(const std::string& filePath) {
            auto location = shaders.find(filePath);
            if (location != shaders.end())
                return location->second;
            IO::LogWarning("Chemical::Core::AssetImporter::Layer::GetShader({}) shader does not exist.", filePath);
            throw std::exception();
        }

        Render::Scene& AssetImporterLayer::GetScene(const std::string& filePath){

            auto location = scenes.find(filePath);
            if (location != scenes.end())
                return location->second;
            IO::LogWarning("Chemical::Core::AssetImporter::Layer::GetScene({}) scene does not exist.", filePath);
            throw std::exception();
        }
    }


}
