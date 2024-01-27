#include "pch.h"

#include "window.h"

namespace
{

  // glfw: whenever the window size changed (by OS or user resize) this callback function executes
  #pragma warning(suppress : 4100)
  void framebuffer_size_callback(GLFWwindow* window, int width, int height)
  {
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
  }

}

namespace Portyl
{

  WindowProps Window::s_props{};

  bool Window::CreateWindow(WindowProps props) {

    s_props = props;

    m_glfwwindow = glfwCreateWindow(s_props.width, s_props.height, s_props.title.c_str(), nullptr, nullptr);
    if (m_glfwwindow == nullptr) {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return false;
    }

    glfwMakeContextCurrent(m_glfwwindow);
    glfwSetFramebufferSizeCallback(m_glfwwindow, framebuffer_size_callback);
    return true;
  }

}
