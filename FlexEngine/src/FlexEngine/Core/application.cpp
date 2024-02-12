#include "pch.h"

#include "application.h"
#include "input.h"

namespace FlexEngine
{
  Application* Application::s_instance = nullptr;

  Application::Application()
  {
    FE_FLOW_BEGINSCOPE();

    if (s_instance)
    {
      Log::Fatal("Application already exists!");
    }
    s_instance = this;

    // create layer stack
    m_layerstack = LayerStack();

    // create window
    m_window = new Window();
    if (m_window == nullptr)
    {
      Log::Fatal("Window object not created!");
    }
    m_glfwwindow = m_window->GetGLFWWindow(); // cache glfw window

    // load all OpenGL function pointers (glad)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
      Log::Fatal("Failed to initialize GLAD!");
    }

    //Renderer::Init();

    m_imguilayer = new ImGuiLayer();
    PushOverlay(m_imguilayer);
  }

  Application::~Application()
  {
    //Renderer::Shutdown();

    glfwTerminate();

    delete m_window;

    FE_FLOW_ENDSCOPE();
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

  void Application::PopLayer(Layer* layer)
  {
    m_layerstack.PopLayer(layer);
    layer->OnDetach();
  }

  void Application::PopOverlay(Layer* layer)
  {
    m_layerstack.PopOverlay(layer);
    layer->OnDetach();
  }

  void Application::Close()
  {
    FE_FLOW_FUNCTION();

    glfwSetWindowShouldClose(m_glfwwindow, true);
    m_is_running = false;
  }

  void Application::Run()
  {
    while (m_is_running && !glfwWindowShouldClose(m_glfwwindow))
    {
      // wait events if minimized
      if (m_is_minimized)
      {
        glfwWaitEvents();
        continue;
      }

      // poll IO events (keys pressed/released, mouse moved etc.)
      glfwPollEvents();

      // quit application
      // TODO: replace glfw get key with a proper input system
      //if (glfwGetKey(FlexEngine::Application::Get().GetGLFWWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
      //{
      //  glfwSetWindowShouldClose(FlexEngine::Application::Get().GetGLFWWindow(), true);
      //}

      if (Input::GetKey(GLFW_KEY_LEFT_CONTROL) && Input::GetKeyDown(GLFW_KEY_Q))
      {
        Close();
      }

      // clear screen
      glClearColor(0.f, 0.f, 0.f, 1.f);
      glClear(GL_COLOR_BUFFER_BIT); // clear framebuffer

      // run every layer
      for (Layer* layer : m_layerstack)
      {
        layer->OnUpdate();
      }

      // run renderer
      //Renderer::Render();

      m_imguilayer->Begin();
      for (Layer* layer : m_layerstack)
      {
        layer->OnImGuiRender();
      }
      m_imguilayer->End();

      // swap buffers
      glfwSwapBuffers(m_glfwwindow);
    }
  }

}