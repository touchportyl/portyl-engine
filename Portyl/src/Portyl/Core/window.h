#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

namespace Portyl
{

  struct WindowProps
  {
    std::string title;
    int width, height;

    WindowProps(
      std::string const& title = "Portyl Engine",
      int width = 1600,
      int height = 900
    ) : title(title), width(width), height(height) {}
  };

  class Window
  {
  public:
    Window(WindowProps props = {})
    {
      CreateWindow(props);
    }
    ~Window() = default;

    bool CreateWindow(WindowProps props);

    static unsigned int GetWidth() { return s_props.width; }
    static unsigned int GetHeight() { return s_props.height; }

    GLFWwindow* GetGLFWWindow() const { return m_glfwwindow; }

  private:
    GLFWwindow* m_glfwwindow{ nullptr };
    static WindowProps s_props;
  };

}