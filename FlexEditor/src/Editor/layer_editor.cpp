#include "layer_editor.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <sstream>

using namespace FlexEngine;

namespace FlexEditor
{

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


    // todo: remove debug code

    //UUID uuid;
    //Log::Debug(uuid);

    { // test 1: 
      Transform t1{1, 2, 3};
      FlexEngine::Reflection::TypeDescriptor* type_desc = FlexEngine::Reflection::TypeResolver<Transform>::Get();
      type_desc->Dump(&t1);
    }

    //{ // test 1: properties in a component
    //  Transform t1{}, t2{};
    //  t1.properties.SetProperty("x", 10.f);
    //  t1.properties.SetProperty("y", 200.f);
    //  t1.properties.SetProperty<float>("z", 400);
    //
    //  t2.properties.SetProperty("z", t1.properties.GetProperty<float>("y"));
    //
    //  std::stringstream ss{};
    //  ss << "\n";
    //  t1.properties.Serialize(ss);
    //  ss << "\n";
    //  t2.properties.Serialize(ss);
    //  Log::Debug(ss.str());
    //}

    //{ // test 2: recursive properties
    //  GameObject go1{};// , go2{};
    //  go1.AddComponent<Transform>();
    //  //go2.AddComponent<Transform>();
    //
    //  Transform* t1 = go1.GetComponent<Transform>();
    //  //Transform* t2 = go2.GetComponent<Transform>();
    //
    //  t1->properties.SetProperty("x", 1.f);
    //  t1->properties.SetProperty("y", 2.f);
    //  t1->properties.SetProperty("z", 3.f);
    //
    //  //t2->properties.SetProperty("x", t1->properties.GetProperty<float>("z"));
    //  //t2->properties.SetProperty("y", t1->properties.GetProperty<float>("y"));
    //  //t2->properties.SetProperty("z", t1->properties.GetProperty<float>("x"));
    //
    //  std::stringstream ss{};
    //  ss << "\n";
    //  go1.properties.Serialize(ss);
    //  //ss << "\n";
    //  //go2.properties.Serialize(ss);
    //  Log::Debug(ss.str());
    //}

    //Scene scene{};

    //GameObject* go1 = scene.AddGameObject("Transform Test");
    //GameObject* go2 = scene.AddGameObject("Transformers 2: Electric Boogaloo");

    //Transform* t1 = go1->AddComponent<Transform>();
    //go2->AddComponent<Transform>();

    //Transform* t2 = go2->GetComponent<Transform>();

    //t1->properties.SetProperty("x", 42.f);
    //t2->properties.SetProperty("x", t1->properties.GetProperty<float>("x"));
    //t1->properties.SetProperty("x", 35.f);



    //ComponentMap* go2_props = go2->properties.GetPropertyReference<ComponentMap>("components");

    // get the transform component by going thru the map
    //auto it = go2_props->find(typeid(Transform));
    //if (it != go2_props->end())
    //{
    //  Transform* t3 = dynamic_cast<Transform*>(it->second.get());
    //  t3->properties.SetProperty("x", 100.f);
    //}


    //std::stringstream ss{};
    //scene.Serialize(ss);
    //Log::Debug(ss.str());


    //Transform transform{};
    //transform.x = 1;
    //Log::Debug(transform.uuid);


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
