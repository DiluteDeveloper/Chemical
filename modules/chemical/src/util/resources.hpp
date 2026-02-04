#pragma once

#include <string>
namespace Chemical {

  inline std::string GetResourceDirectory(const std::string_view &affix) {
    if (affix == "")
      return "/home/dilute/Documents/Dev/Chemical/modules/chemical/";
    else {
      return std::string(
          "/home/dilute/Documents/Dev/Chemical/modules/chemical/" +
          std::string(affix));
    }
  }
} // namespace Chemical
