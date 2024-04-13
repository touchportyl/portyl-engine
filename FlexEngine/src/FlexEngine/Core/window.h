#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h> // put glad before glfw

#include <string>
#include <vector>

#include <imgui.h>

#include "FlexEngine/Core/layerstack.h"

namespace FlexEngine
{

  // Window properties.
  // Pass in window hints as a vector of pairs of integers.
  // Window hints cannot be changed after the window is created.
  // https://www.glfw.org/docs/3.0/window.html#window_hints
  struct WindowProps
  {
    std::string title;
    int width, height;
    const std::vector<std::pair<int, int>> window_hints;
    const char* opengl_version_text;

    WindowProps(
      std::string const& title = "FlexEngine",
      int width = 1280, int height = 720,
      std::vector<std::pair<int, int>> window_hints = {
        { GLFW_CONTEXT_VERSION_MAJOR, 4 },
        { GLFW_CONTEXT_VERSION_MINOR, 6 },
        { GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE }
      },
      const char* opengl_version_text = "#version 460 core"
    )
      : title(title), width(width), height(height)
      , window_hints(window_hints)
      , opengl_version_text(opengl_version_text)
    {
    }
  };

  class Window
  {
  public:
    Window(const WindowProps& props);
    ~Window();

    // delete default, copy and move constructors, and assign operators
    Window() = delete;
    Window(Window&) = delete;
    Window(Window&&) = delete;
    Window(const Window&) = delete;
    Window& operator=(Window&) = delete;
    Window& operator=(const Window&) = delete;

    // getters

    std::string GetTitle() const { return s_props.title; }
    unsigned int GetWidth() const { return s_props.width; }
    unsigned int GetHeight() const { return s_props.height; }
    WindowProps GetProps() const { return Props(); } // alias for convention
    WindowProps Props() const { return s_props; }

    // setters

    void SetTitle(std::string const& title) { s_props.title = title; }
    void SetWidth(unsigned int const& width) { s_props.width = width; }
    void SetHeight(unsigned int const& height) { s_props.height = height; }

    // Sets the current window as the active window
    // Clears the screen and runs the layer stack
    void Update();

    // Closes the window
    void Close();

    // Moves the window to the center of the screen
    // Do not call this function unnecessarily
    void CenterWindow();

    // passthrough functions for the layer stack

    void PushLayer(std::shared_ptr<Layer> layer) { m_layerstack.PushLayer(layer); }
    void PushOverlay(std::shared_ptr<Layer> layer) { m_layerstack.PushOverlay(layer); }
    void PopLayer() { m_layerstack.PopLayer(); }
    void PopOverlay() { m_layerstack.PopOverlay(); }

    // Returns the GLFW window pointer.
    // Do not use this unless you know what you are doing.
    // The function is unsafe because it returns a raw pointer.
    GLFWwindow* GetGLFWWindow() const { return m_glfwwindow; }

    // Returns the ImGui context pointer.
    // Do not use this unless you know what you are doing.
    // The function is unsafe because it returns a raw pointer.
    ImGuiContext* GetImGuiContext() const { return m_imguicontext; }

  private:
    LayerStack m_layerstack;

    GLFWwindow* m_glfwwindow{ nullptr };
    ImGuiContext* m_imguicontext{ nullptr };

    WindowProps s_props;
  };

}