#include "layer_launcher.h"

#include <imgui/imgui.h>

namespace Portyl
{
  LauncherLayer::LauncherLayer()
    : Layer("LauncherLayer")
  {
  }

  void LauncherLayer::OnAttach()
  {
  }

  void LauncherLayer::OnDetach()
  {
  }

  void LauncherLayer::OnUpdate()
  {
  }
  
  void LauncherLayer::OnImGuiRender()
  {
    ImGui::Begin("Layer Launcher");
    ImGui::Text("Layer Launcher");
    ImGui::End();
  }

}