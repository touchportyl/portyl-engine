#pragma once

#include "layer.h"

namespace FlexEngine {

  /// <summary>
  /// Custom layer for ImGui.
  /// </summary>
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