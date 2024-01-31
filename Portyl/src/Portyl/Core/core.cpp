#include "pch.h"

#include "core.h"

#include "application.h"

namespace Portyl
{
  namespace Core
  {
    void Init()
    {

    }
    void Update()
    {
      // quit application
      // TODO: replace glfw get key with a proper input system
      if (glfwGetKey(Portyl::Application::Get().GetGLFWWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
      {
        glfwSetWindowShouldClose(Portyl::Application::Get().GetGLFWWindow(), true);
      }
    }
    void Shutdown()
    {

    }
  }
}