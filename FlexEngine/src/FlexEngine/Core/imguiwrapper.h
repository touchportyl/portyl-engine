#pragma once

#include "layer.h" // <string>

#include <imgui.h>

#include "window.h"

namespace FlexEngine
{

  namespace ImGuiWrapper
  {

    ImGuiContext* Init(Window* window);
    void Shutdown(ImGuiContext* imgui_context);

    void BeginFrame();
    void EndFrame();

    unsigned int GetActiveWidgetID();

  }

}