#include "chemical/resource_manager.h"

#include "chemical/util/image.h"
#include "spdlog/spdlog.h"

#include <fstream>
#include <nlohmann-json/json.hpp>
#include <string>
#include <string_view>

namespace Chemical {

  using json = nlohmann::json;

  std::optional<json> LoadJSONFile(const std::string_view& path) {
    std::ifstream file(path.data());

    if (!file) {
      SPDLOG_ERROR(R"(Failed to open json file "{}")", path);
      return std::nullopt;
    }
    json json_data = json::parse(file);
    file.close();
    if (json_data.is_discarded()) {
      SPDLOG_ERROR(R"(Failed to parse json file "{}")", path);
      return std::nullopt;
    }
    return json_data;
  }

  std::optional<Scene> ResourceManager::LoadSceneJSONFile(const std::string_view& resource_path) {
    auto opt_j_scene = LoadJSONFile(std::string_view(resource_master_path + resource_path.data()));

    if (!opt_j_scene)
      return std::nullopt;

    json j_scene = opt_j_scene.value();
    Scene scene;

    auto j_transforms = j_scene.find("transforms");
    if (j_transforms != j_scene.end()) {
      for (auto& j_transform : j_transforms.value()) {
        auto j_id = j_transform.find("id");
        if (j_id == j_transform.end()) {
          SPDLOG_WARN("JSON transform contains no id");
          continue;
        }
        std::string id = j_id.value();
        Transform t;
        auto j_pos = j_transform.find("position");
        if (j_pos != j_transform.end()) {
          t.position.x = j_pos.value()[0].get<float>();
          t.position.y = j_pos.value()[1].get<float>();
        }
        auto j_rot = j_transform.find("rotation");
        if (j_rot != j_transform.end()) {
          t.rotation = j_rot.value().get<float>();
        }
        scene.RegisterTransform(id, t);
      }
    }
    auto j_sprites = j_scene.find("sprites");
    if (j_sprites != j_scene.end()) {
      for (auto& j_sprite : j_sprites.value()) {
        auto j_id = j_sprite.find("id");
        if (j_id == j_sprite.end()) {
          SPDLOG_WARN("JSON sprite contains no id");
          continue;
        }
        std::string id = j_id.value();
        Sprite sprite;
        auto j_t_id = j_sprite.find("transform_id");
        if (j_t_id == j_sprite.end()) {
          SPDLOG_WARN("JSON sprite contains no transform id");
          continue;
        }
        sprite.transform_id = j_t_id.value().get<std::string>();

        auto j_m_id = j_sprite.find("material_id");
        if (j_m_id == j_sprite.end()) {
          SPDLOG_WARN("JSON sprite contains no material id");
          continue;
        }
        sprite.material_id = j_m_id.value().get<std::string>();
        auto j_size = j_sprite.find("size");
        if (j_size != j_sprite.end()) {
          sprite.size.x = j_size.value()[0].get<float>();
          sprite.size.y = j_size.value()[1].get<float>();
        }
        scene.RegisterSprite(id, sprite);
      }
    }
    return scene;
  }

  std::optional<StrValVec<MaterialTraits>> ResourceManager::LoadMaterialsJSONFile() {
    auto opt_j_mtls = LoadJSONFile(resource_master_path + "materials.json");

    if (!opt_j_mtls)
      return std::nullopt;

    auto f_j_mtls = opt_j_mtls.value().find("materials");

    if (f_j_mtls == opt_j_mtls.value().end()) {
      SPDLOG_INFO("No materials in materials json");
      return std::nullopt;
    }
    StrValVec<MaterialTraits> vec;
    auto j_mtls = f_j_mtls.value();

    for (auto j_mtl : j_mtls) {
      auto j_id = j_mtl.find("id");
      if (j_id == j_mtl.end()) {
        SPDLOG_WARN("JSON material contains no id");
        continue;
      }
      std::string id = j_id.value();

      auto j_tex_path = j_mtl.find("texturePath");
      Util::Image img;
      if (j_tex_path == j_mtl.end()) {
        auto opt_img = Util::LoadImage(std::string_view(resource_master_path + "textures/white.png"));
        if (opt_img) {
          img = std::move(opt_img.value());
        } else {
          SPDLOG_ERROR("FAILED TO LOAD IMAGE white.png");
          return std::nullopt;
        }
      } else {
        auto opt_img = Util::LoadImage(resource_master_path + j_tex_path.value().get<std::string>());
        if (opt_img) {
          img = std::move(opt_img.value());
        } else {
          SPDLOG_ERROR(R"(FAILED TO LOAD IMAGE "{}")", j_tex_path.value().get<std::string>());
          return std::nullopt;
        }
      }
      MaterialTraits mtl(img);

      auto j_tint = j_mtl.find("tint");
      if (j_tint != j_mtl.end()) {
        mtl.tint.r = j_tint.value()[0].get<unsigned int>();
        mtl.tint.g = j_tint.value()[1].get<unsigned int>();
        mtl.tint.b = j_tint.value()[2].get<unsigned int>();
      }
      vec.emplace_back(id, mtl);
    }
    return vec;
  }

  std::optional<StrValVec<ShaderTraits>> ResourceManager::LoadShadersJSONFile() {
    return std::nullopt;
  }
} // namespace Chemical
