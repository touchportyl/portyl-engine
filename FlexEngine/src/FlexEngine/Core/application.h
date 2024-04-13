#pragma once

#include <memory>

#include "window.h"
#include "layerstack.h"
#include "layer.h"
#include "imgui.h"

int main(int, char**);

namespace FlexEngine
{

  // the architecture of the engine is designed to have only one application instance
  class Application
  {
  public:
    Application();
    virtual ~Application();

    // Closes the application and all windows
    // Sets the glfwSetWindowShouldClose flag
    static void Close();

    // Window handling

    static std::shared_ptr<Window> OpenWindow(const WindowProps& props = {});

    static void CloseWindow(std::string& window_title);
    static void CloseWindow(std::shared_ptr<Window> window);

    // checks

    static bool IsRunning() { return m_is_running; }
    static bool IsMinimized() { return m_is_minimized; }

    // Returns all the windows
    static const std::vector<std::shared_ptr<Window>>& GetWindows() { return m_windows; }

  private:
    static void Run();

    static bool m_is_running;
    static bool m_is_minimized;

    static std::vector<std::shared_ptr<Window>> m_windows;

    friend int ::main(int, char**);
  };

  Application* CreateApplication();
}
