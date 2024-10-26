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
    auto scene = FlexECS::Scene::GetActiveScene();

    //battle positions for characters
    m_battlesystem.SetUpBattleScene(3, 2);
    #pragma region Chrono Gear Main Stats
    /*FlexECS::Entity main_hp = FlexECS::Scene::CreateEntity("Flat HP");
    main_hp.AddComponent<Stat>({
      scene->Internal_StringStorage_New("Flat HP"),
      112,
      112
      });
    FlexECS::Entity main_atk = FlexECS::Scene::CreateEntity("Flat Main ATK Stats");
    main_atk.AddComponent<Stat>({
      scene->Internal_StringStorage_New("Flat Main ATK Stats"),
      56,
      56
      });
    FlexECS::Entity main_spd = FlexECS::Scene::CreateEntity("Flat Main SPD Stats");
    main_spd.AddComponent<Stat>({
      scene->Internal_StringStorage_New("Flat Main SPD Stats"),
      4,
      4
      });*/
    #pragma endregion

    #pragma region Chrono Gear Sub Stats
    /*FlexECS::Entity sub_hp = FlexECS::Scene::CreateEntity("Flat Sub HP Stats");
    sub_hp.AddComponent<Stat>({
      scene->Internal_StringStorage_New("Flat Sub HP Stats"),
      38,
      38
      });
    FlexECS::Entity sub_atk = FlexECS::Scene::CreateEntity("Flat Sub ATK Stats");
    sub_atk.AddComponent<Stat>({
      scene->Internal_StringStorage_New("Flat Sub ATK Stats"),
      21,
      21
      });
    FlexECS::Entity sub_spd = FlexECS::Scene::CreateEntity("Flat Sub SPD Stats");
    sub_spd.AddComponent<Stat>({
      scene->Internal_StringStorage_New("Flat Sub SPD Stats"),
      2,
      2
      });*/
    #pragma endregion

    #pragma region Character Components
    //scene->Internal_StringStorage_New("White Queen"); //27
    //scene->Internal_StringStorage_New("White King");    //28
    //scene->Internal_StringStorage_New("Black Knight");  // 29
    //scene->Internal_StringStorage_New("Black Bishop"); // 30
    //scene->Internal_StringStorage_New("Black Pawn");   // 31

    //scene->Internal_StringStorage_New("Enemy Weapon"); // 32
    //scene->Internal_StringStorage_New("Dummy Weapon"); // 33
    //scene->Internal_StringStorage_New("Poke");         // 34

    //scene->Internal_StringStorage_New("Plasma Blade"); // 35
    //scene->Internal_StringStorage_New("Blade made of plasma");
    //scene->Internal_StringStorage_New("Dual Strike"); // Double Enemy Target
    //scene->Internal_StringStorage_New("Whirlwind"); // Entire Enemy Target
    //scene->Internal_StringStorage_New("Block"); // Self Target
    //scene->Internal_StringStorage_New("Flashbang"); // Single Enemy Target

    //scene->Internal_StringStorage_New("Electric Railgun");
    //scene->Internal_StringStorage_New("Railgun that uses electromagnetic force");
    //scene->Internal_StringStorage_New("Rift Surge"); // Double Ally Target
    //scene->Internal_StringStorage_New("Full Recovery"); // Entire Team Target
    //scene->Internal_StringStorage_New("Heal"); // Single Ally Target

    //scene->Internal_StringStorage_New("Demo Chrono Gear");
    //scene->Internal_StringStorage_New("Revives a dead drifter");
    //scene->Internal_StringStorage_New("Instant Win");

    //FlexECS::Scene::StringIndex shdr_path = scene->Internal_StringStorage_New(R"(\shaders\texture)");
    //scene->Internal_StringStorage_New(R"(\images\chess_queen.png)");
    //scene->Internal_StringStorage_New(R"(\images\chess_king.png)");
    //scene->Internal_StringStorage_New(R"(\images\chess_knight.png)");
    //scene->Internal_StringStorage_New(R"(\images\chess_bishop.png)");
    //scene->Internal_StringStorage_New(R"(\images\chess_pawn.png)");

    File& file = File::Open(Path::current().append("pee.flxscene"));
    auto loaded_scene = FlexECS::Scene::Load(file);
    FlexECS::Scene::SetActiveScene(loaded_scene);
    
    #if 0
    FlexECS::Entity player1 = LoadCharacter("White Queen");
    FlexECS::Entity player2 = LoadCharacter("White King");
    FlexECS::Entity enemy1 = LoadCharacter("Black Knight");
    FlexECS::Entity enemy2 = LoadCharacter("Black Bishop");
    FlexECS::Entity enemy3 = LoadCharacter("Black Pawn");

    Asset::FlxData flex("/data/BlackBishop.flxdata");
    flex.SetBool("test", true);
    flex.SetString("tester", "Hello2 World!");
    flex.SetInt("testint", 1243);
    flex.Save();

    std::vector<FlexECS::Entity> list_of_characters;
    list_of_characters.push_back(player1);
    list_of_characters.push_back(player2);
    list_of_characters.push_back(enemy1);
    list_of_characters.push_back(enemy2);
    list_of_characters.push_back(enemy3);
    m_battlesystem.AddCharacters(list_of_characters);

    /*m_battlesystem.AddCharacter(player1, 0);
    m_battlesystem.AddCharacter(player2, 1);
    m_battlesystem.AddCharacter(enemy1, 2);
    m_battlesystem.AddCharacter(enemy2, 3);
    m_battlesystem.AddCharacter(enemy3, 4);*/

    player1.AddComponent<Transform>({ });
    player2.AddComponent<Transform>({ });
    enemy1.AddComponent<Transform>({ });
    enemy2.AddComponent<Transform>({ });
    enemy3.AddComponent<Transform>({ });

    
    #pragma region Turn Order Display

    for (size_t i = 0; i < m_battlesystem.GetCharacterCount(); i++)
    {
      FlexECS::Entity display_slot = FlexECS::Scene::CreateEntity("TurnOrder" + std::to_string(i));
      display_slot.AddComponent<TurnOrderDisplay>({});
      display_slot.AddComponent<IsActive>({ false });
      display_slot.AddComponent<Position>({ });
      display_slot.AddComponent<Scale>({ { 60,60 } });
      display_slot.AddComponent<ZIndex>({ 10 });
      display_slot.AddComponent<Sprite>({});
      display_slot.AddComponent<Rotation>({});
      display_slot.AddComponent<Transform>({});
      display_slot.AddComponent<Shader>({ shdr_path });
    }

    #pragma endregion

    FlexECS::Entity battle_state = FlexECS::Scene::CreateEntity("BattleState");
    battle_state.AddComponent<BattleState>({});
    #endif

    MoveRegistry::RegisterMoves();
    m_battlesystem.AddCharacters(FlexECS::Scene::GetActiveScene()->View<Character>());
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
    UpdateSprite2DMatrix();
    RendererSprite2D();
    
    if (Input::GetKeyDown(GLFW_KEY_S)) {
      SaveCharacters();
    }
    else if (Input::GetKeyDown(GLFW_KEY_R)) {
      ResetCharacters();
    }
  }
}