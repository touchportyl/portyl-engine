#pragma once

#include "Portyl.h"

namespace Portyl
{

  class LauncherLayer : public Layer
  {
  public:
    LauncherLayer();
    virtual ~LauncherLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void OnUpdate() override;
    virtual void OnImGuiRender() override;
  };

}