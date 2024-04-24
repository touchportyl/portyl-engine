#include "Layers/editorui.h"


// temporary
#include <FlexEngine/flexformatter.h>
//#include <FlexEngine/FlexECS/datastructures.h>
#include "Components/DemoComponents.h"

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

  //Entity selected_entity;

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

    #pragma region Test 1 - Reflection Dumping

    // test 1a: dump to console (default to std::cout)

    //{
    //  Log::Debug("test 1a");
    //  Transform t1{ 1, 2, 3 };
    //  Reflection::TypeDescriptor* type_desc = Reflection::TypeResolver<Transform>::Get();
    //  type_desc->Dump(&t1);
    //}

    // test 1b: dump to console (logged)

    //{
    //  Log::Debug("test 1b");
    //  Transform t1{ 10, 20, 30 };
    //  Reflection::TypeDescriptor* type_desc = Reflection::TypeResolver<Transform>::Get();
    //  std::stringstream ss{};
    //  type_desc->Dump(&t1, ss);
    //  Log::Debug(ss.str());
    //}
    
    // test 1c: dump a vector (default)
    
    //{ 
    //  Log::Debug("test 1c");
    //  std::vector<Transform> list;
    //  Transform t1{ 11, 22, 33 }, t2{ 55, 66, 77 };
    //  list.push_back(t1); list.push_back(t2);
    //  Reflection::TypeDescriptor* type_desc = Reflection::TypeResolver<std::vector<Transform>>::Get();
    //  type_desc->Dump(&list);
    //}

    // test 1d: dump a std::vector of custom structs (logged)

    //{
    //  Log::Debug("test 1d");
    //  std::vector<Transform> list;
    //  Transform t1{ 11, 22, 33 }, t2{ 55, 66, 77 };
    //  list.push_back(t1); list.push_back(t2);
    //  Reflection::TypeDescriptor* type_desc = Reflection::TypeResolver<std::vector<Transform>>::Get();
    //  std::stringstream ss{};
    //  type_desc->Dump(&list, ss);
    //  Log::Debug(ss.str());
    //}

    #pragma endregion

    #pragma region Test 2 - Reflection Serialization

    // test 2a: serialize and log

    //{
    //  Log::Debug("test 2a");
    //  Transform t1{ 7, 8, 9 };
    //  Reflection::TypeDescriptor* type_desc = Reflection::TypeResolver<Transform>::Get();
    //  std::stringstream ss{};
    //  type_desc->Serialize(&t1, ss);
    //  Log::Debug(ss.str());
    //}

    // test 2b: serialize a std::vector of custom structs

    //{
    //  Log::Debug("test 2b");
    //  std::vector<Transform> list;
    //  Transform t1{ 11, 22, 33 }, t2{ 55, 66, 77 };
    //  list.push_back(t1); list.push_back(t2);
    //  Reflection::TypeDescriptor* type_desc = Reflection::TypeResolver<std::vector<Transform>>::Get();
    //  std::stringstream ss{};
    //  type_desc->Serialize(&list, ss);
    //  Log::Debug(ss.str());
    //}

    #pragma endregion

    #pragma region Test 3 - FlexFormatter write to file

    // test 3: writing to .flx file
    
    //{
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

    #pragma endregion

    #pragma region Test 4 - Reflection Deserialization

#if 0
    { // test 4a: basic deserialization
      Log::Debug("test 4a");
      std::string json = R"(
        {
          "type": "Transform",
          "data": [
            {
              "type": "float",
              "data": 11
            },
            {
              "type": "float",
              "data": 22
            },
            {
              "type": "float",
              "data": 33
            }
          ]
        }
      )";
      rapidjson::Document document;
      document.Parse(json.c_str());
    
      Transform t;
    
      Reflection::TypeDescriptor* type_desc = Reflection::TypeResolver<Transform>::Get();
    
      type_desc->Deserialize(&t, document);
    
      std::stringstream ss{};
      type_desc->Serialize(&t, ss);
      Log::Debug(ss.str());
    }
