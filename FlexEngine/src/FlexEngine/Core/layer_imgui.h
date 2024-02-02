#pragma once

#include "layer.h"

namespace FlexEngine {

  class ImGuiLayer : public Layer
  {
  public:
    ImGuiLayer();
    ~ImGuiLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void Begin();
    void End();

    uint32_t GetActiveWidgetID() const;
  };

}