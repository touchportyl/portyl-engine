#pragma once

#include <FlexEngine.h>

namespace FlexEngine
{

  class LauncherLayer : public Layer
  {
  public:
    LauncherLayer();
    ~LauncherLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate() override;
    virtual void OnImGuiRender() override;
  };

}