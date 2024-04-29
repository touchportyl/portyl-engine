#pragma once

#include "flx_api.h"

#include "window.h"

#include <string>
#include <vector>
#include <memory> // std::shared_ptr

int main(int, char**);

namespace FlexEngine
{

  // The architecture of the engine is designed to only have one application instance.
  class __FLX_API Application
  {
  public:
    Application();
    virtual ~Application();

    // Closes the application and all windows
    // Sets the glfwSetWindowShouldClose flag
    static void Close();

    #pragma region Window Handling Functions

    static std::shared_ptr<Window> OpenWindow(const WindowProps& props = {});

    static void CloseWindow(std::string& window_title);
    static void CloseWindow(std::shared_ptr<Window> window);

    #pragma endregion

    // Checks

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
