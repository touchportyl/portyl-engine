#include "editor.h"

#include "States.h"
#include "Layers.h"

using namespace FlexEngine;

namespace FlexEditor
{

  void EditorState::OnEnter()
  {
    window = Application::OpenWindow(
      {
        "FlexEditor",
        1280, 720
      }
    );
    AlignImGuiContext(window);
    window->PushLayer(std::make_shared<EditorLayer>());
  }

  void EditorState::OnExit()
  {
    Application::CloseWindow(window);
    window.reset();
  }

  void EditorState::Update()
  {
    window->Update();
  }

}