#endif

    // test 4b: deserialization of a std::vector of custom structs

    //{
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

    #pragma endregion
    
    #pragma region Test 5 - FlexFormatter read and deserialize from file

    // test 5: parsing .flx file

    //{
    //  Log::Debug("test 5");
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

    #pragma endregion

    #pragma region Test 6 - Reflection for complex data structures

    // test 6a: shared_ptr serialization and deserialization

#if 0
    {
      Log::Debug("test 6a");
    
      std::shared_ptr<float> value = std::make_shared<float>();
      *value = 3.14f;
    
      Reflection::TypeDescriptor* type_desc = Reflection::TypeResolver<std::shared_ptr<float>>::Get();
      //type_desc->Dump(&value);
    
      std::stringstream ss{};
    
      type_desc->Serialize(&value, ss);
      Log::Debug(ss.str());
    
      std::shared_ptr<float> deserialized_value;
      Document document;
      document.Parse(ss.str().c_str());
      type_desc->Deserialize(&deserialized_value, document);
    
      Log::Debug(std::to_string(*deserialized_value));
    }
#endif

    // test 6b: unordered_map serialization and deserialization

#if 0
    {
      Log::Debug("test 6b");
    
      std::unordered_map<std::string, bool> umap;
      std::unordered_map<std::size_t, std::unordered_map<std::string, bool>> umap2;
    
      umap["a"] = true;
      umap["b"] = false;
      umap["c"] = true;
    
      umap2[1] = umap;
    
      Reflection::TypeDescriptor* type_desc = Reflection::TypeResolver<std::unordered_map<std::size_t, std::unordered_map<std::string, bool>>>::Get();
      //type_desc->Dump(&umap);
    
      std::stringstream ss{};
    
      type_desc->Serialize(&umap2, ss);
      Log::Debug(ss.str());
    
      std::unordered_map<std::size_t, std::unordered_map<std::string, bool>> deserialized_umap;
      Document document;
      document.Parse(ss.str().c_str());
      //DumpJsonNodes(document.GetObject());
      type_desc->Deserialize(&deserialized_umap, document);
      
      for (auto& [key, value] : deserialized_umap)
      {
        Log::Debug(std::to_string(key));
        for (auto& [key2, value2] : value)
        {
          Log::Debug(key2 + " = " + (value2 ? "true" : "false"));
        }
      }
    }
#endif

    // test 6c: vector in vector serialization and deserialization

#if 0
    {
      Log::Debug("test 6c");

      std::vector<std::vector<int>> vec2d;
      std::vector<int> vec1{ 1, 2, 3, 4, 5 };
      std::vector<int> vec2{ 6, 7, 8, 9, 10 };
      vec2d.push_back(vec1);
      vec2d.push_back(vec2);

      Reflection::TypeDescriptor* type_desc = Reflection::TypeResolver<std::vector<std::vector<int>>>::Get();
      //type_desc->Dump(&vec2d);

      std::stringstream ss{};
      type_desc->Serialize(&vec2d, ss);
      Log::Debug(ss.str());
    }
