#pragma once

#include <FlexEngine.h>

namespace FlexEditor
{

  class EditorState : public FlexEngine::State
  {
    std::shared_ptr<FlexEngine::Window> window;

  public:
    EditorState() = default;
    ~EditorState() = default;

    void OnEnter() override;
    void Update() override;
    void OnExit() override;
  };

}