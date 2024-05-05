#include "mainstate.h"

#include "States.h"
#include "Layers.h"

namespace OpenGLRendering
{

  void MainState::OnEnter()
  {
    window = Application::OpenWindow(
      {
        "FlexEngine - OpenGL Rendering",
        1280, 720 + 30, // 30 pixels for the title bar
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

    // load assets only after the window has been created
    AssetManager::Load();

    window->SetIcon(FLX_ASSET_GET(Asset::Texture, R"(\images\flexengine\flexengine_icon_white.png)"));
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