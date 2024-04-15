#include "Layers/editorui.h"

#include "flexformatter.h"

#include <imgui.h>
#include <imgui_internal.h>

#include "ECS/registry.h"
#include "ECS/transform.h"

void DumpJsonNodes(const rapidjson::Value& value, int depth = 0) {
  if (value.IsObject()) {
    std::cout << std::string(depth, ' ') << "Object:" << std::endl;
    for (auto& member : value.GetObject()) {
      std::cout << std::string(depth + 2, ' ') << "Key: " << member.name.GetString() << std::endl;
      DumpJsonNodes(member.value, depth + 4);
    }
  }
  else if (value.IsArray()) {
    std::cout << std::string(depth, ' ') << "Array:" << std::endl;
    for (rapidjson::SizeType i = 0; i < value.Size(); ++i) {
      std::cout << std::string(depth + 2, ' ') << "Index " << i << ":" << std::endl;
      DumpJsonNodes(value[i], depth + 4);
    }
  }
  else if (value.IsString()) {
    std::cout << std::string(depth, ' ') << "String: " << value.GetString() << std::endl;
  }
  else if (value.IsNumber()) {
    std::cout << std::string(depth, ' ') << "Number: " << value.GetDouble() << std::endl;
  }
  else if (value.IsBool()) {
    std::cout << std::string(depth, ' ') << "Boolean: " << (value.GetBool() ? "true" : "false") << std::endl;
  }
  else if (value.IsNull()) {
    std::cout << std::string(depth, ' ') << "Null" << std::endl;
  }
}

namespace FlexEditor
{

  Asset::Shader shader_color;
  Asset::Texture test_image;

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


    // todo: remove debug code
    #pragma region Regression Testing

    { // test 1: entity + transform component
      ECS ecs;

      // entity creation
      ECS::Entity e1 = ecs.CreateEntity("Test Entity 1");
      Log::Debug(e1.second + " [" + std::to_string(e1.first) + "]");
      ECS::Entity e2 = ecs.CreateEntity("Test Entity 2");

      // component creation
      ecs.AddComponent(e1, Transform());
      //ecs.AddComponent<Transform>(e1, { { 1.0f, 2.0f, 3.0f }, 5.0f, { 7.0f, 8.0f, 9.0f } });
      //auto component = ecs.GetComponent<Transform>(e1).first.lock();
      //Log::Debug(std::to_string(component->position.x));
      //Log::Debug(std::to_string(component->position.y));
      //Log::Debug(std::to_string(component->position.z));

      // find entity by name
      auto e3 = ecs.FindEntityByName("Test Entity 2");
      Log::Debug(e3.second + " [" + std::to_string(e3.first) + "]");

      // find entities by component
      //auto e_list = ecs.FindEntitiesByComponent<Transform>();
      //for (auto& e : e_list)
      //{
      //  Log::Debug(e.second + " [" + std::to_string(e.first) + "]");
      //}
    }

    #pragma endregion


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

  void EditorLayer::Update()
  {
    // https://github.com/ocornut/imgui/blob/docking/imgui_demo.cpp

    // check for viewport and docking
    ImGuiIO& io = ImGui::GetIO();
    FLX_ASSERT(
      io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable && io.ConfigFlags & ImGuiConfigFlags_DockingEnable,
      "ImGui Docking and/or Viewports are not enabled!"
    );

    // setup dockspace
    #pragma warning(suppress: 4189) // unused variable
    ImGuiID dockspace_id = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

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
        if (ImGui::MenuItem("Exit", "Ctrl+Q")) { Application::Close(); }
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
