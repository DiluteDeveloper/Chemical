#pragma once

#include <string>
namespace Chemical {

  inline std::string GetResourceDirectory(const std::string_view &affix) {
    if (affix == "")
      return "/mnt/storage/Chemical/Chemical/modules/chemical/";
    else {
      return std::string("/mnt/storage/Chemical/Chemical/modules/chemical/" +
                         std::string(affix));
    }
  }
} // namespace Chemical
