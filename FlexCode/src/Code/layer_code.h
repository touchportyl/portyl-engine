#pragma once

#include <FlexEngine.h>

namespace FlexCode
{

  class CodeLayer : public FlexEngine::Layer
  {
  public:
    CodeLayer();
    ~CodeLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate() override;
    virtual void OnImGuiRender() override;
  };

}