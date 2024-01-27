#pragma once

#include "window.h"
#include "layerstack.h"
#include "layer.h"
#include "layer_imgui.h"

int main(int argc, char** argv);

namespace Portyl
{
  class Application
  {
  public:
    Application();
    virtual ~Application();

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);

    Window& GetWindow() { return *m_window; }

    void Close();

    ImGuiLayer* GetImGuiLayer() { return m_imguilayer; }

    static Application& Get() { return *s_instance; }

  private:
    void Run();

    bool m_is_running{ true };
    bool m_is_minimized{ false };

    Window* m_window;
    LayerStack m_layerstack;
    ImGuiLayer* m_imguilayer;

    static Application* s_instance;

    friend int ::main(int argc, char** argv);
  };

  Application* CreateApplication();
}
