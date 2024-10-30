/* Start Header
*****************************************************************/
/*!
WLVERSE [https://wlverse.web.app]
\file      battlesystem.cpp
\author    [100%] Ho Jin Jie Donovan, h.jinjiedonovan, 2301233
\par       h.jinjiedonovan\@digipen.edu
\date      03 October 2024
\brief     This file is where the functions utilized by the
           battle system to execute the battle scene, is defined
           at.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/* End Header
*******************************************************************/
#include "battlesystem.h"

#include "States.h"
#include "Layers.h"

#include "Components/battlecomponents.h"
#include "Components/physics.h"
#include "Components/rendering.h"

#include "BattleSystems/turnorderdisplay.h"
#include "BattleSystems/targetingsystem.h"

#include "Physics/box2d.h"
#include "Renderer/sprite2d.h"

// temporary
#include "CharacterPrefab/characterprefab.h"


namespace ChronoShift {


  //Sorting functions
  struct SortLowestSpeed
  {
    bool operator()(FlexECS::Entity e1, FlexECS::Entity e2) {
      return e1.GetComponent<Character>()->current_speed < e2.GetComponent<Character>()->current_speed;
    }
  };


  BattleSystem::BattleSystem() : players_displayed(0), enemies_displayed(0) {

  }
  BattleSystem::~BattleSystem()
  {
  }
  void BattleSystem::SetUpBattleScene() {
    auto scene = FlexECS::Scene::GetActiveScene();

    for (int i = 0; i < (players_displayed + enemies_displayed); i++) {
      FlexECS::Entity slot; //FlexECS::Scene::CreateEntity("Slot" + std::to_string(i));
      if (i < players_displayed) slot = m_players[i];
      else slot = m_enemies[i - players_displayed];
      slot.AddComponent<OnClick>({});
      slot.AddComponent<IsActive>({ false });
      slot.AddComponent<Position>({});
      slot.AddComponent<Scale>({ { 100,100 } });
      slot.AddComponent<ZIndex>({ 9 });
      slot.AddComponent<Rotation>({});
      slot.AddComponent<Transform>({});
      slot.AddComponent<Sprite>({
        scene->Internal_StringStorage_New(R"()"),
        SLOT_COLOR_PLAYER,
        Vector3::Zero,
        Vector3::One,
        Renderer2DProps::Alignment_Center
       });
      slot.AddComponent<Shader>({ scene->Internal_StringStorage_New(R"(\shaders\texture)") });

      if (i < players_displayed) {
        slot.GetComponent<Position>()->position = { SLOT_POS_HOR_PLAYER + 120.f * i, SLOT_POS_VERT_PLAYER };
        //m_players.push_back(slot);
      }
      else {
        slot.GetComponent<Position>()->position = { SLOT_POS_HOR_ENEMY + 120.f * (i - players_displayed), SLOT_POS_VERT_ENEMY };
        //m_enemies.push_back(slot);
      }
      //m_slots.push_back(slot);
    }
    // Move Buttons for Mouse Click Selection of Moves
    if (!move_buttons.empty()) return;
    for (int i = 0; i < 3; i++)
    {
      Vector2 position = { 900.f, 450.f + (80.f * i) };

      FlexECS::Entity move_button = FlexECS::Scene::CreateEntity();
      move_button.AddComponent<MoveButton>({ i });
      move_button.AddComponent<OnHover>({});
      move_button.AddComponent<OnClick>({});
      move_button.AddComponent<IsActive>({ false });
      move_button.AddComponent<Position>({ position });
      move_button.AddComponent<Scale>({ { 350,70 } });
      move_button.AddComponent<ZIndex>({ 9 });
      move_button.AddComponent<Rotation>({});
      move_button.AddComponent<Transform>({});
      move_button.AddComponent<Sprite>({
        scene->Internal_StringStorage_New(R"()"),
        Vector3::One,
        Vector3::Zero,
        Vector3::One - Vector3(0.10f * i, 0.10f * i , 0.10f * i),
        Renderer2DProps::Alignment_Center
       });
      move_button.AddComponent<Shader>({ scene->Internal_StringStorage_New(R"(\shaders\texture)") });
      move_buttons.push_back(move_button);
    }
  }

