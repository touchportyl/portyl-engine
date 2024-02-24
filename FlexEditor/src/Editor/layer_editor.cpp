#include "layer_editor.h"

#include <imgui.h>
#include <imgui_internal.h>

using namespace FlexEngine;

namespace FlexEditor
{

  class Renderer2D
  {
    Renderer2D() = delete;
    ~Renderer2D() = delete;

    static void Init()
    {
      // create vertex buffer
      glGenBuffers(1, &s_Data.QuadVertexBuffer);
      glBindBuffer(GL_ARRAY_BUFFER, s_Data.QuadVertexBuffer);
      glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 4, nullptr, GL_DYNAMIC_DRAW);

      // create vertex array
      glGenVertexArrays(1, &s_Data.QuadVertexArray);
      glBindVertexArray(s_Data.QuadVertexArray);

      // create vertex buffer layout
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const void*)0);
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const void*)(sizeof(float) * 2));

      // create index buffer
      unsigned int indices[6] = { 0, 1, 2, 2, 3, 0 };
      glGenBuffers(1, &s_Data.QuadIndexBuffer);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Data.QuadIndexBuffer);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    }

    static void Shutdown()
    {
      glDeleteBuffers(1, &s_Data.QuadVertexBuffer);
      glDeleteBuffers(1, &s_Data.QuadIndexBuffer);
      glDeleteVertexArrays(1, &s_Data.QuadVertexArray);
    }

    static void BeginScene()
    {
      glBindVertexArray(s_Data.QuadVertexArray);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Data.QuadIndexBuffer);
    }

    static void EndScene()
    {
    }

    static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
      glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 4, nullptr, GL_DYNAMIC_DRAW);

      float vertices[4 * 4] = {
        position.x, position.y, 0.0f, 0.0f,
        position.x + size.x, position.y, 1.0f, 0.0f,
        position.x + size.x, position.y + size.y, 1.0f, 1.0f,
        position.x, position.y + size.y, 0.0f, 1.0f
      };

      glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

  private:
    struct QuadVertex
    {
      glm::vec3 Position;
      glm::vec4 Color;
      glm::vec2 TexCoord;
      float TexIndex;
      float TilingFactor;
    };

    struct Renderer2DData
    {
      static const unsigned int MaxQuads = 10000;
      static const unsigned int MaxVertices = MaxQuads * 4;
      static const unsigned int MaxIndices = MaxQuads * 6;
      static const unsigned int MaxTextureSlots = 32; // TODO: RenderCaps

      unsigned int QuadVertexArray;
      unsigned int QuadVertexBuffer;
      unsigned int QuadIndexBuffer;
      unsigned int WhiteTexture;
      unsigned int TextureSlots[MaxTextureSlots];
      unsigned int TextureSlotIndex = 1; // 0 = white texture
    };

    static Renderer2DData s_Data;
  };

  //struct IndirectDrawCommand {
  //  unsigned int count;         // Draw Count: The number of draw calls to execute.
  //  unsigned int instanceCount; // Instance Count: The number of instances to render.
  //  unsigned int firstIndex;    // First Index: The starting index within an index buffer.
  //  unsigned int baseVertex;    // Base Vertex: A constant offset to add to each index before fetching the vertex data.
  //  unsigned int baseInstance;  // Base Instance: A constant offset to add to each instance ID before fetching per-instance data.
  //};


  Shader shader_color;

  Asset::Texture test_image;

  EditorLayer::EditorLayer()
    : Layer("Flex Editor") {}

  void EditorLayer::OnAttach()
  {
    FLX_FLOW_BEGINSCOPE();

    // load shaders
    shader_color.SetBasePath("assets/shaders")
      ->CreateVertexShader("color.vert")
      ->CreateFragmentShader("color.frag")
      ->Link();

    // load image
    //test_image.Load(); // default texture
    test_image.Load("assets/images/Body_SG1_baseColor.jpeg");

    // todo: add opengl rendering
    // vertex buffer objects (VBO)
    // indices (IBO)
    // vertex array object (VAO)
  }

  void EditorLayer::OnDetach()
  {
    shader_color.Destroy();

    FLX_FLOW_ENDSCOPE();
  }

  void EditorLayer::OnUpdate()
  {
  }

  void EditorLayer::OnImGuiRender()
  {
    // https://github.com/ocornut/imgui/blob/docking/imgui_demo.cpp

    // check for viewport and docking
    ImGuiIO& io = ImGui::GetIO();
    FLX_ASSERT(
      io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable && io.ConfigFlags & ImGuiConfigFlags_DockingEnable,
      "ImGui Docking and/or Viewports are not enabled!"
    );

    // setup dockspace
    //ImGuiID dockspace_id = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    //if (glfwGetKey(Application::Get().GetGLFWWindow(), GLFW_KEY_R))
    //{
    //  // setup dock node
    //  ImGui::DockBuilderRemoveNode(dockspace_id);
    //  ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
    //  ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);
    //
    //  // split the dockspace into 2 nodes -- DockBuilderSplitNode takes in the following args in the following order
    //  //   window ID to split, direction, fraction (between 0 and 1), the final two setting let's us choose which id we want (which ever one we DON'T set as NULL, will be returned by the function)
    //  //                                                              out_id_at_dir is the id of the node in the direction we specified earlier, out_id_at_opposite_dir is in the opposite direction
    //  auto dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.35f, nullptr, &dockspace_id);
    //  auto dock_id_down = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.25f, nullptr, &dockspace_id);
    //
    //  // we now dock our windows into the docking node we made above
    //  ImGui::DockBuilderDockWindow("Content Browser", dock_id_left);
    //  ImGui::DockBuilderDockWindow("Down", dock_id_down);
    //  ImGui::DockBuilderFinish(dockspace_id);
    //}

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
    ImGui::Text("pointer = %p", test_image.GetTexture());
    ImGui::Text("size = %d x %d", test_image.GetWidth(), test_image.GetHeight());
    ImGui::Image(IMGUI_IMAGE(test_image));
    ImGui::End();

    ImGui::ShowDemoWindow();
  }

}