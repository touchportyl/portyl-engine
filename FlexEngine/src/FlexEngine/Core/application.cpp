#include "pch.h"

#include "application.h"
#include "input.h"

namespace FlexEngine
{
  Application* Application::s_instance = nullptr;

  Application::Application(WindowProps window_props)
  {
    FLX_FLOW_BEGINSCOPE();

    // check if application already exists
    if (s_instance) Log::Fatal("Application already exists!"); 
    else s_instance = this;

    // create window
    m_window = new Window(window_props);
    FLX_NULLPTR_ASSERT(m_window, "Window object not created!");
    FreeQueue::Push([&]() { delete m_window; });
    m_glfwwindow = m_window->GetGLFWWindow(); // cache glfw window

    // load all OpenGL function pointers (glad)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
      Log::Fatal("Failed to initialize GLAD!");
    }

    //Renderer::Init();
    //FreeQueue::Push(Renderer::Shutdown);

    m_imguilayer = new ImGuiLayer();
    PushOverlay(m_imguilayer);
  }

  Application::~Application()
  {
    FLX_FLOW_ENDSCOPE();
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
    FLX_FLOW_FUNCTION();

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