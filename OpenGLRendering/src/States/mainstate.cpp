#include "mainstate.h"

#include "States.h"
#include "Layers.h"

namespace OpenGLRendering
{

  void MainState::OnEnter()
  {
    window = Application::OpenWindow(
      {
        "OpenGL Rendering",
        1280, 720,
        {
          { GLFW_CONTEXT_VERSION_MAJOR, 4 },
          { GLFW_CONTEXT_VERSION_MINOR, 6 },
          { GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE },
          { GLFW_DECORATED, false },
          { GLFW_RESIZABLE, false }
        }
      }
    );
    AlignImGuiContext(window);
    window->PushLayer(std::make_shared<MainLayer>());

    window->SetTargetFPS(60);
  }

  void MainState::OnExit()
  {
    Application::CloseWindow(window);
    window.reset();
  }

  void MainState::Update()
  {
    window->Update();
  }

}