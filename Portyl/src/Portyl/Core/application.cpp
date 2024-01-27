#include "pch.h"

#include "application.h"

namespace Portyl
{

  Application* Application::s_instance = nullptr;

  Application::Application()
  {
    assert(!s_instance && "Application already exists!");
    s_instance = this;

    assert(m_window->CreateWindow({}) && "Window not created!");

    //Renderer::Init();

    m_imguilayer = new ImGuiLayer();
    PushOverlay(m_imguilayer);
  }

  Application::~Application()
  {
    //Renderer::Shutdown();
  }

  void Application::PushLayer(Layer* layer)
  {
    m_layerstack.PushLayer(layer);
    layer->OnAttach();
  }

  void Application::PushOverlay(Layer* layer)
  {
    m_layerstack.PushOverlay(layer);
    layer->OnAttach();
  }

  void Application::Close()
  {
    m_is_running = false;
  }

  void Application::Run()
  {
    if (!m_is_minimized)
    {
      while (m_is_running)
      {
        for (Layer* layer : m_layerstack)
        {
          layer->OnUpdate();
        }

        m_imguilayer->Begin();
        for (Layer* layer : m_layerstack)
        {
          layer->OnImGuiRender();
        }
        m_imguilayer->End();
      }
    }
  }

}