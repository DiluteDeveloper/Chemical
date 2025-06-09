// #include "chemical/io/json_scene.h"
//
// #include "chemical/graphics/shader.h"
// #include "chemical/util/image.h"
// #include "spdlog/spdlog.h"
//
// #include <fstream>
// #include <nlohmann-json/json.hpp>
// #include <optional>
//
// using json = nlohmann::json;
//
// namespace Chemical {
//
//   void LoadTransformsFromSceneJSON(const json &scene_json, Scene &scene) {
//
//     for (auto &tfm_json : scene_json["transforms"]) {
//       auto pos_json = tfm_json["position"];
//       glm::vec2 position = glm::vec2(pos_json[0].get<float>(), pos_json[1].get<float>());
//
//       float rotation = tfm_json["rotation"].get<float>();
//
//       auto scale_json = tfm_json["scale"];
//       glm::vec2 scale = glm::vec2(scale_json[0].get<float>(), scale_json[1].get<float>());
//
//       scene.CreateTransform(tfm_json["id"].get<std::string>(), position, rotation, scale);
//     }
//   }
//   void LoadTexturesFromSceneJSON(const json &scene_json, Scene &scene) {
//     for (auto &tex_json : scene_json["textures"]) {
//       std::string id = tex_json["id"].get<std::string>();
//       std::optional<Util::Image> image =
//           Util::LoadImage(scene.GetResourcePath(id), tex_json["requiredComponents"].get<unsigned int>());
//
//       if (image.has_value()) {
//         scene.CreateTexture(id, image.value());
//       }
//     }
//   }
//   void LoadShadersFromSceneJSON(const json &scene_json, Scene &scene) {
//     for (auto &shd_json : scene_json["shaders"]) {
//       Graphics::ShaderTraits traits;
//
//       std::string vs = shd_json["vertexShaderPath"].get<std::string>();
//       std::string fs = shd_json["fragmentShaderPath"].get<std::string>();
//       std::string gs = shd_json["geometryShaderPath"].get<std::string>();
//       std::string cs = shd_json["computeShaderPath"].get<std::string>();
//       if (vs != "")
//         traits.vs_file_path = scene.GetResourcePath(vs);
//       if (fs != "")
//         traits.fs_file_path = scene.GetResourcePath(fs);
//       if (gs != "")
//         traits.gs_file_path = scene.GetResourcePath(gs);
//       if (cs != "")
//         traits.cs_file_path = scene.GetResourcePath(cs);
//
//       Graphics::Shader shader(traits);
//
//       if (shader.compile_status == -1)
//         return;
//
//       scene.MoveConstructShader(shd_json["id"].get<std::string>(), std::move(shader));
//     }
//   }
//   void LoadMaterialsFromSceneJSON(const json &scene_json, Scene &scene) {
//
//     for (auto &mtl_json : scene_json["materials"]) {
//       auto tint_json = mtl_json["tint"];
//       glm::vec3 tint =
//           glm::vec3(tint_json[0].get<float>(), tint_json[1].get<float>(), tint_json[2].get<float>());
//
//       scene.CreateMaterial(mtl_json["id"].get<std::string>(), mtl_json["texture_id"].get<std::string>(),
//                            tint);
//     }
//   }
//   void LoadStaticSpritesFromSceneJSON(const json &scene_json, Scene &scene) {
//
//     for (auto &ssp_json : scene_json["static_sprites"]) {
//       auto size_json = ssp_json["size"];
//       glm::vec2 size = glm::vec2(size_json[0].get<float>(), size_json[1].get<float>());
//       scene.CreateStaticSprite(ssp_json["shader_id"].get<std::string>(),
//                                ssp_json["transform_id"].get<std::string>(),
//                                ssp_json["material_id"].get<std::string>(), size);
//     }
//   }
//   void LoadCamerasFromSceneJSON(const json &scene_json, Scene &scene) {
//     for (auto &cam_json : scene_json["cameras"]) {
//       std::string id = cam_json["id"].get<std::string>();
//       scene.CreateCamera(id, cam_json["transform_id"].get<std::string>(),
//                          cam_json["zoom_level"].get<float>());
//     }
//   }
//
//   std::optional<Scene> LoadSceneFromJSONFile(const std::string_view &file_path) {
//     std::ifstream file(file_path.data());
//
//     if (!file) {
//       SPDLOG_ERROR(R"(Failed to read file "{}" : returning 0)", file_path);
//       return std::nullopt;
//     }
//     json scene_json = json::parse(file);
//
//     Scene scene("/mnt/storage/Chemical/Chemical/modules/chemical/res/");
//
//     LoadTransformsFromSceneJSON(scene_json, scene);
//     LoadTexturesFromSceneJSON(scene_json, scene);
//     LoadMaterialsFromSceneJSON(scene_json, scene);
//     LoadShadersFromSceneJSON(scene_json, scene);
//     LoadStaticSpritesFromSceneJSON(scene_json, scene);
//     LoadCamerasFromSceneJSON(scene_json, scene);
//
//     return scene;
//   }
// } // namespace Chemical
