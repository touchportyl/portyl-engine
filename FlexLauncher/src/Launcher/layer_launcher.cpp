#include "layer_launcher.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/imgui_stdlib.h>

// Yellow is content region min/max
#define IMGUI_SHOW_CONTENT_REGION \
{ \
  ImVec2 vMin = ImGui::GetWindowContentRegionMin(); \
  ImVec2 vMax = ImGui::GetWindowContentRegionMax(); \
 \
  vMin.x += ImGui::GetWindowPos().x; \
  vMin.y += ImGui::GetWindowPos().y; \
  vMax.x += ImGui::GetWindowPos().x; \
  vMax.y += ImGui::GetWindowPos().y; \
 \
  ImGui::GetForegroundDrawList()->AddRect(vMin, vMax, IM_COL32(255, 255, 0, 255)); \
}

using namespace FlexEngine;

namespace FlexLauncher
{

  Asset::Texture flexengine_logo;

  LauncherLayer::LauncherLayer()
    : Layer("Launcher")
  {
  }

  void LauncherLayer::OnAttach()
  {
    FLX_FLOW_FUNCTION();

    flexengine_logo.Load("../FlexEngine/assets/images/flexengine/flexengine_logo_white.png");
  }

  void LauncherLayer::OnDetach()
  {
    FLX_FLOW_FUNCTION();
  }

  void LauncherLayer::OnUpdate()
  {
  }

  void LauncherLayer::OnImGuiRender()
  {
    // check for viewport and docking
    ImGuiIO& io = ImGui::GetIO();
    FLX_ASSERT(
      io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable && io.ConfigFlags & ImGuiConfigFlags_DockingEnable,
      "ImGui Docking and/or Viewports are not enabled!"
    );

    // generic flags
    ImGuiDockNodeFlags dock_node_flags =
      ImGuiDockNodeFlags_NoCloseButton |
      ImGuiDockNodeFlags_NoResize |
      ImGuiDockNodeFlags_NoTabBar |
      ImGuiDockNodeFlags_NoUndocking |
      ImGuiDockNodeFlags_NoWindowMenuButton
    ;

    ImGuiWindowFlags window_flags =
      ImGuiWindowFlags_NoTitleBar |
      ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_NoCollapse |
      ImGuiWindowFlags_NoScrollbar |
      ImGuiWindowFlags_NoScrollWithMouse |
      ImGuiWindowFlags_NoBringToFrontOnFocus |
      ImGuiWindowFlags_NoNavFocus |
      ImGuiWindowFlags_NoBackground |
      ImGuiWindowFlags_NoDecoration
    ;

    // setup dockspace
    ImGuiID dockspace_id = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    // setup dock nodes (only once)
    static bool init = true; if (init) { init = false;
      // create dock node
      ImGui::DockBuilderRemoveNode(dockspace_id);
      ImGuiID dock_id_node = ImGui::DockBuilderAddNode(dockspace_id, dock_node_flags);
      ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);

      // split nodes
      ImGuiID dock_id_up = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Up, 0.1f, nullptr, &dockspace_id);
      ImGuiID dock_id_up_center{};
      ImGuiID dock_id_up_left = ImGui::DockBuilderSplitNode(dock_id_up, ImGuiDir_Left, 0.25f, nullptr, &dock_id_up_center);
      ImGuiID dock_id_up_right = ImGui::DockBuilderSplitNode(dock_id_up_center, ImGuiDir_Right, 0.3f, nullptr, &dock_id_up_center);

      ImGuiID dock_id_down = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.05f, nullptr, &dockspace_id);

      ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.6f, nullptr, &dockspace_id);
      ImGuiID dock_id_left_down{};
      ImGuiID dock_id_left_up = ImGui::DockBuilderSplitNode(dock_id_left, ImGuiDir_Up, 0.4f, nullptr, &dock_id_left_down);

      ImGuiID dock_id_right_down{};
      ImGuiID dock_id_right_up = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Up, 0.2f, nullptr, &dock_id_right_down);

      // we now dock our windows into the docking node we made above
      ImGui::DockBuilderDockWindow("Logo", dock_id_up_left);
      ImGui::DockBuilderDockWindow("Tabs", dock_id_up_center);
      ImGui::DockBuilderDockWindow("Buttons", dock_id_up_right);

      ImGui::DockBuilderDockWindow("Down", dock_id_down);

      ImGui::DockBuilderDockWindow("Banner", dock_id_left_up);
      ImGui::DockBuilderDockWindow("News", dock_id_left_down);

      ImGui::DockBuilderDockWindow("Game Logo", dock_id_right_up);
      ImGui::DockBuilderDockWindow("Login", dock_id_right_down);

      // disable tab bar for custom toolbar
      ImGui::DockBuilderGetNode(dock_id_node)->LocalFlags |= dock_node_flags;

      ImGui::DockBuilderGetNode(dock_id_up)->LocalFlags |= dock_node_flags;
      ImGui::DockBuilderGetNode(dock_id_up_left)->LocalFlags |= dock_node_flags;
      ImGui::DockBuilderGetNode(dock_id_up_center)->LocalFlags |= dock_node_flags;
      ImGui::DockBuilderGetNode(dock_id_up_right)->LocalFlags |= dock_node_flags;

      ImGui::DockBuilderGetNode(dock_id_down)->LocalFlags |= dock_node_flags;

      ImGui::DockBuilderGetNode(dock_id_left)->LocalFlags |= dock_node_flags;
      ImGui::DockBuilderGetNode(dock_id_left_up)->LocalFlags |= dock_node_flags;
      ImGui::DockBuilderGetNode(dock_id_left_down)->LocalFlags |= dock_node_flags;

      ImGui::DockBuilderGetNode(dock_id_right_up)->LocalFlags |= dock_node_flags;
      ImGui::DockBuilderGetNode(dock_id_right_down)->LocalFlags |= dock_node_flags;

      // finish
      ImGui::DockBuilderFinish(dockspace_id);
    }



    #pragma region Up

    ImGui::Begin("Logo", NULL, window_flags);
    float logo_scl = ImGui::GetWindowHeight() * 0.76f; // .76f special scaling factor to fit the logo
    ImGui::Image(flexengine_logo.GetTextureImGui(), ImVec2((logo_scl / 7 * 20), logo_scl)); // 7:20 aspect ratio
    ImGui::End();

    ImGui::Begin("Tabs", NULL, window_flags);
    ImGui::Button("Home");
    ImGui::SameLine();
    ImGui::Button("Downloads");
    ImGui::SameLine();
    ImGui::Button("Patch Notes");
    ImGui::SameLine();
    ImGui::Button("Website");
    ImGui::SameLine();
    ImGui::Button("Store");
    ImGui::End();

    ImGui::Begin("Buttons", NULL, window_flags);
    ImGui::Text("Buttons");
    ImGui::End();

    #pragma endregion



    #pragma region Left

    ImGui::Begin("Banner", NULL, window_flags);
    ImGui::Text("Banner");
    ImGui::End();

    ImGui::Begin("News", NULL, window_flags);
    ImGui::Text("News");
    ImGui::End();

    #pragma endregion



    #pragma region Right

    ImGui::Begin("Game Logo", NULL, window_flags);
    ImGui::Text("Game Logo");
    ImGui::End();

    ImGui::Begin("Login", NULL, window_flags);
    ImGui::Text("Login");
    ImGui::End();

    #pragma endregion



    #pragma region Down

    ImGui::Begin("Down", NULL, window_flags);
    ImGui::Text("Down");
    ImGui::End();

    #pragma endregion

  }

}