  void BattleSystem::AddCharacters(std::vector<FlexECS::Entity> characters) {
    if (!m_characters.empty()) m_characters.clear();
    if (!m_enemies.empty()) {
      for (auto& e : m_enemies) {
        FlexECS::Scene::DestroyEntity(e);
      }
      m_enemies.clear();
    }
    if (!m_players.empty()) {
      for (auto& p : m_players) {
        FlexECS::Scene::DestroyEntity(p);
      }
      m_players.clear();
    }
    // positions of character sprites should be updated according to the slot positions
    for (size_t i = 0; i < characters.size(); i++) {
      m_characters.push_back(characters[i]);
      FlexECS::Entity temp_character = FlexECS::Scene::CreateEntity();
      temp_character.AddComponent<BattleSlot>({ characters[i] });
      if (characters[i].GetComponent<Character>()->is_player) {
        m_players.push_back(temp_character);
      }
      else m_enemies.push_back(temp_character);
    }
    players_displayed = m_players.size();
    enemies_displayed = m_enemies.size();
    SetUpBattleScene();
  }

  void BattleSystem::BeginBattle()
  {
    ResetCharacters();
    std::sort(m_characters.begin(), m_characters.end(), SortLowestSpeed());

    auto scene = FlexECS::Scene::GetActiveScene();
    std::cout << "Beginning Stack Order:\nName / HP / Speed \n";
    for (auto& entity : m_characters)
    {
      auto cw = entity.GetComponent<Character>()->weapon_name;
      std::cout << scene->Internal_StringStorage_Get(entity.GetComponent<Character>()->character_name)
        << "  HP: " << entity.GetComponent<Character>()->current_health
        << "  Spd: " << entity.GetComponent<Character>()->current_speed
        << " Weapon: " << scene->Internal_StringStorage_Get(cw);
      if (entity.GetComponent<Character>()->is_player) {
        auto cg = entity.GetComponent<Character>()->chrono_gear_description;
        std::cout << " Chrono Gear: " << scene->Internal_StringStorage_Get(cg);
      }
      std::cout << std::endl;
    }
    std::cout << "\n";

    FlexECS::Entity battle_state = FlexECS::Scene::GetActiveScene()->Query<BattleState>()[0];
    battle_state.GetComponent<BattleState>()->phase = BP_PROCESSING;
  }

  void BattleSystem::UpdateSpeedStack()
  {
    auto scene = FlexECS::Scene::GetActiveScene();
    int speed_to_decrease = m_characters.front().GetComponent<Character>()->current_speed;
    FlexECS::Scene::StringIndex front_character = m_characters.front().GetComponent<Character>()->character_name;
    std::cout << scene->Internal_StringStorage_Get(front_character) << " turn:" << std::endl;
    for (auto& entity : m_characters)
    {
      entity.GetComponent<Character>()->current_speed -= speed_to_decrease;
      std::cout << scene->Internal_StringStorage_Get(entity.GetComponent<Character>()->character_name)
        << "  HP: " << entity.GetComponent<Character>()->current_health
        << "  Spd: " << entity.GetComponent<Character>()->current_speed << std::endl;
    }
    std::cout << "\n";

    FlexECS::Entity battle_state = FlexECS::Scene::GetActiveScene()->Query<BattleState>()[0];
    battle_state.GetComponent<BattleState>()->active_character = m_characters.front();
    battle_state.GetComponent<BattleState>()->phase = BP_MOVE_SELECTION;
  }

  void BattleSystem::Update()
  {
    FlexECS::Entity battle_state = FlexECS::Scene::GetActiveScene()->Query<BattleState>()[0];
    int battle_phase = battle_state.GetComponent<BattleState>()->phase;
    if (battle_phase == BP_PROCESSING) {
      UpdateSpeedStack();
    }
    else if (battle_phase == BP_MOVE_SELECTION) {
      PlayerMoveSelection();
    }
    if (battle_phase != BP_BATTLE_FINISH) EndBattleScene();
  }

