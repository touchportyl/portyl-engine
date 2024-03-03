#pragma once

#include <FlexEngine.h>

namespace FlexEngine
{

  class ViewerLayer : public Layer
  {
  public:
    ViewerLayer();
    ~ViewerLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate() override;
    virtual void OnImGuiRender() override;
  };

}