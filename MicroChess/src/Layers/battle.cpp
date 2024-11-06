/* Start Header
*****************************************************************/
/*!
WLVERSE [https://wlverse.web.app]
\file      battle.cpp
\author    [78%] Rocky Sutarius
\par       rocky.sutarius\@digipen.edu
\co-author [22%] Ho Jin Jie Donovan, h.jinjiedonovan, 2301233
\par       h.jinjiedonovan\@digipen.edu
\date      03 October 2024
\brief     This file is where the functions to set up and run
           the battle scene of the game, is contained at. The
           functions include:
           - SetupBattle()
           - OnAttach()
           - OnDetach()
           - Update()

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/* End Header
*******************************************************************/
#include "battle.h"

#include "States.h"
#include "Layers.h"

#include "Components/battlecomponents.h"
#include "Components/physics.h"
#include "Components/rendering.h"

#include "BattleSystems/turnorderdisplay.h"
#include "BattleSystems/targetingsystem.h"
#include "BattleSystems/battlesystem.h"

#include "Physics/box2d.h"
#include "Renderer/sprite2d.h"
#include <Components/characterinput.h>
#include <BattleSystems/physicssystem.h>


namespace ChronoShift {

  void BattleLayer::SetupBattle()
  {

    File& file = File::Open(Path::current().append("demobattle2.flxscene"));
    auto loaded_scene = FlexECS::Scene::Load(file);
    FlexECS::Scene::SetActiveScene(loaded_scene);
    
    auto temp_scene = FlexECS::Scene::GetActiveScene();
    FlexECS::Entity camera;
    for (auto& entity : FlexECS::Scene::GetActiveScene()->CachedQuery<Camera>()) {
      camera = entity;
    }
    // amazing thing here that is required for the camera to not break down the game
    SceneCamSorter::GetInstance().AddCameraEntity(camera.Get(), camera.GetComponent<Camera>()->camera);
    SceneCamSorter::GetInstance().SwitchMainCamera(camera.Get());

    MoveRegistry::RegisterMoveFunctions();
    MoveRegistry::RegisterStatusFunctions();

    m_battlesystem.AddCharacters(FlexECS::Scene::GetActiveScene()->CachedQuery<Character>());
    m_battlesystem.BeginBattle();
  }

  void BattleLayer::OnAttach()
  {
    FLX_FLOW_BEGINSCOPE();

    // ECS Setup
    auto scene = FlexECS::Scene::CreateScene();
    FlexECS::Scene::SetActiveScene(scene);

    // Setup the board
    SetupBattle();
  }

  void BattleLayer::OnDetach()
  {
    FLX_FLOW_ENDSCOPE();
  }

  void BattleLayer::Update()
  {
    m_battlesystem.Update();

    
    RendererSprite2D();
    /*if (Input::GetKeyDown(GLFW_KEY_4)) {
      auto temp_scene = FlexECS::Scene::GetActiveScene();
      for (auto& t : temp_scene->Query<Character>()) {
        FlexECS::Scene::StringIndex how = t.GetComponent<Character>()->character_name;
        std::string temp_name = temp_scene->Internal_StringStorage_Get(how);
        if (temp_name == "Renko") {
          t.AddComponent<CharacterInput>({});
          t.AddComponent<Rigidbody>({ {}, false });
        }
      }
    }*/
    for (auto& entity : FlexECS::Scene::GetActiveScene()->CachedQuery<CharacterInput>())
    {
      entity.GetComponent<CharacterInput>()->up = Input::GetKey(GLFW_KEY_W);
      entity.GetComponent<CharacterInput>()->down = Input::GetKey(GLFW_KEY_S);
      entity.GetComponent<CharacterInput>()->left = Input::GetKey(GLFW_KEY_A);
      entity.GetComponent<CharacterInput>()->right = Input::GetKey(GLFW_KEY_D);
    }

    for (auto& entity : FlexECS::Scene::GetActiveScene()->CachedQuery<CharacterInput, Rigidbody>())
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
  }
}