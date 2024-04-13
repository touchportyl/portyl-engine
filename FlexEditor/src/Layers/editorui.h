#pragma once

#include <FlexEngine.h>
using namespace FlexEngine;

namespace FlexEditor
{

  class EditorLayer : public FlexEngine::Layer
  {
  public:
    EditorLayer() : Layer("Flex Editor") {}
    ~EditorLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void Update() override;
  };

}
