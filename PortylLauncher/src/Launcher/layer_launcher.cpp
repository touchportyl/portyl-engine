#include "layer_launcher.h"

#include <imgui.h>

namespace Portyl
{
  static bool show_demo_window{ true };
  static bool show_another_window{ false };
  static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  LauncherLayer::LauncherLayer()
    : Layer("ImGui Demo")
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
    ImGui::ShowDemoWindow(&show_demo_window);

    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
    ImGui::Checkbox("Another Window", &show_another_window);

    ImGui::SliderFloat3("float3", (float*)&clear_color, 0.0f, 1.0f);

    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
      ImGui::Text("Hello, world!");

    ImGui::End();

    // 2. Show another simple window.
    if (show_another_window)
    {
      ImGui::Begin("Another Window", &show_another_window);
      ImGui::Text("Hello from another window!");
      if (ImGui::Button("Close Me"))
        show_another_window = false;
      ImGui::End();
    }
  }


}