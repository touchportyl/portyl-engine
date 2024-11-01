#include "mainstate.h"

#include "layer_code.h"

namespace FlexCode
{

  void MainState::OnEnter()
  {
    window = Application::OpenWindow(
      {
        "FlexCode by Wen Loong",
        1280, 720 + 30, // 30 pixels for the title bar
        {
          FLX_DEFAULT_WINDOW_HINTS,
          { GLFW_DECORATED, false },
          { GLFW_RESIZABLE, false }
        }
      }
    );
    AlignImGuiContext(window);

    window->PushLayer(std::make_shared<CodeLayer>());
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