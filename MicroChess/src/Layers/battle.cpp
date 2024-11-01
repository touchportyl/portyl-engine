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
#include "CharacterPrefab/characterprefab.h"


namespace ChronoShift {

  void BattleLayer::SetupBattle()
  {

    File& file = File::Open(Path::current().append("demobattle.flxscene"));
    auto loaded_scene = FlexECS::Scene::Load(file);
    FlexECS::Scene::SetActiveScene(loaded_scene);

    MoveRegistry::RegisterMoveFunctions();
    MoveRegistry::RegisterStatusFunctions();
    m_battlesystem.AddCharacters(FlexECS::Scene::GetActiveScene()->Query<Character>());
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

    DisplayTurnOrder(m_battlesystem.GetTurnOrder());
    RendererSprite2D();
    
    /*if (Input::GetKeyDown(GLFW_KEY_S)) {
      SaveCharacters();
    }*/
    if (Input::GetKeyDown(GLFW_KEY_R)) {
      ResetCharacters();
      m_battlesystem.AddCharacters(FlexECS::Scene::GetActiveScene()->CachedQuery<Character>());
      m_battlesystem.BeginBattle();
    }
  }
}