#endif

    #pragma endregion

    #pragma region Test 7 - ECS 1.0

    // test 7a: ECS component serialization
    
    //{
    //  Log::Debug("test 7a");
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

    // test 7b: ECS components
    
    //{
    //  Log::Debug("test 7b");
    //
    //  Entity e1 = ECS::CreateEntity("Entity 1");
    //  Entity e2 = ECS::CreateEntity("En-two-ty");
    //  Entity e3 = ECS::CreateEntity("E3");
    //
    //  Transform::AddComponent(e1);
    //  //Transform::AddComponent(e2);
    //  Transform::AddComponent(e3);
    //
    //  Transform::GetComponent(e1)->SetPosition( Vector3(1.0f, 2.0f, 3.0f) );
    //  //Transform::GetComponent(e2)->SetPosition( Vector3(4.0f, 5.0f, 6.0f) );
    //  Transform::GetComponent(e3)->SetPosition( Vector3(7.0f, 8.0f, 9.0f) );
    //
    //  //for (auto& data : Transform::GetComponent(e1.uuid)->GetPosition())
    //  //{
    //  //  Log::Debug(std::to_string(data));
    //  //}
    //
    //  FLX_ECS_SYSTEM_VIEW_START(view)
    //    FLX_ECS_SYSTEM_VIEW_QUERY(Transform)
    //  FLX_ECS_SYSTEM_VIEW_END(view);
    //
    //  for (auto& entity : view)
    //  {
    //    Log::Debug(entity.ToString());
    //  }
    //
    //  ECS::Dump();
    //}

    #pragma endregion

    #pragma region Test 8 - FlexECS

    // test 8a: basic ECS commands

    //{
    //  Log::Debug("test 8a");
    //
    //  Log::Debug("Create scene");
    //  auto scene = FlexECS::Scene::CreateScene();
    //  FlexECS::Scene::SetActiveScene(scene); // done automatically when creating a scene
    //
    //  // component
    //  // Moved to the IsAlive.h file
    //  //struct Amount { int value; };
    //  //struct Health { float value; };
    //
    //
    //  Log::Debug("Create entity");
    //  FlexECS::Entity entity1 = FlexECS::Scene::CreateEntity("Entity 1");
    //  Log::Debug("Created entity1");
    //  FlexECS::Entity entity2 = FlexECS::Scene::CreateEntity("Entity 2");
    //  Log::Debug("Created entity2");
    //
    //
    //  Log::Debug("Add component");
    //  entity1.AddComponent<Amount>({ 35 });
    //  Log::Debug("Added Amount component to entity1 with the value: 35");
    //  entity2.AddComponent<Health>({ 0.7f });
    //  Log::Debug("Added Health component to entity2 with the value: 0.7f");
    //
    //
    //  Log::Debug("Has component");
    //  Log::Debug("entity1 " + std::string(entity1.HasComponent<Amount>() ? "has" : "does not have") + " the Amount component");
    //  Log::Debug("entity2 " + std::string(entity2.HasComponent<Amount>() ? "has" : "does not have") + " the Amount component");
    //  Log::Debug("entity1 " + std::string(entity1.HasComponent<Health>() ? "has" : "does not have") + " the Health component");
    //  Log::Debug("entity2 " + std::string(entity2.HasComponent<Health>() ? "has" : "does not have") + " the Health component");
    //
    //
    //  Log::Debug("Get component");
    //  Log::Debug("entity1 Amount:" + std::to_string(entity1.GetComponent<Amount>()->value));
    //
    //
    //  Log::Debug("TryGet component");
    //  std::shared_ptr<Amount> amount2;
    //  if (entity2.TryGetComponent<Amount>(amount2)) Log::Debug("entity2 Amount:" + std::to_string(amount2->value));
    //  else Log::Warning("Amount component not found");
    //
    //  std::shared_ptr<Health> health2;
    //  if (entity2.TryGetComponent<Health>(health2)) Log::Debug("entity2 Health:" + std::to_string(health2->value));
    //  else Log::Warning("Health component not found");
    //
    //
    //  Log::Debug("Remove component");
    //  Log::Debug("entity1 " + std::string(entity1.HasComponent<Amount>() ? "has" : "does not have") + " the Amount component");
    //  entity1.RemoveComponent<Amount>();
    //  Log::Debug("entity1 " + std::string(entity1.HasComponent<Amount>() ? "has" : "does not have") + " the Amount component");
    //
    //
    //  // dump the entire ecs data structure to the console
    //  FlexECS::Scene::GetActiveScene()->Dump();
    //}
    
    // test 8b: TypeDescriptor uniqueness
    
    //{
    //  Log::Debug("test 8b");
    //  // this test is for replacing ComponentID with TypeDescriptor
    //
    //  std::vector<Reflection::TypeDescriptor*> type_descs1 = {
    //    Reflection::TypeResolver<Amount>::Get(),
    //    Reflection::TypeResolver<Health>::Get(),
    //    Reflection::TypeResolver<IsAlive>::Get()
    //  };
    //  std::vector<Reflection::TypeDescriptor*> type_descs2 = {
    //    Reflection::TypeResolver<IsAlive>::Get(),
    //    Reflection::TypeResolver<Health>::Get(),
    //    Reflection::TypeResolver<Amount>::Get()
    //  };
    //
    //  std::sort(type_descs1.begin(), type_descs1.end());
    //  std::sort(type_descs2.begin(), type_descs2.end());
    //
    //  FLX_ASSERT(type_descs1 != type_descs2, "TypeDescriptor vectors should be the same");
    //}

    #pragma endregion

    #pragma region Test 9 - FlexECS Reflection

    // test 9a: basic ECS commands

