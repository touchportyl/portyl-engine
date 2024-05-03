#include "mainlayer.h"

#include "Components/Components.h"

namespace OpenGLRendering
{

  void MainLayer::OnAttach()
  {
    FLX_FLOW_BEGINSCOPE();

    AssetManager::Load();

    FlexECS::Scene::CreateScene();
    //camera = FlexECS::Scene::CreateEntity("Camera");
    //cube = FlexECS::Scene::CreateEntity("Cube");
    plane = FlexECS::Scene::CreateEntity("Plane");

    //camera.AddComponent<Transform>({});
    //camera.AddComponent<Camera>({});

    //cube.AddComponent<Transform>({});
    //cube.AddComponent<Mesh>({});

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

    #pragma region Camera System

    //glm::mat4 camera_transform = camera.GetComponent<Transform>()->transform;

    #pragma endregion

    #pragma region Renderer System

    Renderer::Clear();

    auto view = FlexECS::Scene::GetActiveScene()->View<Transform, Mesh, Texture, Shader>();
    
    for (auto& entity : view)
    {
      auto transform = entity.GetComponent<Transform>();
      auto mesh = entity.GetComponent<Mesh>();
      auto texture = entity.GetComponent<Texture>();
      auto shader = entity.GetComponent<Shader>();

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

      // get shader
      //Asset::Shader& shader_asset = std::get<Asset::Shader>(*AssetManager::Get(shader->shader));
      auto& shader_asset = FLX_ASSET_GET(Asset::Shader, shader->shader);

      // bind the shader
      shader_asset.Use();

      // get the texture
      //Asset::Texture& texture_asset = std::get<Asset::Texture>(*AssetManager::Get(texture->texture));
      auto& texture_asset = FLX_ASSET_GET(Asset::Texture, texture->texture);

      // bind the texture
      texture_asset.Bind();

      Renderer::Draw(IBO->GetCount());
    }

    #pragma endregion

    #if 0
    {
      ImGui::Begin("Image Test");
      ImGui::Image(IMGUI_IMAGE(texture_asset));
      ImGui::End();
    }
    #endif

  }

}
