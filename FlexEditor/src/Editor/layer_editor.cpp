#include "layer_editor.h"

#include <filesystem>

#include <imgui.h>
#include <imgui_internal.h>

//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#define GLM_ENABLE_EXPERIMENTAL
//#include <glm/gtx/quaternion.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "FlexEngine/Loader/shaders.h"

#pragma region copium and pasta

bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
  // Load from file
  int image_width = 0;
  int image_height = 0;
  unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
  if (image_data == NULL)
    return false;

  // Create a OpenGL texture identifier
  GLuint image_texture;
  glGenTextures(1, &image_texture);
  glBindTexture(GL_TEXTURE_2D, image_texture);

  // Setup filtering parameters for display
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

  // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
  stbi_image_free(image_data);

  *out_texture = image_texture;
  *out_width = image_width;
  *out_height = image_height;

  return true;
}

#pragma endregion

// temporary struct
struct Image
{
  int width, height;
  GLuint texture;
};

namespace FlexEngine
{
  Shader shader_color;

  Image test_image;

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

    // load image
    bool ret = LoadTextureFromFile("assets/images/Body_SG1_baseColor.jpeg", &test_image.texture, &test_image.width, &test_image.height);
    assert(ret);

    // todo: add opengl rendering
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

    // setup dockspace
    ImGuiID dockspace_id = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    if (glfwGetKey(Application::Get().GetGLFWWindow(), GLFW_KEY_R))
    {
      // setup dock node
      ImGui::DockBuilderRemoveNode(dockspace_id);
      ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
      ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);

      // split the dockspace into 2 nodes -- DockBuilderSplitNode takes in the following args in the following order
      //   window ID to split, direction, fraction (between 0 and 1), the final two setting let's us choose which id we want (which ever one we DON'T set as NULL, will be returned by the function)
      //                                                              out_id_at_dir is the id of the node in the direction we specified earlier, out_id_at_opposite_dir is in the opposite direction
      auto dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.35f, nullptr, &dockspace_id);
      auto dock_id_down = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.25f, nullptr, &dockspace_id);

      // we now dock our windows into the docking node we made above
      ImGui::DockBuilderDockWindow("Content Browser", dock_id_left);
      ImGui::DockBuilderDockWindow("Down", dock_id_down);
      ImGui::DockBuilderFinish(dockspace_id);
    }

    #pragma region Main Menu Bar

    //ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 12.0f));
    //ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(18.0f, 12.0f));

    if (ImGui::BeginMainMenuBar())
    {

      if (ImGui::BeginMenu("File"))
      {
        if (ImGui::MenuItem("New", "Ctrl+N")) {}
        if (ImGui::MenuItem("Open", "Ctrl+O")) {}
        if (ImGui::MenuItem("Save", "Ctrl+S")) {}
        if (ImGui::MenuItem("Save As", "Ctrl+Shift+S")) {}
        if (ImGui::MenuItem("Exit", "Ctrl+Q")) { Application::Get().Close(); }
        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("Edit"))
      {
        if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
        if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}
        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("View"))
      {
        if (ImGui::MenuItem("Viewport", "Ctrl+1")) {}
        if (ImGui::MenuItem("Content Browser", "Ctrl+2")) {}
        if (ImGui::MenuItem("Down", "Ctrl+3")) {}
        ImGui::EndMenu();
      }

      ImGui::EndMainMenuBar();
    }

    //ImGui::PopStyleVar(2);

    #pragma endregion


    // create imgui windows
    #pragma region Content Browser

    ImGui::Begin("Content Browser");

    static std::filesystem::path base_path = std::filesystem::current_path();

    if (ImGui::Button("Reload"))
    {
      // pull filesystem from path
      base_path = std::filesystem::current_path();
    }

    ImGui::Separator();

    // display the current path
    if (base_path.has_parent_path())
    {
      if (ImGui::Button("../"))
      {
        base_path = base_path.parent_path();
      }
    }

    // iterate through the current path and return a list of files
    for (const auto& entry : std::filesystem::directory_iterator(base_path))
    {
      // if the path is a directory, display it as a button
      if (entry.is_directory())
      {
        if (ImGui::Button(entry.path().filename().generic_string().c_str()))
        {
          base_path = entry.path();
        }
      }
      else
      {
        ImGui::Text(entry.path().filename().generic_string().c_str());
      }
    }

    ImGui::End();

    #pragma endregion

    ImGui::Begin("Down");
    ImGui::Text("Hello, down!");
    ImGui::End();

    ImGui::Begin("Viewport");
    ImGui::Text("pointer = %p", test_image.texture);
    ImGui::Text("size = %d x %d", test_image.width, test_image.height);
    ImGui::Image((void*)(intptr_t)test_image.texture, ImVec2(static_cast<float>(test_image.width), static_cast<float>(test_image.height)));
    ImGui::End();

    ImGui::ShowDemoWindow();
  }

}