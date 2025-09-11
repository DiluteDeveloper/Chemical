#pragma once

#include "util/transform.hpp"
#include <string>
#include <vector>
namespace Chemical {
  namespace GUI {

    extern void RenderTransformWindow(
        std::vector<std::pair<Transform *, std::string *>> &transforms);

  }
} // namespace Chemical
