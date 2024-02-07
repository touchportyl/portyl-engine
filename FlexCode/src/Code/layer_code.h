#pragma once

#include <FlexEngine.h>

namespace FlexEngine
{

  class CodeLayer : public Layer
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