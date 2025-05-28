
#include <format>
namespace Chemical {

  inline constexpr std::string GetResourceDirectory(const std::string_view &affix = "") {
    if (affix == "")
      return "/mnt/storage/Chemical/Chemical/modules/chemical/res/";
    return std::format("/mnt/storage/Chemical/Chemical/modules/chemical/res/{}", affix);
  }
} // namespace Chemical
