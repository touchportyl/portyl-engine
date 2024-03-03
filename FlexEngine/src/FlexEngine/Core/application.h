#pragma once

#include "window.h"
#include "layerstack.h"
#include "layer.h"
#include "layer_imgui.h"

int main(int argc, char** argv);

namespace FlexEngine
{
  class Application
  {
  public:
    Application(WindowProps window_props = {});
    virtual ~Application();

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);
    void PopLayer(Layer* layer);
    void PopOverlay(Layer* layer);

    /// <summary>
    /// Closes the application.
    /// <para>Sets the glfwSetWindowShouldClose flag</para>
    /// </summary>
    void Close();

    ImGuiLayer* GetImGuiLayer() { return m_imguilayer; }

    /// <returns>The instance of the application</returns>
    static Application& Get() { return *s_instance; }

    /// <returns>The window</returns>
    Window& GetWindow() { return *m_window; }

    /// <returns>The glfw window</returns>
    GLFWwindow* GetGLFWWindow() { return m_glfwwindow; }

  private:
    void Run();

    bool m_is_running = true;
    bool m_is_minimized = false;

    LayerStack m_layerstack;
    ImGuiLayer* m_imguilayer;

    static Application* s_instance;
    Window* m_window;
    GLFWwindow* m_glfwwindow;

    friend int ::main(int argc, char** argv);
  };

  Application* CreateApplication();
}