#if 0
    {
      Log::Debug("test 9a");
    
      auto scene = FlexECS::Scene::CreateScene();
      
      auto entity1 = FlexECS::Scene::CreateEntity("Player");
      entity1.AddComponent<Vector2>({ 1.0f, 2.0f });
      entity1.AddComponent<IsAlive>({ true });
      entity1.AddComponent<Health>({ 0.9f });
      
      auto entity2 = FlexECS::Scene::CreateEntity("Item");
      entity2.AddComponent<Vector2>({ 35.0f, 42.0f });
      entity2.AddComponent<Amount>({ 3 });
      
      Reflection::TypeDescriptor* type_desc = Reflection::TypeResolver<FlexECS::Scene>::Get();
      //type_desc->Dump(scene.get());
      
      std::stringstream ss{};
      type_desc->Serialize(scene.get(), ss);
      Log::Debug(ss.str());

      // deserialize
      FlexECS::Scene deserialized_scene;
      Document document;
      document.Parse(ss.str().c_str());
      type_desc->Deserialize(&deserialized_scene, document);

      // relink entity archetype pointers
      deserialized_scene.Internal_RelinkEntityArchetypePointers();

      // dump the scene
      deserialized_scene.Dump();
    }
#endif

    // test 9b: Saving/loading scene file to .flxscene

#if 1
    {
      Log::Debug("test 9b");

      // set up scene
      auto scene = FlexECS::Scene::CreateScene();

      auto entity1 = FlexECS::Scene::CreateEntity("Player");
      entity1.AddComponent<Vector2>({ 1.0f, 2.0f });
      entity1.AddComponent<IsAlive>({ true });
      entity1.AddComponent<Health>({ 0.9f });

      auto entity2 = FlexECS::Scene::CreateEntity("Item");
      entity2.AddComponent<Vector2>({ 35.0f, 42.0f });
      entity2.AddComponent<Amount>({ 3 });

      // dump
      scene->DumpEntityIndex();

      // save to file
      auto& save_file = FlexEngine::File::Open( Path::current_path("test9b.flxscene") );
      scene->Save(save_file);

      // read from file
      auto loaded_scene = FlexECS::Scene::Load(save_file);

      // dump
      loaded_scene.DumpEntityIndex();
    }
#endif

    #pragma endregion


    #pragma region Reflection Tests

    // Testing reflection
    
#if 0
    {
      Log::Debug("Reflection Tests");
    
      // data
      Vector2 a{ 1.0f, 2.0f };
      Vector2 b{ 3.0f, 4.0f };
      Vector2 c{ 5.0f, 6.0f };
      Vector2 d{ 7.0f, 8.0f };
    
      // component
    
    
      std::vector<Vector2> component_vec2_a;
      std::vector<Vector2> component_vec2_b;
    
      // archetype
      std::vector<std::vector<Vector2>> component_vec2;
    
      // component creation
      component_vec2_a.push_back({ 1.0f, 2.0f });
      component_vec2_a.push_back({ 3.0f, 4.0f });
      component_vec2_a.push_back({ 5.0f, 6.0f });
      component_vec2.push_back(component_vec2_a);
    
      component_vec2_b.push_back({ 10.0f, 20.0f });
      component_vec2_b.push_back({ 30.0f, 40.0f });
      component_vec2_b.push_back({ 50.0f, 60.0f });
      component_vec2.push_back(component_vec2_b);
    
      // reflection
      Reflection::TypeDescriptor_StdVector* archetype_desc = (Reflection::TypeDescriptor_StdVector*)Reflection::TypeResolver<std::vector<std::vector<Vector2>>>::Get();
      Reflection::TypeDescriptor_StdVector* type_desc = (Reflection::TypeDescriptor_StdVector*)Reflection::TypeResolver<std::vector<Vector2>>::Get();
    
      // system
      // uses reflection to access the data
      // Important thing to note:
      // - The system only takes in the archetype vector
      // - Does it need to know the type of the component?
      for (auto& component : component_vec2)
      {
        for (int i = 0; i < type_desc->get_size(&component); ++i)
        {
          Log::Debug(((Vector2*)type_desc->get_item(&component, i))->ToString());
        }
      };
    }
