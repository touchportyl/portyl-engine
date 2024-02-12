#include "pch.h"

#include "window.h"
#include "input.h"

namespace
{

  // glfw: whenever the window size changed (by OS or user resize) this callback function executes
  #pragma warning(suppress : 4100) // unused parameters
  void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
  {
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);

    // update the window properties
    FlexEngine::Window::SetWidth(width);
    FlexEngine::Window::SetHeight(height);
  }

}

namespace FlexEngine
{

  WindowProps Window::s_props{};

  Window::Window(WindowProps const& props)
  {
    FE_FLOW_FUNCTION();

    s_props = props;

    // initialize and configure glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // todo: custom title bar
    // https://www.glfw.org/docs/3.0/window.html#window_hints
    //glfwWindowHint(GLFW_DECORATED, false);

    // create window
    m_glfwwindow = glfwCreateWindow(s_props.width, s_props.height, s_props.title.c_str(), nullptr, nullptr);
    if (m_glfwwindow == nullptr)
    {
      Log::Fatal("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(m_glfwwindow);

    // set callbacks
    glfwSetKeyCallback(m_glfwwindow, Input::KeyCallback);
    glfwSetCursorPosCallback(m_glfwwindow, Input::CursorPositionCallback);
    glfwSetMouseButtonCallback(m_glfwwindow, Input::MouseButtonCallback);
    glfwSetScrollCallback(m_glfwwindow, Input::ScrollCallback);
    //glfwSetWindowSizeCallback(m_glfwwindow, WindowSizeCallback);
    glfwSetFramebufferSizeCallback(m_glfwwindow, FramebufferSizeCallback);
    //glfwSetWindowCloseCallback(m_glfwwindow, WindowCloseCallback);
    //glfwSetWindowFocusCallback(m_glfwwindow, WindowFocusCallback);
    //glfwSetCharCallback(m_glfwwindow, CharCallback);
    //glfwSetDropCallback(m_glfwwindow, DropCallback);
  }

}
