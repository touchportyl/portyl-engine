#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
      int width = 1600,
      int height = 900
    ) : title(title), width(width), height(height) {}
  };

  class Window
  {
  public:
    Window(WindowProps props = {})
    {
      // initialize and configure glfw
      glfwInit();
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

      // todo: custom title bar
      // https://www.glfw.org/docs/3.0/window.html#window_hints
      //glfwWindowHint(GLFW_DECORATED, false);

      // create window
      assert((CreateWindow(props)) && "Window not created!");
    }
    ~Window() = default;

    static unsigned int GetWidth() { return s_props.width; }
    static unsigned int GetHeight() { return s_props.height; }
    static void SetWidth(unsigned int const& width) { s_props.width = width; }
    static void SetHeight(unsigned int const& height) { s_props.height = height; }

    /// <returns>glfw window</returns>
    GLFWwindow* GetGLFWWindow() const { return m_glfwwindow; }

  private:
    bool CreateWindow(WindowProps props);
    GLFWwindow* m_glfwwindow{ nullptr };
    static WindowProps s_props;
  };

}