#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

namespace FlexEngine
{

  /// <summary>
  /// Window properties.
  /// </summary>
  struct WindowProps
  {
    std::string title;
    int width, height;

    WindowProps(
      std::string const& title = "FlexEngine",
      int width = 1280,
      int height = 720
    ) : title(title), width(width), height(height) {}
  };

  class Window
  {
  public:
    Window(WindowProps const& props = {});
    ~Window() = default;

    static unsigned int GetWidth() { return s_props.width; }
    static unsigned int GetHeight() { return s_props.height; }
    static void SetWidth(unsigned int const& width) { s_props.width = width; }
    static void SetHeight(unsigned int const& height) { s_props.height = height; }

    /// <returns>glfw window</returns>
    GLFWwindow* GetGLFWWindow() const { return m_glfwwindow; }

  private:
    GLFWwindow* m_glfwwindow{ nullptr };
    static WindowProps s_props;
  };

}