#endif

    #pragma endregion


    #pragma region Logging Tests

#if 0
    {
      Log::Debug("Debug message");
      Log::Flow("Flow message");
      Log::Info("Info message");
      Log::Warning("Warning message");
      Log::Error("Error message");
      Log::Fatal("Fatal message");
    }
#endif

    #pragma endregion


    #pragma region Vector Swizzle Test

#if 0
    {
      Vector2 vec2{ 1.0f, 2.0f };
      Log::Debug(vec2.Swizzle("yx").ToString()); // Expected log: (2, 1), vec2 is still (1, 2)
      Vector2::Swizzle(vec2, "11");
      Log::Debug(vec2.ToString()); // Expected log: (2, 2)
    }
#endif

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

    #pragma region Entity Testing Panel

    // this panel will be used to test the ECS system
    // it allows the user to create entities and select them for the properties panel
    ImGui::Begin("Entity Testing");
    
    //// create a new entity
    //if (ImGui::Button("Create Entity"))
    //{
    //  ImGui::OpenPopup("Create Entity");
    //}
    //
    //// popup to set the entity name
    //if (ImGui::BeginPopupModal("Create Entity"))
    //{
    //  static char name[128] = "New Entity";
    //  ImGui::InputText("Name", name, sizeof(name));
    //
    //  if (ImGui::Button("Create"))
    //  {
    //    selected_entity = ECS::CreateEntity(name);
    //    ImGui::CloseCurrentPopup();
    //  }
    //
    //  ImGui::EndPopup();
    //}
    //
    //ImGui::SeparatorText("Entities");
    //
    //// get all entities
    //FLX_ECS_SYSTEM_VIEW_START(view_all_entities)
    //FLX_ECS_SYSTEM_VIEW_END(view_all_entities);
    //
    //// list all entities with a button to select them
    //for (auto& [uuid, name] : view_all_entities)
    //{
    //  std::string buffer;
    //
    //  buffer = "Select " + name;
    //  if (ImGui::Button(buffer.c_str()))
    //  {
    //    selected_entity = { name, uuid };
    //  }
    //
    //  ImGui::SameLine();
    //
    //  buffer = "Destroy " + name;
    //  if (ImGui::Button(buffer.c_str()))
    //  {
    //    ECS::DestroyEntity(uuid);
    //  }
    //}
    
    ImGui::End();

    #pragma endregion


    #pragma region Properties Panel

    ImGui::Begin("Properties");
    
    //// display the selected entity
    //if (selected_entity)
    //{
    //  ImGui::Text(selected_entity.name.c_str());
    //}
    //else
    //{
    //  ImGui::Text("No entity selected");
    //}
    //
    //
    //// loop through every single registered component
    //// if the entity is found, display the component
    //if (Transform::GetComponent(selected_entity))
    //{
    //  // display the component
    //  // each data field is automatically generated based on the type
    //  if (ImGui::CollapsingHeader("Transform"))
    //  {
    //    // TODO: cache this so we don't have to call GetComponent() every frame
    //    Vector2 vec2 = Transform::GetComponent(selected_entity)->GetPosition();
    //    ImGui::DragFloat2("Position", vec2.begin(), 0.1f, -std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    //
    //    if (ImGui::IsItemEdited())
    //    {
    //      // update the transform component
    //      Transform::GetComponent(selected_entity)->SetPosition(vec2);
    //    }
    //  }
    //}
    //
    //// loop through every single registered component
    ////for (auto& bucket : ECS::Internal_GetComponentBuckets())
    ////{
    ////  // if the entity is found, display the component
    ////  if (bucket.second->find(selected_entity) != bucket.second->end())
    ////  {
    ////    // serialize the data to json
    ////
    ////    // display field depending on json type
    ////  }
    ////}
    //
    //
    //if (ImGui::Button("Dump"))
    //{
    //  ECS::Dump();
    //}
    
    ImGui::End();

    #pragma endregion


    #pragma region Old Code

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

    #pragma endregion
  }

}
