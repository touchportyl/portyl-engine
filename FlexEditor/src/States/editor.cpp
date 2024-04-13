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
    window->PushLayer(std::make_shared<EditorLayer>());
  }

  void EditorState::OnExit()
  {
    Application::CloseWindow(window);
  }

  void EditorState::Update()
  {
    window->Update();
  }

}