  void BattleSystem::PlayerMoveSelection()
  {
    static auto selected_num = m_enemies.begin();
    std::array<FlexECS::Scene::StringIndex, 3> character_moves{
      m_characters.front().GetComponent<Character>()->weapon_move_one,
      m_characters.front().GetComponent<Character>()->weapon_move_two,
      m_characters.front().GetComponent<Character>()->weapon_move_three,
    };

    for (auto& slot : m_enemies) {
      slot.GetComponent<IsActive>()->is_active = false;
    }
    for (auto& slot : m_players) {
      slot.GetComponent<IsActive>()->is_active = false;
    }

    static int move_decision = -1; // variable storing move selection
    if (!m_characters.front().GetComponent<Character>()->is_player) {
      // Random move selection for enemies
      move_decision = Range(0, 2).Get();
    }
    else {
      for (auto& entity : FlexECS::Scene::GetActiveScene()->Query<IsActive, MoveButton>()) {
        entity.GetComponent<IsActive>()->is_active = true;
      }
      // move selection system by mouse click for players
      GetMoveSelection();
      for (auto& entity : FlexECS::Scene::GetActiveScene()->Query<OnClick, MoveButton>())
      {
        auto& click_status = entity.GetComponent<OnClick>()->is_clicked;
        if (click_status == true) {
          move_decision = entity.GetComponent<MoveButton>()->move_number;
          Move player_move = MoveRegistry::GetMove(
            FlexECS::Scene::GetActiveScene()->Internal_StringStorage_Get(
              character_moves[move_decision]));
          std::cout << "Move Selected: " << player_move.name << std::endl;
          if (player_move.is_target_enemy) selected_num = m_enemies.begin();
          else selected_num = m_players.begin();
          //selected_num = (!player_move.is_target_player * players_displayed);
          //selected_num += (!player_move.is_target_player * players_displayed);
        }
      }
    }
    if (move_decision == -1) return;
    Move selected_move = MoveRegistry::GetMove(
            FlexECS::Scene::GetActiveScene()->Internal_StringStorage_Get(
              character_moves[move_decision]));
    std::vector<FlexECS::Entity> result;
    auto max_targets = m_enemies.end();
    auto min_targets = m_enemies.begin();
    if (m_characters.front().GetComponent<Character>()->is_player) {
      static int adjacent = 1;

      // Identify whether target is enemy or team
      // number of targets
      // selection of adjecent targets should only apply when target count is less than 5
      size_t no_of_targets = 0;
      if (!selected_move.is_target_enemy) {
        // means is player target
        no_of_targets = players_displayed;
        min_targets = m_players.begin();
        max_targets = m_players.end();
      }
      else {
        // means is enemy target
        no_of_targets = enemies_displayed;
      }
      if (selected_move.target_type < no_of_targets) {
        // targetting system
        if (Input::GetKeyDown(GLFW_KEY_D)) {
          if (selected_num != (max_targets - 1)) selected_num++;
        }
        if (Input::GetKeyDown(GLFW_KEY_A)) {
          if (selected_num != min_targets) selected_num--;
        }

        if (selected_num == (max_targets - 1)) adjacent = -1;
        else if (selected_num == min_targets) adjacent = 1;

        // This needs to be resolved ASAP
        // display sprites when selected, adjacency matters
        if (selected_num != min_targets || selected_num != (max_targets - 1)) {
          if (selected_move.target_type == MOVE_TARGET_TRIPLE) {
            (*(selected_num - 1)).GetComponent<IsActive>()->is_active = true;
            (*(selected_num + 1)).GetComponent<IsActive>()->is_active = true;
          }
          else if (selected_move.target_type == MOVE_TARGET_DOUBLE) {
            (*(selected_num + adjacent)).GetComponent<IsActive>()->is_active = true;
          }
        }
        else (*(selected_num + adjacent)).GetComponent<IsActive>()->is_active = true;
        (*selected_num).GetComponent<IsActive>()->is_active = true;
      }
      else {
        // all the slots are to be targetted
        for (auto i = min_targets; i != max_targets; i++) {
          (*i).GetComponent<IsActive>()->is_active = true;
        }
      }
      if (Input::GetKeyDown(GLFW_KEY_SPACE)) {
        for (auto i = min_targets; i != max_targets; i++) {
          if ((*i).GetComponent<IsActive>()->is_active) result.push_back((*i).GetComponent<BattleSlot>()->character);
        }
      }
    }
    else {
      int max = 0;
      if (!selected_move.is_target_enemy) {
        max = static_cast<int>(players_displayed);
        min_targets = m_players.begin();
      }
      else max = static_cast<int>(enemies_displayed);
      int random_selection = Range(0, max).Get();
      if (selected_move.target_type < max) {
        if (random_selection != 0 || random_selection != max) {
          if (selected_move.target_type == MOVE_TARGET_TRIPLE) {
            result.push_back((*(min_targets + 1)).GetComponent<BattleSlot>()->character);
            result.push_back((*(min_targets - 1)).GetComponent<BattleSlot>()->character);
          }
          else if (selected_move.target_type == MOVE_TARGET_DOUBLE) result.push_back((*(min_targets - 1)).GetComponent<BattleSlot>()->character);
        }
        else {
          if (random_selection == max) result.push_back((*(min_targets - 1)).GetComponent<BattleSlot>()->character);
          else result.push_back((*(min_targets + 1)).GetComponent<BattleSlot>()->character);
        }
        result.push_back((*min_targets).GetComponent<BattleSlot>()->character);
      }
      else {
        // all the slots are to be targetted
        for (int i = 0; i < max; i++) {
          result.push_back((*(min_targets + i)).GetComponent<BattleSlot>()->character);
        }
      }
    }
    if (result.empty()) return;

    int final_decision = move_decision;
    move_decision = -1;
    std::vector<FlexECS::Entity> final_result = result;
    result.clear();
    ExecuteMove(character_moves[final_decision], final_result);
  }

