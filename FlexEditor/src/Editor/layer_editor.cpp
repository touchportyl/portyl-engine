#include "layer_editor.h"

#include <imgui.h>

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
    ImGui::ShowDemoWindow();

    ImGui::Begin("Test");
    ImGui::Text("This is to test if imgui works.");
    ImGui::End();
    
    ImGui::Begin("Another Window");
    ImGui::Text("This is another window to test docking.");
    ImGui::End();
    
    ImGui::Begin("Third Window");
    ImGui::Text("Third window for the lols.");
    ImGui::End();

  }

}