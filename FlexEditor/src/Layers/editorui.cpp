#include "Layers/editorui.h"

#include "flexformatter.h"

#include <imgui.h>
#include <imgui_internal.h>

// temporary
#include "Component/Transform2D.h"

void DumpJsonNodes(const rapidjson::Value& value, int depth = 0)
{
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

    //{ // test 1a: dump to console (default to std::cout)
    //  Log::Debug("test 1a");
    //  Transform t1{ 1, 2, 3 };
    //  Reflection::TypeDescriptor* type_desc = Reflection::TypeResolver<Transform>::Get();
    //  type_desc->Dump(&t1);
    //}

    //{ // test 1b: dump to console (logged)
    //  Log::Debug("test 1b");
    //  Transform t1{ 10, 20, 30 };
    //  Reflection::TypeDescriptor* type_desc = Reflection::TypeResolver<Transform>::Get();
    //  std::stringstream ss{};
    //  type_desc->Dump(&t1, ss);
    //  Log::Debug(ss.str());
    //}

    //{ // test 1c: dump a vector (default)
    //  Log::Debug("test 1c");
    //  std::vector<Transform> list;
    //  Transform t1{ 11, 22, 33 }, t2{ 55, 66, 77 };
    //  list.push_back(t1); list.push_back(t2);
    //  Reflection::TypeDescriptor* type_desc = Reflection::TypeResolver<std::vector<Transform>>::Get();
    //  type_desc->Dump(&list);
    //}

    //{ // test 2a: serialize and log
    //  Log::Debug("test 2a");
    //  Transform t1{ 7, 8, 9 };
    //  Reflection::TypeDescriptor* type_desc = Reflection::TypeResolver<Transform>::Get();
    //  std::stringstream ss{};
    //  type_desc->Serialize(&t1, ss);
    //  Log::Debug(ss.str());
    //}

    //{ // test 2b: dump a std::vector of custom structs
    //  Log::Debug("test 2b");
    //  std::vector<Transform> list;
    //  Transform t1{ 11, 22, 33 }, t2{ 55, 66, 77 };
    //  list.push_back(t1); list.push_back(t2);
    //  Reflection::TypeDescriptor* type_desc = Reflection::TypeResolver<std::vector<Transform>>::Get();
    //  std::stringstream ss{};
    //  type_desc->Dump(&list, ss);
    //  Log::Debug(ss.str());
    //}

    //{ // test 2c: serialize a std::vector of custom structs
    //  Log::Debug("test 2c");
    //  std::vector<Transform> list;
    //  Transform t1{ 11, 22, 33 }, t2{ 55, 66, 77 };
    //  list.push_back(t1); list.push_back(t2);
    //  Reflection::TypeDescriptor* type_desc = Reflection::TypeResolver<std::vector<Transform>>::Get();
    //  std::stringstream ss{};
    //  type_desc->Serialize(&list, ss);
    //  Log::Debug(ss.str());
    //}

    //{ // test 3: writing to .flx file
    //  Log::Debug("test 3");
    //
    //  // scene
    //  std::vector<Transform> list;
    //  Transform t1{ 11, 22, 33 }, t2{ 55, 66, 77 };
    //  list.push_back(t1); list.push_back(t2);
    //  Reflection::TypeDescriptor* type_desc = Reflection::TypeResolver<std::vector<Transform>>::Get();
    //  std::stringstream ss{};
    //  type_desc->Serialize(&list, ss);
    //
    //  std::string file_path = "scene.flx";
    //  std::ofstream file(file_path);
    //
    //  if (file.is_open())
    //  {
    //    FlexFormatter::Format(file, ss.str());
    //    Log::Debug("Wrote to file: " + file_path);
    //  }
    //  else
    //  {
    //    Log::Error("Failed to open file: " + file_path);
    //  }
    //
    //}

    //{ // test 4a: basic deserialization
    //  Log::Debug("test 4a");
    //  std::string json = R"(
    //    {
    //      "type": "Transform",
    //      "data": [
    //        {
    //          "type": "float",
    //          "data": 11
    //        },
    //        {
    //          "type": "float",
    //          "data": 22
    //        },
    //        {
    //          "type": "float",
    //          "data": 33
    //        }
    //      ]
    //    }
    //  )";
    //  rapidjson::Document document;
    //  document.Parse(json.c_str());
    //
    //  Transform t;
    //
    //  Reflection::TypeDescriptor* type_desc = Reflection::TypeResolver<Transform>::Get();
    //
    //  type_desc->Deserialize(&t, document);
    //
    //  std::stringstream ss{};
    //  type_desc->Serialize(&t, ss);
    //  Log::Debug(ss.str());
    //}

    //{ // test 4b: deserialization of a std::vector of custom structs
    //  Log::Debug("test 4b");
    //  #pragma region R-string
    //  std::string json = R"(
    //    {
    //      "type": "std::vector<Transform>",
    //      "data": [
    //        {
    //          "type": "Transform",
    //          "data": [
    //            {
    //              "type": "float",
    //              "data": 11
    //            },
    //            {
    //              "type": "float",
    //              "data": 22
    //            },
    //            {
    //              "type": "float",
    //              "data": 33
    //            }
    //          ]
    //        },
    //        {
    //          "type": "Transform",
    //          "data": [
    //            {
    //              "type": "float",
    //              "data": 55
    //            },
    //            {
    //              "type": "float",
    //              "data": 66
    //            },
    //            {
    //              "type": "float",
    //              "data": 77
    //            }
    //          ]
    //        }
    //      ]
    //    }
    //  )";
    //  #pragma endregion
    //  
    //  rapidjson::Document document;
    //  document.Parse(json.c_str());
    //
    //  std::vector<Transform> list;
    //
    //  Reflection::TypeDescriptor* type_desc = Reflection::TypeResolver<std::vector<Transform>>::Get();
    //
    //  type_desc->Deserialize(&list, document);
    //
    //  std::stringstream ss{};
    //  type_desc->Serialize(&list, ss);
    //  Log::Debug(ss.str());
    //}

    //{ // test 4c: parsing .flx file
    //  Log::Debug("test 4c");
    //  std::string file_path = "scene.flx";
    //  std::ifstream file(file_path);
    //
    //  if (file.is_open())
    //  {
    //    // parse the file
    //    Document document = FlexFormatter::Load(file);
    //
    //    // dump the document
    //    //printJsonNodes(document);
    //    //printJsonNodes(document["data"][0]);
    //
    //    // deserialize
    //    std::vector<Transform> list;
    //    
    //    Reflection::TypeDescriptor* type_desc = Reflection::TypeResolver<std::vector<Transform>>::Get();
    //    
    //    type_desc->Deserialize(&list, document["data"][0]);
    //    
    //    std::stringstream ss{};
    //    type_desc->Serialize(&list, ss);
    //    Log::Debug(ss.str());
    //  }
    //  else
    //  {
    //    Log::Error("Failed to open file: " + file_path);
    //  }
    //}

    //{ // test 5a: shared_ptr serialization and deserialization
    //  Log::Debug("test 5a");
    //
    //  std::shared_ptr<float> value = std::make_shared<float>();
    //  *value = 3.14f;
    //
    //  Reflection::TypeDescriptor* type_desc = Reflection::TypeResolver<std::shared_ptr<float>>::Get();
    //  //type_desc->Dump(&value);
    //
    //  std::stringstream ss{};
    //
    //  type_desc->Serialize(&value, ss);
    //  Log::Debug(ss.str());
    //
    //  std::shared_ptr<float> deserialized_value;
    //  Document document;
    //  document.Parse(ss.str().c_str());
    //  type_desc->Deserialize(&deserialized_value, document);
    //
    //  Log::Debug(std::to_string(*deserialized_value));
    //}

    //{ // test 5b: unordered_map serialization and deserialization
    //  Log::Debug("test 5b");
    //
    //  std::unordered_map<std::string, bool> umap;
    //
    //  umap["a"] = true;
    //  umap["b"] = false;
    //  umap["c"] = true;
    //
    //  Reflection::TypeDescriptor* type_desc = Reflection::TypeResolver<std::unordered_map<std::string, bool>>::Get();
    //  //type_desc->Dump(&umap);
    //
    //  std::stringstream ss{};
    //
    //  type_desc->Serialize(&umap, ss);
    //  Log::Debug(ss.str());
    //
    //  std::unordered_map<std::string, bool> deserialized_umap;
    //  Document document;
    //  document.Parse(ss.str().c_str());
    //  //DumpJsonNodes(document.GetObject());
    //  type_desc->Deserialize(&deserialized_umap, document);
    //  
    //  for (auto& [key, value] : deserialized_umap)
    //  {
    //    Log::Debug(key + " = " + (value ? "true" : "false"));
    //  }
    //}

    #pragma region Test 6: ECS

    // test 6a: ECS component serialization
    //{
    //  Log::Debug("test 6a");
    //
    //  Log::Debug("Create new entity");
    //  UUID entity = UUID::Generate();
    //  Log::Debug(std::to_string(entity));
    //
    //  Log::Debug("Add Transform component, set values, then ToString()");
    //  std::shared_ptr<Transform> transform = Transform::AddComponent(entity);
    //
    //  transform->SetX(1.0f);
    //  transform->SetY(2.0f);
    //  transform->SetZ(3.0f);
    //
    //  Log::Debug(transform->ToString());
    //
    //  //Reflection::TypeDescriptor* type_desc = Reflection::TypeResolver<Transform>::Get();
    //
    //  //Log::Debug("TypeDescriptor->Dump()");
    //  //type_desc->Dump(Transform::GetComponent(entity).get());
    //
    //  //Log::Debug("TypeDescriptor->SerializeJson()");
    //  //Document document;
    //  //type_desc->SerializeJson(Transform::GetComponent(entity).get(), document);
    //  //FlexEngine::Reflection::TypeDescriptor::json component;
    //  //component = document.Move();
    //  //DumpJsonNodes(component);
    //
    //  ECS::Dump();
    //}

    // test 6b: ECS components
    {
      Log::Debug("test 6b");

      Entity e1 = ECS::CreateEntity("Entity 1");
      Entity e2 = ECS::CreateEntity("En-two-ty");
      Entity e3 = ECS::CreateEntity("E3");

      Transform::AddComponent(e1);
      //Transform::AddComponent(e2);
      Transform::AddComponent(e3);

      Transform::GetComponent(e1)->SetPosition( Vector3(1.0f, 2.0f, 3.0f) );
      //Transform::GetComponent(e2)->SetPosition( Vector3(4.0f, 5.0f, 6.0f) );
      Transform::GetComponent(e3)->SetPosition( Vector3(7.0f, 8.0f, 9.0f) );

      //for (auto& data : Transform::GetComponent(e1.uuid)->GetPosition())
      //{
      //  Log::Debug(std::to_string(data));
      //}

      FLX_ECS_SYSTEM_VIEW_START(view)
        FLX_ECS_SYSTEM_VIEW_QUERY(Transform)
      FLX_ECS_SYSTEM_VIEW_END(view);

      for (auto& entity : view)
      {
        Log::Debug(entity.ToString());
      }

      ECS::Dump();
    }

    #pragma endregion

    #pragma endregion


    #pragma region Logging Tests

    //{
    //  Log::Debug("Debug message");
    //  Log::Flow("Flow message");
    //  Log::Info("Info message");
    //  Log::Warning("Warning message");
    //  Log::Error("Error message");
    //  Log::Fatal("Fatal message");
    //}

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
