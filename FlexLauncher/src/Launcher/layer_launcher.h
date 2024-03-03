#pragma once

#include <FlexEngine.h>

namespace FlexLauncher
{

  class LauncherLayer : public FlexEngine::Layer
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