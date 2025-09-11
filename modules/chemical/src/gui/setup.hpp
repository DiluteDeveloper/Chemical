#pragma once

struct GLFWwindow;

namespace Chemical {

  namespace GUI {
    extern void SetupGUI(GLFWwindow *window);
    extern void CleanupGUI();

    extern void BeginFrame();
    extern void RenderFrame();
  } // namespace GUI
} // namespace Chemical
