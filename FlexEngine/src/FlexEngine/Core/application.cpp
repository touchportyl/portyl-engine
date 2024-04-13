#include "pch.h"

#include "application.h"

#include "StateManager/statemanager.h"
#include "imguiwrapper.h"
#include "input.h"
#include "freequeue.h"

namespace FlexEngine
{
  // static member initialization
  bool Application::m_is_running = true;
  bool Application::m_is_minimized = false;
  std::vector<std::shared_ptr<Window>> Application::m_windows;

  Application::Application()
  {
    FLX_FLOW_BEGINSCOPE();

    // initialize glfw
    FLX_CORE_ASSERT(glfwInit(), "Failed to initialize GLFW!");
    FreeQueue::Push([]() { glfwMakeContextCurrent(NULL); });
    FreeQueue::Push([]() { glfwTerminate(); });

    // initialize imgui
    //ImGuiWrapper::InitBackend();
  }

  Application::~Application()
  {
    // shutdown imgui
    //ImGuiWrapper::ShutdownBackend();

    FLX_FLOW_ENDSCOPE();
  }

  void Application::Close()
  {
    FLX_FLOW_FUNCTION();

    // close all windows
    // done this way because the windows vector is modified in the loop
    size_t windows_to_close = m_windows.size();
    for (size_t i = 0; i < windows_to_close; i++) CloseWindow(m_windows[0]);

    // make sure windows vector is empty
    m_windows.clear();

    m_is_running = false;
  }

  std::shared_ptr<Window> Application::OpenWindow(const WindowProps& props)
  {
    FLX_FLOW_FUNCTION();

    auto window = std::make_shared<Window>(props);
    m_windows.push_back(window);
    return window;
  }

  void Application::CloseWindow(std::string& window_title)
  {
    for (auto& window : m_windows)
    {
      if (window->GetTitle() == window_title)
      {
        CloseWindow(window);
        break;
      }
    }
  }

  void Application::CloseWindow(std::shared_ptr<Window> window)
  {
    FLX_FLOW_FUNCTION();

    window->Close();
    m_windows.erase(std::remove(m_windows.begin(), m_windows.end(), window), m_windows.end());
  }

  void Application::Run()
  {
    while (m_is_running)
    {
      // quit application
      if (Input::GetKey(GLFW_KEY_LEFT_CONTROL) && Input::GetKeyDown(GLFW_KEY_Q)) Application::Close();

      // run every window
      //for (auto& window : m_windows) window->Run();

      // run the application state
      ApplicationStateManager::Update();

      // poll IO events (keys pressed/released, mouse moved etc.)
      glfwPollEvents();
    }
  }

}