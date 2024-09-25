#include "overworld.h"

#include "States.h"
#include "Layers.h"

#include "Components/battlecomponents.h"
#include "Components/charactercomponents.h"
#include "Components/physics.h"
#include "Components/rendering.h"
#include "Components/characterinput.h"

#include "BattleSystems/physicssystem.h"

#include "Renderer/sprite2d.h"


namespace ChronoShift {


  void OverworldLayer::SetupWorld()
  {
    auto scene = FlexECS::Scene::GetActiveScene();

    FlexECS::Entity player1 = FlexECS::Scene::CreateEntity("White Queen");
    player1.AddComponent<CharacterInput>({ });
    player1.AddComponent<Rigidbody>({ {}, false });
    player1.AddComponent<BoundingBox2D>({ });
    player1.AddComponent<IsActive>({ true });
    player1.AddComponent<Position>({ {200, 600} });
    player1.AddComponent<Scale>({ { 100,100 } });
    player1.AddComponent<ZIndex>({ 10 });
    player1.AddComponent<Sprite>({
      //scene->Internal_StringStorage_New(R"(\images\chess_queen.png)"),
      scene->Internal_StringStorage_New(R"()"),
      Vector3::One,
      Vector3::Zero,
      Vector3::One,
      Renderer2DProps::Alignment_Center
     });
    player1.AddComponent<Shader>({ scene->Internal_StringStorage_New(R"(\shaders\texture)") });

  
    FlexECS::Entity house = FlexECS::Scene::CreateEntity();
    house.AddComponent<TurnOrderDisplay>({});
    house.AddComponent<Rigidbody>({ {}, true });
    house.AddComponent<BoundingBox2D>({ });
    house.AddComponent<IsActive>({ true });
    house.AddComponent<Position>({ {800, 500 } });
    house.AddComponent<Scale>({ { 250,250 } });
    house.AddComponent<ZIndex>({ 10 });
    house.AddComponent<Sprite>({
        scene->Internal_StringStorage_New(R"()"),
        { 0.45f, 0.58f, 0.32f },
        Vector3::Zero,
        Vector3::One,
        Renderer2DProps::Alignment_Center
       });
    house.AddComponent<Shader>({ scene->Internal_StringStorage_New(R"(\shaders\texture)") });

    FlexECS::Entity box = FlexECS::Scene::CreateEntity();
    box.AddComponent<TurnOrderDisplay>({});
    box.AddComponent<Rigidbody>({ {}, false });
    box.AddComponent<BoundingBox2D>({ });
    box.AddComponent<IsActive>({ true });
    box.AddComponent<Position>({ {350, 500 } });
    box.AddComponent<Scale>({ { 150,150 } });
    box.AddComponent<ZIndex>({ 10 });
    box.AddComponent<Sprite>({
        scene->Internal_StringStorage_New(R"()"),
        { 0.35f, 0.58f, 0.80f },
        Vector3::Zero,
        Vector3::One,
        Renderer2DProps::Alignment_Center
       });
    box.AddComponent<Shader>({ scene->Internal_StringStorage_New(R"(\shaders\texture)") });


  }



  void OverworldLayer::OnAttach()
  {
    FLX_FLOW_BEGINSCOPE();

    // ECS Setup
    auto scene = FlexECS::Scene::CreateScene();
    FlexECS::Scene::SetActiveScene(scene);

    SetupWorld();
  }

  void OverworldLayer::OnDetach()
  {
    FLX_FLOW_ENDSCOPE();
  }

  void OverworldLayer::Update()
  {
    for (auto& entity : FlexECS::Scene::GetActiveScene()->View<CharacterInput>())
    {
      entity.GetComponent<CharacterInput>()->up = Input::GetKey(GLFW_KEY_W);
      entity.GetComponent<CharacterInput>()->down = Input::GetKey(GLFW_KEY_S);
      entity.GetComponent<CharacterInput>()->left = Input::GetKey(GLFW_KEY_A);
      entity.GetComponent<CharacterInput>()->right = Input::GetKey(GLFW_KEY_D);
    }

    for (auto& entity : FlexECS::Scene::GetActiveScene()->View<CharacterInput, Rigidbody>())
    {
      auto& velocity = entity.GetComponent<Rigidbody>()->velocity;
      velocity.x = 0.0f;
      velocity.y = 0.0f;

      if (entity.GetComponent<CharacterInput>()->up)
      {
        velocity.y = -300.0f;
      }

      if (entity.GetComponent<CharacterInput>()->down)
      {
        velocity.y = 300.0f;
      }

      if (entity.GetComponent<CharacterInput>()->left)
      {
        velocity.x = -300.0f;
      }

      if (entity.GetComponent<CharacterInput>()->right)
      {
        velocity.x = 300.0f;
      }
    }

    UpdatePhysicsSystem();
    RendererSprite2D();
  }
}
