#include "mainlayer.h"

#include "Components/Components.h"

namespace OpenGLRendering
{

  void MainLayer::OnAttach()
  {
    FLX_FLOW_BEGINSCOPE();

    // ECS Setup

    FlexECS::Scene::CreateScene();
    main_camera = FlexECS::Scene::CreateEntity("Main Camera");
    point_light = FlexECS::Scene::CreateEntity("Point Light");
    directional_light = FlexECS::Scene::CreateEntity("Directional Light");
    object = FlexECS::Scene::CreateEntity("Object");
    plane = FlexECS::Scene::CreateEntity("Plane");

    main_camera.AddComponent<GlobalPosition>({ { -0.4f, 0.24f, 0.44f } });
    main_camera.AddComponent<Rotation>({ { -12, -35, 0 } });
    main_camera.AddComponent<Camera>({});

    point_light.AddComponent<GlobalPosition>({ { 0, 0.9f, 0.35f } });
    point_light.AddComponent<PointLight>({
      { 0.5f, 0.5f, 0.5f },
      { 1.0f, 1.0f, 1.0f },
      { 1.0f, 1.0f, 1.0f }
    });

    //directional_light.AddComponent<GlobalPosition>({ { 0, 1, 0 } });
    //directional_light.AddComponent<DirectionalLight>({
    //  { 0.5f, 0.5f, 0.5f },
    //  { 1.0f, 1.0f, 1.0f },
    //  { 1.0f, 1.0f, 1.0f }
    //});

    object.AddComponent<GlobalPosition>({ { 0, 0, 0 } });
    object.AddComponent<LocalPosition>({ { 0, 0, 0 } });
    object.AddComponent<Rotation>({ { 0, 0, 0 } });
    object.AddComponent<Scale>({ { 0.001f, 0.001f, 0.001f } });
    object.AddComponent<Transform>({});
    object.AddComponent<Model>({ R"(\models\firetruck\firetruck.fbx)" });
    object.AddComponent<Shader>({ R"(\shaders\renderer)" });

    plane.AddComponent<GlobalPosition>({ { 0, 0, 0 } });
    plane.AddComponent<LocalPosition>({ { 0, 0, 0 } });
    plane.AddComponent<Rotation>({ { 0, 0, 20 } });
    Vector3 plane_scale = { 0.9f, 0.35f, 1 };
    plane_scale *= 0.0f;
    plane.AddComponent<Scale>({ plane_scale });
    plane.AddComponent<Transform>({});
    //plane.AddComponent<Texture>({ R"(\images\flexengine\flexengine_splash.png)" });
    plane.AddComponent<Material>(
      { // Material
        R"(\images\flexengine\flexengine_splash.png)",
        { // std::pair<AssetKey, float>
          R"(\images\flexengine\flexengine_splash.png)",
          32.0f
        }
      }
    );
    plane.AddComponent<Shader>({ R"(\shaders\renderer)" });
    
    // manually build a mesh
    plane.AddComponent<Mesh>({
      { // Mesh
        { // std::vector<Vertex>
          Vertex(Vector3(1.0f,  1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector2(1.0f, 0.0f), Vector3::Zero, Vector3::Zero, Vector3::Zero),
          Vertex(Vector3(1.0f, -1.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector2(1.0f, 1.0f), Vector3::Zero, Vector3::Zero, Vector3::Zero),
          Vertex(Vector3(-1.0f, -1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3::Zero, Vector3::Zero, Vector3::Zero),
          Vertex(Vector3(-1.0f,  1.0f, 0.0f), Vector3(1.0f, 1.0f, 0.0f), Vector2(0.0f, 0.0f), Vector3::Zero, Vector3::Zero, Vector3::Zero)
        },
        { // std::vector<unsigned int>
          0, 1, 3, // first triangle
          1, 2, 3  // second triangle
        }
      }
    });

    // manually create buffers
    plane.GetComponent<Mesh>()->mesh.Internal_CreateBuffers();

    // Renderer Setup

    OpenGLRenderer::EnableDepthTest();
  }

  void MainLayer::OnDetach()
  {
    FLX_FLOW_ENDSCOPE();
  }

  void MainLayer::Update()
  {
    OpenGLRenderer::ClearFrameBuffer();

    #pragma region Title Bar

    #if 1
    {
      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 8.0f));
      ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(16.0f, 8.0f));

      if (ImGui::BeginMainMenuBar())
      {
        // center the logo
        float logo_scl = 25.f;
        float prev_cursor_y = ImGui::GetCursorPosY();
        float cursor_y = prev_cursor_y + (ImGui::GetFrameHeight() - logo_scl) * 0.5f;
        ImGui::SetCursorPosY(cursor_y);

        // display the flexengine logo
        Asset::Texture& flexengine_logo = FLX_ASSET_GET(Asset::Texture, R"(\images\flexengine\flexengine_logo_white.png)");
        ImGui::Image(flexengine_logo.GetTextureImGui(), ImVec2((logo_scl / 7 * 20), logo_scl));

        // reset the cursor position
        ImGui::SetCursorPosY(prev_cursor_y);

        if (ImGui::BeginMenu("File"))
        {
          //if (ImGui::MenuItem("New", "Ctrl+N")) {}
          //if (ImGui::MenuItem("Save", "Ctrl+S")) {}
          //if (ImGui::MenuItem("Reload", "Ctrl+R")) {}
          //if (ImGui::MenuItem("Close", "Ctrl+W")) {}
          if (ImGui::MenuItem("Exit", "Ctrl+Q")) { Application::Close(); }
          ImGui::EndMenu();
        }

        //if (ImGui::BeginMenu("Edit"))
        //{
        //  if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
        //  if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}
        //  ImGui::EndMenu();
        //}

        ImGui::EndMainMenuBar();
      }

      ImGui::PopStyleVar(2);

      // Title bar window drag functionality

      static bool is_dragging_window = false;

      ImGuiIO& io = ImGui::GetIO();
      ImVec2 mouse_pos = io.MousePos;
      
      // The main menu bar height is 30 pixels
      ImGuiViewport* viewport = ImGui::GetMainViewport();
      ImVec2 top_left = viewport->Pos;
      ImVec2 bottom_right = ImVec2(top_left.x + viewport->Size.x, top_left.y + 30.0f);

      // Window dragging state
      if (io.MouseDown[0])
      {
        if (
          io.MouseClicked[0] &&
          mouse_pos.x >= top_left.x && mouse_pos.x <= bottom_right.x &&
          mouse_pos.y >= top_left.y && mouse_pos.y <= bottom_right.y
        )
        {
          is_dragging_window = true;
        }
      }
      else
      {
        is_dragging_window = false;
      }
      
      // Dragging the window
      if (is_dragging_window)
      {
        Window* current_window = Application::GetCurrentWindow();
        if (current_window)
        {
          int pos_x, pos_y;
          current_window->GetWindowPosition(&pos_x, &pos_y);

          // We can't use Input::GetCursorPositionDelta() because it only handles within the glfw window
          // ImGui's IO MousePos is in global screen coordinates
          
          //Vector2 mouse_delta = Input::GetCursorPositionDelta();
          //if (mouse_delta.x != 0.0f || mouse_delta.y != 0.0f)
          //{
          //  current_window->SetWindowPosition(
          //    pos_x + static_cast<int>(mouse_delta.x),
          //    pos_y + static_cast<int>(mouse_delta.y)
          //  );
          //}

          if (io.MouseDelta.x != 0.0f || io.MouseDelta.y != 0.0f)
          {
            current_window->SetWindowPosition(
              pos_x + static_cast<int>(io.MouseDelta.x),
              pos_y + static_cast<int>(io.MouseDelta.y)
            );
          }
        }
      }
    }
    #endif

    #pragma endregion


    #pragma region ImGui Editor System

    #if 1
    {
      // hardcoded editor for testing
      ImGui::Begin("Properties");
      ImGuiTreeNodeFlags tree_node_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick; // open by default

      // camera
      if (ImGui::CollapsingHeader("Main Camera", tree_node_flags))
      {
        auto& global_position = main_camera.GetComponent<GlobalPosition>()->position;
        auto& rotation = main_camera.GetComponent<Rotation>()->rotation;
        auto camera = main_camera.GetComponent<Camera>();

        ImGui::PushID("camera");
        ImGui::DragFloat3("Global Position", global_position.begin(), 0.01f, -1000.0f, 1000.0f, "%.2f");
        if (ImGui::DragFloat3("Rotation", rotation.begin(), 0.1f, -0.1f, 360.1f, "%.1f"))
        {
          // loop around
          for (auto& value : rotation)
          {
            if (value > 360.0f) value -= 360.0f;
            else if (value < 0.0f) value += 360.0f;
          }
        }
        ImGui::SeparatorText("Camera Settings");
        ImGui::Checkbox("Perspective", &camera->perspective);
        ImGui::DragFloat("FOV", &camera->fov, 0.1f, 20.0f, 120.0f, "%.1f");
        ImGui::DragFloat("Near Clip", &camera->near, 0.1f, 0.1f, 100.0f, "%.1f");
        ImGui::DragFloat("Far Clip", &camera->far, 0.1f, 0.1f, 100.0f, "%.1f");
        ImGui::PopID();
      }

      // point light
      if (ImGui::CollapsingHeader("Point Light", tree_node_flags))
      {
        auto& position = point_light.GetComponent<GlobalPosition>()->position;
        auto& ambient = point_light.GetComponent<PointLight>()->ambient;
        auto& diffuse = point_light.GetComponent<PointLight>()->diffuse;
        auto& specular = point_light.GetComponent<PointLight>()->specular;

        ImGui::PushID("point_light");
        ImGui::DragFloat3("Position", position.begin(), 0.01f, -100.0f, 100.0f, "%.2f");
        ImGui::ColorEdit3("Ambient", ambient.begin());
        ImGui::ColorEdit3("Diffuse", diffuse.begin());
        ImGui::ColorEdit3("Specular", specular.begin());
        ImGui::PopID();
      }

      // directional light
      //if (ImGui::CollapsingHeader("Directional Light", tree_node_flags))
      //{
      //  auto& position = directional_light.GetComponent<GlobalPosition>()->position;
      //  auto& ambient = directional_light.GetComponent<DirectionalLight>()->ambient;
      //  auto& diffuse = directional_light.GetComponent<DirectionalLight>()->diffuse;
      //  auto& specular = directional_light.GetComponent<DirectionalLight>()->specular;
      //
      //  ImGui::PushID("directional_light");
      //  ImGui::DragFloat3("Position", position.begin(), 0.01f, -1.0f, 1.0f, "%.2f");
      //  ImGui::ColorEdit3("Ambient", ambient.begin());
      //  ImGui::ColorEdit3("Diffuse", diffuse.begin());
      //  ImGui::ColorEdit3("Specular", specular.begin());
      //  ImGui::PopID();
      //}

      ImGui::Separator();
      
      // entities
      for (auto& entity : FlexECS::Scene::GetActiveScene()->View<LocalPosition, GlobalPosition, Rotation, Scale>())
      {
        auto entity_name = entity.GetComponent<std::string>();
        auto& local_position = entity.GetComponent<LocalPosition>()->position;
        auto& global_position = entity.GetComponent<GlobalPosition>()->position;
        auto& rotation = entity.GetComponent<Rotation>()->rotation;
        auto& scale = entity.GetComponent<Scale>()->scale;
      
        if (ImGui::CollapsingHeader(entity_name->c_str(), tree_node_flags))
        {
          ImGui::PushID(entity_name->c_str());
          ImGui::DragFloat3("Global Position", global_position.begin(), 0.01f, -1000.0f, 1000.0f, "%.2f");
          ImGui::DragFloat3("Local Position", local_position.begin(), 0.01f, -1000.0f, 1000.0f, "%.2f");
          if (ImGui::DragFloat3("Rotation", rotation.begin(), 0.1f, -0.1f, 360.1f, "%.1f"))
          {
            // loop around
            for (auto& value : rotation)
            {
              if (value > 360.0f) value -= 360.0f;
              else if (value < 0.0f) value += 360.0f;
            }
          }
          ImGui::DragFloat3("Scale", scale.begin(), 0.01f, 0.1f, 1000.0f, "%.2f");
          ImGui::PopID();
        }
      }

      ImGui::End();
    }
    #endif

    #pragma endregion


    #pragma region Model Showcase System

    #if 1
    {
      // Rotate all entities in the scene (except cameras)
      for (auto& entity : FlexECS::Scene::GetActiveScene()->View<GlobalPosition, Rotation>())
      {
        if (entity.HasComponent<Camera>()) continue;
      
        auto& global_position = entity.GetComponent<GlobalPosition>()->position;
        auto& rotation = entity.GetComponent<Rotation>()->rotation;
      
        // rotate the entity
        rotation.y += 0.4f;
        if (rotation.y > 360.0f) rotation.y -= 360.0f;

        // move the entity
        Vector3 initial_position = { 0, 0, 0 };
        Vector3 final_position = { 0, 0.035f, 0 };
        // lerp bob up and down using sine wave
        static float accumulator = 0.0f;
        accumulator += Application::GetCurrentWindow()->GetDeltaTime();
        if (accumulator > 2.0f * PIf) accumulator -= 2.0f * PIf;
        float t = (sinf(accumulator) + 1.0f) / 2.0f;
        global_position = Lerp(initial_position, final_position, t);
      }
    }
    #endif

    #pragma endregion


    #pragma region Transform Calculation System

    #if 1
    {
      // Updates the transform component
      for (auto& entity : FlexECS::Scene::GetActiveScene()->View<LocalPosition, GlobalPosition, Rotation, Scale, Transform>())
      {
        auto& local_position = entity.GetComponent<LocalPosition>()->position;
        auto& global_position = entity.GetComponent<GlobalPosition>()->position;
        auto& rotation = entity.GetComponent<Rotation>()->rotation;
        auto& scale = entity.GetComponent<Scale>()->scale;
        auto& transform = entity.GetComponent<Transform>()->transform;

        // calculate the transform

        Matrix4x4 local_translation_matrix = Matrix4x4::Translate(Matrix4x4::Identity, local_position);
        Matrix4x4 global_translation_matrix = Matrix4x4::Translate(Matrix4x4::Identity, global_position);
        Matrix4x4 rotation_matrix = Quaternion::FromEulerAnglesDeg(rotation).ToRotationMatrix();
        Matrix4x4 scale_matrix = Matrix4x4::Scale(Matrix4x4::Identity, scale);
        
        // right to left
        // local transforms apply first before placing it in the world
        transform = global_translation_matrix * rotation_matrix * scale_matrix * local_translation_matrix;
      }
    }
    #endif

    #pragma endregion


    #pragma region Camera System

    #if 1
    {
      // Updates the camera component
      for (auto& entity : FlexECS::Scene::GetActiveScene()->View<GlobalPosition, Rotation, Camera>())
      {
        auto& global_position = entity.GetComponent<GlobalPosition>()->position;
        auto& rotation = entity.GetComponent<Rotation>()->rotation;
        auto camera = entity.GetComponent<Camera>();

        // update the camera

        // calculate the front vector
        camera->front = Vector3::Zero;
        camera->front.x = cos(radians(rotation.y)) * cos(radians(rotation.x));
        camera->front.y = sin(radians(rotation.x));
        camera->front.z = sin(radians(rotation.y)) * cos(radians(rotation.x));
        camera->front.Normalize();

        // calculate the right vector
        camera->right = Vector3::Normalize(Cross(camera->front, camera->world_up));

        // calculate the up vector
        camera->up = Vector3::Normalize(Cross(camera->right, camera->front));

        // calculate the view matrix
        camera->view = Matrix4x4::LookAt(global_position, global_position + camera->front, camera->up);

        // calculate the projection matrix
        Window* current_window = Application::GetCurrentWindow();
        if (camera->perspective)
        {
          camera->projection = Matrix4x4::Perspective(
            radians(camera->fov),
            (static_cast<float>(current_window->GetWidth()) / static_cast<float>(current_window->GetHeight())),
            camera->near, camera->far
          );
        }
        else
        {
          // calculate the aspect ratio
          float aspect_ratio = static_cast<float>(current_window->GetWidth()) / static_cast<float>(current_window->GetHeight());

          // calculate the bounds
          float fov_vertical = radians(camera->fov);
          float bounds_vertical = tan(fov_vertical / 2.0f) * camera->near;
          float bounds_horizontal = bounds_vertical * aspect_ratio;

          camera->projection = Matrix4x4::Orthographic(
            -bounds_horizontal, bounds_horizontal,
            -bounds_vertical, bounds_vertical,
            camera->near, camera->far
          );
        }
      }
    }
    #endif

    #pragma endregion


    #pragma region 2D Renderer System (Mesh + Material)
    
    #if 0
    {
      // cache camera
      auto camera = main_camera.GetComponent<Camera>();
      // cache directional light
      auto dir_light_pos = directional_light.GetComponent<GlobalPosition>();
      auto dir_light = directional_light.GetComponent<DirectionalLight>();

      // Render all entities
      for (auto& entity : FlexECS::Scene::GetActiveScene()->View<Transform, Mesh, Material, Shader>())
      {
        auto& transform = entity.GetComponent<Transform>()->transform;
        auto& mesh = entity.GetComponent<Mesh>()->mesh;
        auto material = entity.GetComponent<Material>();
        auto& shader = entity.GetComponent<Shader>()->shader;

        // render mesh
        mesh.VAO->Bind();
        mesh.VBO->Bind();
        mesh.IBO->Bind();

        // shader setup
        auto& shader_asset = FLX_ASSET_GET(Asset::Shader, shader);
        shader_asset.Use();

        // TODO: accumulate transformations
        // local, global, parent, etc.
        shader_asset.SetUniform_mat4("u_model", transform);

        shader_asset.SetUniform_mat4("u_view", camera->view);
        shader_asset.SetUniform_mat4("u_projection", camera->projection);

        shader_asset.SetUniform_vec3("u_light_position", dir_light_pos->position);
        shader_asset.SetUniform_vec3("u_light_ambient", dir_light->ambient);
        shader_asset.SetUniform_vec3("u_light_diffuse", dir_light->diffuse);
        shader_asset.SetUniform_vec3("u_light_specular", dir_light->specular);

        // setup material
        auto& diffuse = FLX_ASSET_GET(Asset::Texture, material->diffuse);
        if (diffuse)
        {
          diffuse.Bind(shader_asset, "u_material_diffuse", 0);
        }
        auto specular_pair = material->specular;
        auto& specular = FLX_ASSET_GET(Asset::Texture, specular_pair.first);
        if (specular)
        {
          specular.Bind(shader_asset, "u_material_specular", 1);
          shader_asset.SetUniform_float("u_material_shininess", specular_pair.second);
        }

        // draw
        OpenGLRenderer::Draw(mesh.IBO->GetCount());

        // cleanup
        mesh.VAO->Unbind();
      }
    }
    #endif

    #pragma endregion


    #pragma region 3D Renderer System (Model)

    #if 1
    {
      // cache camera
      auto camera = main_camera.GetComponent<Camera>();
      // cache point light
      auto& pt_light_pos = point_light.GetComponent<GlobalPosition>()->position;
      auto pt_light = point_light.GetComponent<PointLight>();
      // cache directional light
      //auto dir_light_pos = directional_light.GetComponent<GlobalPosition>();
      //auto dir_light = directional_light.GetComponent<DirectionalLight>();

      // Render all entities
      for (auto& entity : FlexECS::Scene::GetActiveScene()->View<Transform, Model, Shader>())
      {
        auto& transform = entity.GetComponent<Transform>()->transform;
        auto& model = entity.GetComponent<Model>()->model;
        auto& shader = entity.GetComponent<Shader>()->shader;

        // shader setup
        auto& shader_asset = FLX_ASSET_GET(Asset::Shader, shader);
        shader_asset.Use();

        shader_asset.SetUniform_mat4("u_view", camera->view);
        shader_asset.SetUniform_mat4("u_projection", camera->projection);

        shader_asset.SetUniform_vec3("u_light_position", pt_light_pos);
        shader_asset.SetUniform_vec3("u_light_ambient", pt_light->ambient);
        shader_asset.SetUniform_vec3("u_light_diffuse", pt_light->diffuse);
        shader_asset.SetUniform_vec3("u_light_specular", pt_light->specular);

        //shader_asset.SetUniform_vec3("u_light_position", dir_light_pos->position);
        //shader_asset.SetUniform_vec3("u_light_ambient", dir_light->ambient);
        //shader_asset.SetUniform_vec3("u_light_diffuse", dir_light->diffuse);
        //shader_asset.SetUniform_vec3("u_light_specular", dir_light->specular);

        // get model
        auto& model_asset = FLX_ASSET_GET(Asset::Model, model);

        // render all meshes
        for (auto& mesh : model_asset.meshes)
        {
          // render mesh
          mesh.VAO->Bind();
          mesh.VBO->Bind();
          mesh.IBO->Bind();

          // TODO: accumulate transformations
          // local, global, parent, etc.
          Matrix4x4 model_transform = transform * mesh.transform;
          shader_asset.SetUniform_mat4("u_model", model_transform);

          //// hardcoded
          //auto& texture_asset = FLX_ASSET_GET(Asset::Texture, R"(\models\firetruck\Textures\colormap.png)");
          //texture_asset.Bind(shader_asset, "u_texture_diffuse", 0);

          //// first version
          //// materials are stored in the mesh
          //mesh.material.GetDiffuse()->Bind(shader_asset, "u_texture_diffuse", 0);
          //auto specular = mesh.material.GetSpecular();
          //specular.first->Bind(shader_asset, "u_texture_specular", 0);
          //shader_asset.SetUniform_float("u_shininess", specular.second);

          // second version
          // materials are stored in the model
          // material index is stored in the mesh

          if (mesh.material_index < model_asset.materials.size())
          {
            auto& material = model_asset.materials[mesh.material_index];
            auto diffuse = material.GetDiffuse();
            if (diffuse)
            {
              diffuse->Bind(shader_asset, "u_material_diffuse", 0);
            }
            auto specular = material.GetSpecular();
            if (specular.first)
            {
              specular.first->Bind(shader_asset, "u_material_specular", 1);
              shader_asset.SetUniform_float("u_material_shininess", specular.second);
            }
          }

          // draw
          OpenGLRenderer::Draw(mesh.IBO->GetCount());

          // cleanup
          mesh.VAO->Unbind();
        }
      }
    }
    #endif

    #pragma endregion

  }

}
