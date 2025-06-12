#pragma once

#include <optional>
#include <string>

namespace Chemical {

  struct ShaderTraits {
    std::optional<std::string> vs_source = std::nullopt;
    std::optional<std::string> fs_source = std::nullopt;
    std::optional<std::string> gs_source = std::nullopt;
    std::optional<std::string> cs_source = std::nullopt;
  };
} // namespace Chemical
