#include "splash.h"

#include "States.h"
#include "Layers.h"

using namespace FlexEngine;

namespace FlexEditor
{

  void SplashState::OnEnter()
  {
    window = Application::OpenWindow(
      {
        "Splash",
        // hardcoded splash screen size
        // make sure this is always the same size as the splash screen image
        900, 350,
        {
          { GLFW_CONTEXT_VERSION_MAJOR, 4 },
          { GLFW_CONTEXT_VERSION_MINOR, 6 },
          { GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE },
          { GLFW_DECORATED, false },
          { GLFW_RESIZABLE, false }
        }
      }
    );
    window->PushLayer(std::make_shared<SplashScreenLayer>());
  }

  void SplashState::OnExit()
  {
    Application::CloseWindow(window);
  }

  void SplashState::Update()
  {
    window->Update();

    // todo: replace with async asset loading
    if (Input::GetKeyDown(GLFW_KEY_SPACE))
    {
      ApplicationStateManager::SetState<EditorState>();
    }
  }

}