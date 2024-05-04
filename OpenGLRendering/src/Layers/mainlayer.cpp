#include "mainlayer.h"

#include "Components/Components.h"

#include <glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <ext/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

namespace OpenGLRendering
{

  void MainLayer::OnAttach()
  {
    FLX_FLOW_BEGINSCOPE();

    AssetManager::Load();

    FlexECS::Scene::CreateScene();
    main_camera = FlexECS::Scene::CreateEntity("Main Camera");
    //cube = FlexECS::Scene::CreateEntity("Cube");
    plane = FlexECS::Scene::CreateEntity("Plane");

    main_camera.AddComponent<GlobalPosition>({ { 0.7f, 0, -3 } });
    main_camera.AddComponent<Rotation>({ { 0, 90, 0 } });
    main_camera.AddComponent<Camera>({});

    //cube.AddComponent<Transform>({});
    //cube.AddComponent<Mesh>({});

    plane.AddComponent<GlobalPosition>({ { 0, 0, 0 } });
    plane.AddComponent<LocalPosition>({ { 0, 0, 0 } });
    plane.AddComponent<Rotation>({ { 0, 0, 20 } });
    Scale plane_scl = { { 0.9f, 0.35f, 1 } };
    plane_scl.scale *= 1.5f;
    plane.AddComponent<Scale>(plane_scl);
    plane.AddComponent<Transform>({});
    //plane.AddComponent<Mesh>({});
    plane.AddComponent<Texture>({ R"(\images\flexengine\flexengine_splash.png)" });
    plane.AddComponent<Shader>({ R"(\shaders\renderer)" });

    // manually build a mesh
    Mesh mesh_plane;

    mesh_plane.vertices = {
         // positions              // colors               // texture coords
      { {  1.0f,  1.0f, 0.0f },   { 1.0f, 0.0f, 0.0f },   { 1.0f, 0.0f } },  // top right
      { {  1.0f, -1.0f, 0.0f },   { 0.0f, 1.0f, 0.0f },   { 1.0f, 1.0f } },  // bottom right
      { { -1.0f, -1.0f, 0.0f },   { 0.0f, 0.0f, 1.0f },   { 0.0f, 1.0f } },  // bottom left
      { { -1.0f,  1.0f, 0.0f },   { 1.0f, 1.0f, 0.0f },   { 0.0f, 0.0f } }   // top left
    };

    mesh_plane.indices = {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
    };

    plane.AddComponent<Mesh>(mesh_plane);
  }

  void MainLayer::OnDetach()
  {
    FLX_FLOW_ENDSCOPE();
  }

  void MainLayer::Update()
  {

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
        ImGui::DragFloat3("Global Position", global_position.begin(), 0.01f, -10.0f, 10.0f, "%.2f");
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
          ImGui::DragFloat3("Global Position", global_position.begin(), 0.01f, -10.0f, 10.0f, "%.2f");
          ImGui::DragFloat3("Local Position", local_position.begin(), 0.01f, -10.0f, 10.0f, "%.2f");
          if (ImGui::DragFloat3("Rotation", rotation.begin(), 0.1f, -0.1f, 360.1f, "%.1f"))
          {
            // loop around
            for (auto& value : rotation)
            {
              if (value > 360.0f) value -= 360.0f;
              else if (value < 0.0f) value += 360.0f;
            }
          }
          ImGui::DragFloat3("Scale", scale.begin(), 0.01f, 0.1f, 10.0f, "%.2f");
          ImGui::PopID();
        }
      }
      ImGui::End();
    }
    #endif

    #pragma endregion


    #pragma region Demo Rotation System

    #if 0
    {
      // Rotate all entities in the scene (except cameras)
      for (auto& entity : FlexECS::Scene::GetActiveScene()->View<Rotation>())
      {
        if (entity.HasComponent<Camera>()) continue;
      
        auto& rotation = entity.GetComponent<Rotation>()->rotation;
      
        // rotate the entity
        rotation.y += 0.5f;
        if (rotation.y > 360.0f) rotation.y -= 360.0f;
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
        transform = Matrix4x4::Identity;
        transform
          .Translate(global_position)
          .RotateZDeg(rotation.z)
          .RotateYDeg(rotation.y)
          .RotateXDeg(rotation.x)
          .Scale(scale)
          .Translate(local_position)
        ;
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
        camera->right = Vector3::Normalize(Cross(camera->world_up, camera->front));

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


    #pragma region Renderer System
    
    #if 1
    {
      // cache camera
      auto camera = main_camera.GetComponent<Camera>();

      // Render all entities
      for (auto& entity : FlexECS::Scene::GetActiveScene()->View<Transform, Mesh, Texture, Shader>())
      {
        auto& transform = entity.GetComponent<Transform>()->transform;
        auto mesh = entity.GetComponent<Mesh>();
        auto& texture = entity.GetComponent<Texture>()->texture;
        auto& shader = entity.GetComponent<Shader>()->shader;

        OpenGLRenderer::EnableDepthTest();
        OpenGLRenderer::ClearFrameBuffer();

        // render mesh
        std::unique_ptr<VertexArray> VAO;
        std::unique_ptr<VertexBuffer> VBO;
        std::unique_ptr<IndexBuffer> IBO;

        VAO.reset(VertexArray::Create());
        VBO.reset(VertexBuffer::Create(mesh->vertices.data(), mesh->vertices.size() * sizeof(Vertex)));
        IBO.reset(IndexBuffer::Create(mesh->indices.data(), static_cast<GLsizei>(mesh->indices.size())));

        VAO->Bind();
        VBO->Bind();
        IBO->Bind();

        // shader setup
        auto& shader_asset = FLX_ASSET_GET(Asset::Shader, shader);
        shader_asset.Use();

        // TODO: accumulate transformations
        // local, global, parent, etc.
        shader_asset.SetUniform_mat4("u_model", transform);

        shader_asset.SetUniform_mat4("u_view", camera->view);
        shader_asset.SetUniform_mat4("u_projection", camera->projection);

        // setup texture
        auto& texture_asset = FLX_ASSET_GET(Asset::Texture, texture);
        texture_asset.Bind();

        // draw
        OpenGLRenderer::Draw(IBO->GetCount());
      }
    }
    #endif

    #pragma endregion

  }

}
