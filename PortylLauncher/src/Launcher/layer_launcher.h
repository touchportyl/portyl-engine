#pragma once

#include "Portyl.h"

namespace Portyl
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