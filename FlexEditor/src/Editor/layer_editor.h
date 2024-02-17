#pragma once

#include <FlexEngine.h>

namespace FlexEditor
{

  class EditorLayer : public FlexEngine::Layer
  {
  public:
    EditorLayer();
    ~EditorLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate() override;
    virtual void OnImGuiRender() override;
  };

}