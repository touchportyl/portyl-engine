#include "layer_editor.h"

#include <imgui.h>
#include <imgui_internal.h>

//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#define GLM_ENABLE_EXPERIMENTAL
//#include <glm/gtx/quaternion.hpp>

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

#include "FlexEngine/Loader/shaders.h"

namespace FlexEngine
{
  Shader shader_color;

  EditorLayer::EditorLayer()
    : Layer("Flex Editor")
  {
  }

  void EditorLayer::OnAttach()
  {
    // load shaders
    shader_color.SetBasePath("assets/shaders")
      ->CreateVertexShader("color.vert")
      ->CreateFragmentShader("color.frag")
      ->Link();

    // vertex buffer objects (VBO)
    // indices (IBO)
    // vertex array object (VAO)

  }

  void EditorLayer::OnDetach()
  {
    shader_color.Destroy();
  }

  void EditorLayer::OnUpdate()
  {
  }

  void EditorLayer::OnImGuiRender()
  {
    // https://github.com/ocornut/imgui/blob/docking/imgui_demo.cpp

    // check for viewport and docking
    ImGuiIO& io = ImGui::GetIO();
    if (!(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable && io.ConfigFlags & ImGuiConfigFlags_DockingEnable))
    {
      std::cerr << "ImGui Docking and/or Viewports are not enabled!" << std::endl;
      abort();
    }

    ImGuiID dockspace_id = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    ImGui::DockBuilderRemoveNode(dockspace_id);
    ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);
    
    // split the dockspace into 2 nodes -- DockBuilderSplitNode takes in the following args in the following order
    //   window ID to split, direction, fraction (between 0 and 1), the final two setting let's us choose which id we want (which ever one we DON'T set as NULL, will be returned by the function)
    //                                                              out_id_at_dir is the id of the node in the direction we specified earlier, out_id_at_opposite_dir is in the opposite direction
    auto dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.2f, nullptr, &dockspace_id);
    auto dock_id_down = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.25f, nullptr, &dockspace_id);
    
    // we now dock our windows into the docking node we made above
    ImGui::DockBuilderDockWindow("Down", dock_id_down);
    ImGui::DockBuilderDockWindow("Left", dock_id_left);
    ImGui::DockBuilderFinish(dockspace_id);

    ImGui::Begin("Left");
    ImGui::Text("Hello, left!");
    ImGui::End();

    ImGui::Begin("Down");
    ImGui::Text("Hello, down!");
    ImGui::End();

    //ImGui::ShowDemoWindow();
  }

}