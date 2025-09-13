#pragma once

#include "util/transform.hpp"
#include <string>
#include <vector>
namespace Chemical {
  namespace GUI {

    // returns a selection
    extern int RenderTransformWindow(
        std::vector<std::pair<Transform *, std::string *>> &transforms);

  } // namespace GUI
} // namespace Chemical
