#include "core/resource_manager.h"

#include "io/image.h"
#include "spdlog/spdlog.h"

#include <fstream>
#include <string>
#include <string_view>

namespace Chemical {

  static std::string res_master_path;

  void ResourceManager::SetResourceMasterPath(const std::string_view& in_res_master_path) {
    res_master_path = in_res_master_path;
  }
  std::optional<json> ResourceManager::LoadJSONFile(const std::string_view& res_path) {

    std::ifstream file(res_master_path + res_path.data());

    if (!file) {
      SPDLOG_ERROR(R"(Failed to open json file "{}")", res_path);
      return std::nullopt;
    }
    try {
      json json_data = json::parse(file);
      file.close();
      return json_data;

    } catch (const json::parse_error& e) {
      SPDLOG_ERROR(R"(Failed to parse json file "{}")", res_path);
      SPDLOG_ERROR("{}", e.what());
      file.close();
      return std::nullopt;
    }
  }

  bool ResourceManager::LoadAndRegisterScene(const std::string_view& res_path, I_SORHandler& sor_handler) {
    auto opt_j_scene = LoadJSONFile(res_path);

    if (!opt_j_scene)
      return false;

    json j_scene = opt_j_scene.value();

    auto j_transforms = j_scene.find("transforms");
    if (j_transforms != j_scene.end()) {
      for (auto& j_transform : j_transforms.value()) {
        auto j_id = j_transform.find("transformID");
        if (j_id == j_transform.end()) {
          SPDLOG_WARN("JSON transform contains no transformID");
          continue;
        }
        std::string id = j_id.value();
        Transform t;
        auto j_pos = j_transform.find("position");
        if (j_pos != j_transform.end()) {
          t.position.x = j_pos.value()[0].get<float>();
          t.position.y = j_pos.value()[1].get<float>();
        }
        auto j_scale = j_transform.find("scale");
        if (j_scale != j_transform.end()) {
          t.scale.x = j_pos.value()[0].get<float>();
          t.scale.y = j_scale.value()[1].get<float>();
        }
        auto j_rot = j_transform.find("rotation");
        if (j_rot != j_transform.end()) {
          t.rotation = j_rot.value().get<float>();
        }
        if (!sor_handler.RegisterTransform(id, t))
          SPDLOG_WARN("Failed to register transform.");
      }
    }
    auto j_sprites = j_scene.find("sprites");
    if (j_sprites != j_scene.end()) {
      for (auto& j_sprite : j_sprites.value()) {
        auto j_id = j_sprite.find("spriteID");
        if (j_id == j_sprite.end()) {
          SPDLOG_WARN("JSON sprite contains no spriteID");
          continue;
        }
        std::string id = j_id.value();
        Sprite sprite;
        auto j_t_id = j_sprite.find("transformID");
        if (j_t_id == j_sprite.end()) {
          SPDLOG_WARN("JSON sprite contains no transformID");
          continue;
        }
        sprite.transform_id = j_t_id.value().get<std::string>();

        auto j_m_id = j_sprite.find("materialID");
        if (j_m_id == j_sprite.end()) {
          SPDLOG_WARN("JSON sprite contains no materialID");
          continue;
        }
        sprite.material_id = j_m_id.value().get<std::string>();
        auto j_size = j_sprite.find("size");
        if (j_size != j_sprite.end()) {
          sprite.size.x = j_size.value()[0].get<float>();
          sprite.size.y = j_size.value()[1].get<float>();
        }
        if (!sor_handler.RegisterSprite(id, sprite)) {
          SPDLOG_WARN("Failed to register sprite.");
        }
      }
      auto j_mtls = j_scene.find("materials");
      if (j_mtls != j_scene.end()) {
        for (auto& j_mtl : j_mtls.value()) {
          auto j_id = j_mtl.find("materialID");
          if (j_id == j_mtl.end()) {
            SPDLOG_WARN("JSON material contains no materialID");
            continue;
          }
          std::string id = j_id.value();
          Material material;
          auto j_s_id = j_mtl.find("shaderID");
          if (j_s_id == j_mtl.end()) {
            SPDLOG_WARN("JSON material contains no shaderID");
            continue;
          }
          material.shader_id = j_s_id.value().get<std::string>();
          auto j_t_path = j_mtl.find("texturePath");
          if (j_t_path == j_mtl.end()) {
            SPDLOG_WARN("JSON material contains no texturePath");
            continue;
          }
          material.texture_id = j_t_path.value().get<std::string>();

          auto j_tint = j_mtl.find("tint");
          if (j_tint != j_mtl.end()) {
            material.tint.r = j_tint.value()[0].get<unsigned int>();
            material.tint.g = j_tint.value()[1].get<unsigned int>();
            material.tint.b = j_tint.value()[2].get<unsigned int>();
          }
          if (!sor_handler.RegisterMaterial(id, material)) {
            SPDLOG_WARN("Failed to register material.");
          }
        }
      }
      auto j_camera = j_scene.find("mainCamera");
      if (j_camera != j_scene.end()) {
        auto j_t_id = j_camera->find("transformID");
        if (j_t_id == j_camera->end()) {
          SPDLOG_WARN("JSON camera contains no transformID");
          return false;
        }
        Camera camera;
        camera.transform_id = j_t_id.value();

        auto j_zoom = j_camera->find("zoomLevel");
        if (j_zoom != j_camera->end()) {
          camera.SetZoomLevel(j_zoom.value().get<float>());
        }
        sor_handler.SetMainCamera(camera);
      }
    }
    return true;
  }

  bool ResourceManager::LoadAndRegisterProject(const std::string_view& res_path, I_RRHandler& rr_handler) {
    auto opt_j_texs = LoadJSONFile(res_path);

    if (!opt_j_texs)
      return false;

    auto f_j_texs = opt_j_texs.value().find("textures");

    if (f_j_texs == opt_j_texs.value().end()) {
      SPDLOG_INFO("No textures in textures.json");
      return false;
    }
    auto j_texs = f_j_texs.value();

    for (auto j_tex : j_texs) {

      auto j_img_path = j_tex.find("texturePath");
      std::string img_path;
      if (j_img_path == j_tex.end())
        img_path = res_master_path + j_img_path.value().get<std::string>();
      else
        img_path = res_master_path + j_img_path.value().get<std::string>();

      auto img = LoadImageFile(img_path);
      if (!img) {
        SPDLOG_ERROR(R"(FAILED TO LOAD IMAGE "{}")", img_path);
        continue;
      }
      // TODO: Process texture filters from json
      TextureTraits traits(img);

      if (!rr_handler.RegisterTexture(j_img_path.value().get<std::string>(), traits)) {
        SPDLOG_WARN("Failed to register texture.");
      }
    }
    return true;
  }

} // namespace Chemical