  void BattleSystem::ExecuteMove(FlexECS::Scene::StringIndex move_id, std::vector<FlexECS::Entity> selected_targets)
  {
    //get the move user
    FlexECS::Entity battle_state = FlexECS::Scene::GetActiveScene()->Query<BattleState>()[0];
    FlexECS::Entity user = battle_state.GetComponent<BattleState>()->active_character;
    Move move = MoveRegistry::GetMove(FlexECS::Scene::GetActiveScene()->Internal_StringStorage_Get(move_id));
    std::vector<FlexECS::Entity> targets;
    targets.insert(targets.begin(), selected_targets.begin(), selected_targets.end());
    //execute move
    move.effect(targets, move.damage);
    user.GetComponent<Character>()->current_speed += move.cost + user.GetComponent<Character>()->base_speed;
    user.GetComponent<Action>()->move_to_use = FlexECS::Entity::Null;
    std::sort(m_characters.begin(), m_characters.end(), SortLowestSpeed());

    auto scene = FlexECS::Scene::GetActiveScene();
    //Display Character Move and target selection
    std::cout << scene->Internal_StringStorage_Get(user.GetComponent<Character>()->character_name)
      << " executed " << move.name
      << " on ";
    for (int i = 0; i < targets.size(); ++i) {
      std::cout << scene->Internal_StringStorage_Get(targets[i].GetComponent<Character>()->character_name);
      if ((i + 1) < targets.size()) std::cout << " and ";
    }
    std::cout << std::endl;
    // Display Status of alive Characters
    targets.clear();
    for (auto& entity : m_characters)
    {
      if (entity.GetComponent<Character>()->current_health > 0) {
        std::cout << scene->Internal_StringStorage_Get(entity.GetComponent<Character>()->character_name)
          << "  HP: " << entity.GetComponent<Character>()->current_health
          << "  Spd: " << entity.GetComponent<Character>()->current_speed << std::endl;
      }
      else targets.push_back(entity);
    }
    // Display Characters that have died
    for (auto& dead_character : targets)
    {
      std::cout << scene->Internal_StringStorage_Get(dead_character.GetComponent<Character>()->character_name)
        << " has been killed by " << scene->Internal_StringStorage_Get(user.GetComponent<Character>()->character_name)
        << "'s " << move.name << std::endl;
    }
    std::cout << "\n";
    battle_state.GetComponent<BattleState>()->current_target_count = 0;
    battle_state.GetComponent<BattleState>()->active_character = FlexECS::Entity::Null;
    DeathProcession(targets);
  }

  void BattleSystem::DeathProcession(std::vector<FlexECS::Entity> list_of_deaths) {
    FlexECS::Entity battle_state = FlexECS::Scene::GetActiveScene()->Query<BattleState>()[0];
    for (auto it = list_of_deaths.begin(); it != list_of_deaths.end(); it++) {
      for (auto c = m_characters.begin(); c != m_characters.end(); c++) {
        if (*c == *it) {
          m_characters.erase(c);
          std::cout << (*it).GetComponent<Character>()->character_name << " has been removed from characters list" << std::endl;
          break;
        }
      }
      if ((*it).GetComponent<Character>()->is_player) {
        for (auto p = m_players.begin(); p != m_players.end(); p++) {
          if ((*p).GetComponent<BattleSlot>()->character == *it) {
            FlexECS::Scene::DestroyEntity(*p);
            m_players.erase(p);
            players_displayed--;
            std::cout << (*it).GetComponent<Character>()->character_name << " has been removed from player list" << std::endl;
            break;
          }
        }
      }
      else {
        for (auto e = m_enemies.begin(); e != m_enemies.end(); e++) {
          if ((*e).GetComponent<BattleSlot>()->character == *it) {
            FlexECS::Scene::DestroyEntity(*e);
            m_enemies.erase(e);
            enemies_displayed--;
            std::cout << (*it).GetComponent<Character>()->character_name << " has been removed from enemy list" << std::endl;
            break;
          }
        }
      }
    }
    battle_state.GetComponent<BattleState>()->phase = BP_PROCESSING;
  }
  void BattleSystem::EndBattleScene() {
    FlexECS::Entity battle_state = FlexECS::Scene::GetActiveScene()->Query<BattleState>()[0];
    if (m_enemies.empty()) {
      std::cout << "VICTORY!!!!" << std::endl;
      battle_state.GetComponent<BattleState>()->phase = BP_BATTLE_FINISH;
    }
    if (m_players.empty()) {
      std::cout << "DEFEAT. Click R to try again." << std::endl;
      battle_state.GetComponent<BattleState>()->phase = BP_BATTLE_FINISH;
    }
  }
}