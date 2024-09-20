#include "battle.h"

#include "States.h"
#include "Layers.h"

#include "Components/battlecomponents.h"
#include "Components/physics.h"
#include "Components/rendering.h"

#include "Battle/turnorderdisplay.h"
#include "Battle/targetingsystem.h"

#include "Physics/box2d.h"
#include "Renderer/sprite2d.h"


namespace ChronoShift {


  //Sorting functions
  struct SortLowestSpeed
  {
    bool operator()(FlexECS::Entity e1, FlexECS::Entity e2) {
      return e1.GetComponent<CharacterSpeed>()->current_speed < e2.GetComponent<CharacterSpeed>()->current_speed;
    }
  };


  BattleSystem::BattleSystem() 
  {
  }
  BattleSystem::~BattleSystem() 
  {
  }
  void BattleSystem::InitializeBattleSlots()
  {
    //for (int i = 0; i < m_player_slots.size(); i++)
    //{
    //  FlexECS::Entity slot = FlexECS::Scene::CreateEntity();
    //  slot.AddComponent<PlayerSlot>({ i, FlexECS::Entity::Null});
    //  m_player_slots[i] = slot;
    //}
    //for (int i = 0; i < m_enemy_slots.size(); i++)
    //{
    //  FlexECS::Entity slot = FlexECS::Scene::CreateEntity();
    //  slot.AddComponent<EnemySlot>({ i, FlexECS::Entity::Null });
    //  m_enemy_slots[i] = slot;
    //}
    auto scene = FlexECS::Scene::GetActiveScene();
    const Vector3 color_player_slot = { 0.45f, 0.58f, 0.32f };
    const Vector3 color_enemy_slot = { 0.77f, 0.12f, 0.23f };
    
    for (int i = 0; i < m_slots.size(); i++)
    {
      bool is_player_slot = (i < 4);
      Vector2 position;
      if (is_player_slot) position = { 200.f + 120.f * i, 600.f };
      else                position = { 700.f + 120.f * (i-4), 200.f };

      FlexECS::Entity slot = FlexECS::Scene::CreateEntity();
      slot.AddComponent<BattleSlot>({ i, FlexECS::Entity::Null });
      slot.AddComponent<OnHover>({});
      slot.AddComponent<OnClick>({});
      slot.AddComponent<IsActive>({ true });
      slot.AddComponent<Position>({ position });
      slot.AddComponent<Scale>({ { 100,100 } });
      slot.AddComponent<ZIndex>({ 9 });
      slot.AddComponent<Sprite>({
        scene->Internal_StringStorage_New(R"()"),
        is_player_slot ? color_player_slot : color_enemy_slot,
        Vector3::Zero,
        Vector3::One,
        Renderer2DProps::Alignment_Center
       });
      slot.AddComponent<Shader>({ scene->Internal_StringStorage_New(R"(\shaders\texture)") });

      m_slots[i] = slot;
    }

    for (int i = 0; i < 4; i++) 
    {
      Vector2 position = { 900.f, 450.f + (80.f * i) };
      
      FlexECS::Entity move_button = FlexECS::Scene::CreateEntity();
      move_button.AddComponent<MoveButton>({i});
      move_button.AddComponent<OnHover>({});
      move_button.AddComponent<OnClick>({});
      move_button.AddComponent<IsActive>({ false });
      move_button.AddComponent<Position>({ position });
      move_button.AddComponent<Scale>({ { 350,70 } });
      move_button.AddComponent<ZIndex>({ 9 });
      move_button.AddComponent<Sprite>({
        scene->Internal_StringStorage_New(R"()"),
        Vector3::One,
        Vector3::Zero,
        Vector3::One - Vector3(0.10f * i, 0.10f * i , 0.10f * i),
        Renderer2DProps::Alignment_Center
       });
      move_button.AddComponent<Shader>({ scene->Internal_StringStorage_New(R"(\shaders\texture)") });
    }
  }
  
  void BattleSystem::SetMovesForCharacter(FlexECS::Entity character)
  {
    //this is just here to cleanify code.... to remove..... thanks.....
    FlexECS::Entity move1 = FlexECS::Scene::CreateEntity();
    FlexECS::Entity move2 = FlexECS::Scene::CreateEntity();
    FlexECS::Entity move3 = FlexECS::Scene::CreateEntity();
    FlexECS::Entity move4 = FlexECS::Scene::CreateEntity();
    move1.AddComponent<MoveID>({});
    move2.AddComponent<MoveID>({});
    move3.AddComponent<MoveID>({});
    move4.AddComponent<MoveID>({});

    auto movelist = character.GetComponent<MoveList>();
    movelist->move_one = move1;
    movelist->move_two = move2;
    movelist->move_three = move3;
    movelist->move_four = move4;
  }

  void BattleSystem::AddCharacter(FlexECS::Entity character, int position) 
  {
    SetMovesForCharacter(character);
    m_characters.push_back(character);
    m_slots[position].GetComponent<BattleSlot>()->character = character;

    //if (character.GetComponent<IsPlayer>()->is_player) {
    //  if (m_slots[position].GetComponent<PlayerSlot>()->character != FlexECS::Entity::Null) {
    //    //maybe we swap or something
    //    Log::Debug("Player Slot already taken");
    //    return;
    //  }
    //  m_player_slots[position] = character;
    //}
    //else {
    //  if (m_enemy_slots[position].GetComponent<EnemySlot>()->character != FlexECS::Entity::Null) {
    //    Log::Debug("Enemy Slot already taken");
    //    return;
    //  }
    //  m_player_slots[position] = character;
    //}
  }

  void BattleSystem::BeginBattle()
  {
    for (FlexECS::Entity entity : m_characters)
    {
      m_speedstack.push_back(entity);
    }
    m_speedstack.sort(SortLowestSpeed());

    auto scene = FlexECS::Scene::GetActiveScene();
    std::cout << "Beginning Stack Order:\nName / HP / Speed \n";
    for (auto& entity : m_speedstack)
    {
      std::cout << scene->Internal_StringStorage_Get(entity.GetComponent<CharacterName>()->character_name)
        << "  HP: " << entity.GetComponent<CharacterHealth>()->current_health
        << "  Spd: " << entity.GetComponent<CharacterSpeed>()->current_speed << std::endl;
    }
    std::cout << "\n";

    FlexECS::Entity battle_state = FlexECS::Scene::GetActiveScene()->View<BattleState>()[0];
    battle_state.GetComponent<BattleState>()->phase = BP_PROCESSING;
  }

  void BattleSystem::UpdateSpeedStack()
  {
    auto scene = FlexECS::Scene::GetActiveScene();
    int speed_to_decrease = m_speedstack.front().GetComponent<CharacterSpeed>()->current_speed;
    for (auto& entity : m_speedstack)
    {
      entity.GetComponent<CharacterSpeed>()->current_speed -= speed_to_decrease;
      std::cout << scene->Internal_StringStorage_Get(entity.GetComponent<CharacterName>()->character_name)
        << "  HP: " << entity.GetComponent<CharacterHealth>()->current_health
        << "  Spd: " << entity.GetComponent<CharacterSpeed>()->current_speed << std::endl;
    }
    std::cout << "\n";
    
    FlexECS::Entity battle_state = FlexECS::Scene::GetActiveScene()->View<BattleState>()[0];
    battle_state.GetComponent<BattleState>()->active_character = m_speedstack.front();

    if (m_speedstack.front().GetComponent<IsPlayer>()->is_player) {
      battle_state.GetComponent<BattleState>()->phase = BP_PLAYER_TURN;
    }
    else {
      battle_state.GetComponent<BattleState>()->phase = BP_ENEMY_TURN;
    }
  }

  void BattleSystem::Update()
  {
    FlexECS::Entity battle_state = FlexECS::Scene::GetActiveScene()->View<BattleState>()[0];
    int battle_phase = battle_state.GetComponent<BattleState>()->phase;

    if (battle_phase == BP_PROCESSING) {
      UpdateSpeedStack();
    }
    else if (battle_phase == BP_PLAYER_TURN) {
      //Choose a Move
      PlayerMoveSelection();
    }
    else if (battle_phase == BP_ENEMY_TURN) {
      m_speedstack.front().GetComponent<Action>()->move_to_use = m_speedstack.front().GetComponent<MoveList>()->move_one;
      battle_state.GetComponent<BattleState>()->current_target_count = 1;
      battle_state.GetComponent<BattleState>()->target_one = 0;
      battle_state.GetComponent<BattleState>()->phase = BP_MOVE_EXECUTION;

    }
    else if (battle_phase == BP_MOVE_TARGET_SELECTION) {
      PlayerTargetSelection();
    }
    else if (battle_phase == BP_MOVE_EXECUTION) {
      ExecuteMove();
    }
  }

  void BattleSystem::PlayerMoveSelection()
  {
    FlexECS::Entity move_to_use = FlexECS::Entity::Null;
    for (auto& entity : FlexECS::Scene::GetActiveScene()->View<IsActive, MoveButton>()) {
      entity.GetComponent<IsActive>()->is_active = true;
    }
    //move selection system
    GetMoveSelection();
    for (auto& entity : FlexECS::Scene::GetActiveScene()->View<OnClick, MoveButton>())
    {
      auto& click_status = entity.GetComponent<OnClick>()->is_clicked;
      if (click_status == true)
      {
        switch (entity.GetComponent<MoveButton>()->move_number) {
        case 0:
          move_to_use = m_speedstack.front().GetComponent<MoveList>()->move_one;
          break;
        case 1:
          move_to_use = m_speedstack.front().GetComponent<MoveList>()->move_two;
          break;
        case 2:
          move_to_use = m_speedstack.front().GetComponent<MoveList>()->move_three;
          break;
        case 3:
          move_to_use = m_speedstack.front().GetComponent<MoveList>()->move_four;
          break;
        }
      }
    }

    if (move_to_use == FlexECS::Entity::Null) return;

    m_speedstack.front().GetComponent<Action>()->move_to_use = move_to_use;

    FlexECS::Entity battle_state = FlexECS::Scene::GetActiveScene()->View<BattleState>()[0];
    battle_state.GetComponent<BattleState>()->phase = BP_MOVE_TARGET_SELECTION;

    for (auto& entity : FlexECS::Scene::GetActiveScene()->View<IsActive, MoveButton>()) {
      entity.GetComponent<IsActive>()->is_active = false;
    }


    //printing info
    FlexECS::Entity player = battle_state.GetComponent<BattleState>()->active_character;
    FlexECS::Entity selected_move = player.GetComponent<Action>()->move_to_use;
    Move move = MoveRegistry::GetMove(selected_move.GetComponent<MoveID>()->move_name);
    std::cout << "Move Selected: " << move.name << std::endl;
  }

  void BattleSystem::PlayerTargetSelection()
  {
    FlexECS::Entity battle_state = FlexECS::Scene::GetActiveScene()->View<BattleState>()[0];
    auto& target_count = battle_state.GetComponent<BattleState>()->current_target_count;
    
    FlexECS::Entity player = battle_state.GetComponent<BattleState>()->active_character;
    FlexECS::Entity move_to_use = player.GetComponent<Action>()->move_to_use;
    Move move = MoveRegistry::GetMove(move_to_use.GetComponent<MoveID>()->move_name);

    if (target_count < move.target_count) {
      GetTargetSelection();
      
      for (auto& entity : FlexECS::Scene::GetActiveScene()->View<OnClick, BattleSlot>())
      {
        auto& click_status = entity.GetComponent<OnClick>()->is_clicked;
        if (click_status == true) 
        {
          int selection = entity.GetComponent<BattleSlot>()->slot_number;
          target_count += 1;
          switch (target_count) {
          case 1:
            battle_state.GetComponent<BattleState>()->target_one = selection;
            break;
          case 2:
            battle_state.GetComponent<BattleState>()->target_two = selection;
            break;
          case 3:
            battle_state.GetComponent<BattleState>()->target_three = selection;
            break;
          case 4:
            battle_state.GetComponent<BattleState>()->target_four = selection;
            break;
          case 5:
            battle_state.GetComponent<BattleState>()->target_five = selection;
            break;
          }
        }
      }
    }
    if (target_count >= move.target_count) {
      battle_state.GetComponent<BattleState>()->phase = BP_MOVE_EXECUTION;
    }
  }



  void BattleSystem::ExecuteMove()
  {
    //get the move user
    FlexECS::Entity battle_state = FlexECS::Scene::GetActiveScene()->View<BattleState>()[0];
    FlexECS::Entity user = battle_state.GetComponent<BattleState>()->active_character;
    FlexECS::Entity move_to_use = user.GetComponent<Action>()->move_to_use;
    Move move = MoveRegistry::GetMove(move_to_use.GetComponent<MoveID>()->move_name);

    std::vector<FlexECS::Entity> targets;
    
    if (move.target_type == MOVE_TARGET_SINGLE)
    {
      switch (move.target_count) {
      case 5:
        targets.insert(targets.begin(), static_cast<FlexECS::Entity>(m_slots[battle_state.GetComponent<BattleState>()->target_five].GetComponent<BattleSlot>()->character));
      case 4:
        targets.insert(targets.begin(), static_cast<FlexECS::Entity>(m_slots[battle_state.GetComponent<BattleState>()->target_four].GetComponent<BattleSlot>()->character));
      case 3:
        targets.insert(targets.begin(), static_cast<FlexECS::Entity>(m_slots[battle_state.GetComponent<BattleState>()->target_three].GetComponent<BattleSlot>()->character));
      case 2:
        targets.insert(targets.begin(), static_cast<FlexECS::Entity>(m_slots[battle_state.GetComponent<BattleState>()->target_two].GetComponent<BattleSlot>()->character));
      case 1:
        targets.insert(targets.begin(), static_cast<FlexECS::Entity>(m_slots[battle_state.GetComponent<BattleState>()->target_one].GetComponent<BattleSlot>()->character));
        break;
      }
    }
    else if (move.target_type == MOVE_TARGET_SELF) {
      targets.push_back(user);
    }
    else if (move.target_type == MOVE_TARGET_ALL_ENEMY) {
      for (FlexECS::Entity target : m_characters)
      {
        if (!target.GetComponent<IsPlayer>()->is_player)
          targets.push_back(target);
      }
    }
    else if (move.target_type == MOVE_TARGET_ALL_PLAYER) {
      for (FlexECS::Entity target : m_characters)
      {
        if (target.GetComponent<IsPlayer>()->is_player)
          targets.push_back(target);
      }
    }

    //execute move
    move.effect(user, targets);
    user.GetComponent<CharacterSpeed>()->current_speed += move.cost + user.GetComponent<CharacterSpeed>()->base_speed;
    user.GetComponent<Action>()->move_to_use = FlexECS::Entity::Null;
    m_speedstack.sort(SortLowestSpeed());
    
    //Display now info
    auto scene = FlexECS::Scene::GetActiveScene();
    for (auto& entity : m_speedstack)
    {
      std::cout << scene->Internal_StringStorage_Get(entity.GetComponent<CharacterName>()->character_name)
        << "  HP: " << entity.GetComponent<CharacterHealth>()->current_health
        << "  Spd: " << entity.GetComponent<CharacterSpeed>()->current_speed << std::endl;
    }
    std::cout << "\n";
    battle_state.GetComponent<BattleState>()->current_target_count = 0;
    battle_state.GetComponent<BattleState>()->active_character = FlexECS::Entity::Null;
    battle_state.GetComponent<BattleState>()->phase = BP_PROCESSING;
  }

  void BattleLayer::SetupBattle()
  {
    auto scene = FlexECS::Scene::GetActiveScene();

    //battle positions for characters
    m_battlesystem.InitializeBattleSlots();

    #pragma region Character Components
    FlexECS::Entity player1 = FlexECS::Scene::CreateEntity("Renkoooo");
    player1.AddComponent<CharacterName>({ scene->Internal_StringStorage_New("Renkoooo") });
    player1.AddComponent<IsPlayer>({ true });
    player1.AddComponent<CharacterHealth>({ 20, 20 });
    player1.AddComponent<CharacterSpeed>({ 5,5 });
    player1.AddComponent<Action>({});
    player1.AddComponent<MoveList>({});

    FlexECS::Entity player2 = FlexECS::Scene::CreateEntity("Cutemoew");
    player2.AddComponent<CharacterName>({ scene->Internal_StringStorage_New("Cutemoew") });
    player2.AddComponent<IsPlayer>({ true });
    player2.AddComponent<CharacterHealth>({ 10, 10 });
    player2.AddComponent<CharacterSpeed>({ 10,10 });
    player2.AddComponent<Action>({});
    player2.AddComponent<MoveList>({});

    FlexECS::Entity enemy1 = FlexECS::Scene::CreateEntity("Voidling");
    enemy1.AddComponent<CharacterName>({ scene->Internal_StringStorage_New("Voidling") });
    enemy1.AddComponent<IsPlayer>({ false });
    enemy1.AddComponent<CharacterHealth>({ 6, 6 });
    enemy1.AddComponent<CharacterSpeed>({ 7, 7 });
    enemy1.AddComponent<Action>({});
    enemy1.AddComponent<MoveList>({});

    FlexECS::Entity enemy2 = FlexECS::Scene::CreateEntity("Frogling");
    enemy2.AddComponent<CharacterName>({ scene->Internal_StringStorage_New("Frogling") });
    enemy2.AddComponent<IsPlayer>({ false });
    enemy2.AddComponent<CharacterHealth>({ 6, 6 });
    enemy2.AddComponent<CharacterSpeed>({ 8, 8 });
    enemy2.AddComponent<Action>({});
    enemy2.AddComponent<MoveList>({});

    FlexECS::Entity enemy3 = FlexECS::Scene::CreateEntity("Slimling");
    enemy3.AddComponent<CharacterName>({ scene->Internal_StringStorage_New("Slimling") });
    enemy3.AddComponent<IsPlayer>({ false });
    enemy3.AddComponent<CharacterHealth>({ 6, 6 });
    enemy3.AddComponent<CharacterSpeed>({ 9, 9 });
    enemy3.AddComponent<Action>({});
    enemy3.AddComponent<MoveList>({});

    m_battlesystem.AddCharacter(player1, 0);
    m_battlesystem.AddCharacter(player2, 1);
    m_battlesystem.AddCharacter(enemy1, 4);
    m_battlesystem.AddCharacter(enemy2, 5);
    m_battlesystem.AddCharacter(enemy3, 6);
    #pragma endregion

    #pragma region Moves

    //ADding moves
    static_cast<FlexECS::Entity>(player1.GetComponent<MoveList>()->move_one).GetComponent<MoveID>()->move_name = "Dual Strike";
    static_cast<FlexECS::Entity>(player1.GetComponent<MoveList>()->move_two).GetComponent<MoveID>()->move_name = "Whirlwind";
    static_cast<FlexECS::Entity>(player1.GetComponent<MoveList>()->move_three).GetComponent<MoveID>()->move_name = "Block";
    static_cast<FlexECS::Entity>(player1.GetComponent<MoveList>()->move_four).GetComponent<MoveID>()->move_name = "";
    //p2
    static_cast<FlexECS::Entity>(player2.GetComponent<MoveList>()->move_one).GetComponent<MoveID>()->move_name = "Rift Surge";
    static_cast<FlexECS::Entity>(player2.GetComponent<MoveList>()->move_two).GetComponent<MoveID>()->move_name = "Flashbang";
    static_cast<FlexECS::Entity>(player2.GetComponent<MoveList>()->move_three).GetComponent<MoveID>()->move_name = "Temporal Wave";
    static_cast<FlexECS::Entity>(player2.GetComponent<MoveList>()->move_four).GetComponent<MoveID>()->move_name = "";

    //enemeies
    static_cast<FlexECS::Entity>(enemy1.GetComponent<MoveList>()->move_one).GetComponent<MoveID>()->move_name = "Poke";
    static_cast<FlexECS::Entity>(enemy1.GetComponent<MoveList>()->move_two).GetComponent<MoveID>()->move_name = "Poke";
    static_cast<FlexECS::Entity>(enemy1.GetComponent<MoveList>()->move_three).GetComponent<MoveID>()->move_name = "Poke";
    static_cast<FlexECS::Entity>(enemy1.GetComponent<MoveList>()->move_four).GetComponent<MoveID>()->move_name = "";

    static_cast<FlexECS::Entity>(enemy2.GetComponent<MoveList>()->move_one).GetComponent<MoveID>()->move_name = "Poke";
    static_cast<FlexECS::Entity>(enemy2.GetComponent<MoveList>()->move_two).GetComponent<MoveID>()->move_name = "Poke";
    static_cast<FlexECS::Entity>(enemy2.GetComponent<MoveList>()->move_three).GetComponent<MoveID>()->move_name = "Poke";
    static_cast<FlexECS::Entity>(enemy2.GetComponent<MoveList>()->move_four).GetComponent<MoveID>()->move_name = "";

    static_cast<FlexECS::Entity>(enemy3.GetComponent<MoveList>()->move_one).GetComponent<MoveID>()->move_name = "Poke";
    static_cast<FlexECS::Entity>(enemy3.GetComponent<MoveList>()->move_two).GetComponent<MoveID>()->move_name = "Poke";
    static_cast<FlexECS::Entity>(enemy3.GetComponent<MoveList>()->move_three).GetComponent<MoveID>()->move_name = "Poke";
    static_cast<FlexECS::Entity>(enemy3.GetComponent<MoveList>()->move_four).GetComponent<MoveID>()->move_name = "";
    #pragma endregion

    #pragma region Rendering Components
    //Rendering related components
    player1.AddComponent<IsActive>({ true });
    player1.AddComponent<Position>({ {200, 600} });
    player1.AddComponent<Scale>({ { 100,100 } });
    player1.AddComponent<ZIndex>({ 10 });
    player1.AddComponent<Sprite>({
      scene->Internal_StringStorage_New(R"(\images\chess_queen.png)"),
      Vector3::One,
      Vector3::Zero,
      Vector3::One,
      Renderer2DProps::Alignment_Center
     });
    player1.AddComponent<Shader>({ scene->Internal_StringStorage_New(R"(\shaders\texture)") });

    player2.AddComponent<IsActive>({ true });
    player2.AddComponent<Position>({ {320, 600} });
    player2.AddComponent<Scale>({ { 100,100 } });
    player2.AddComponent<ZIndex>({ 10 });
    player2.AddComponent<Sprite>({
      scene->Internal_StringStorage_New(R"(\images\chess_king.png)"),
      Vector3::One,
      Vector3::Zero,
      Vector3::One,
      Renderer2DProps::Alignment_Center
     });
    player2.AddComponent<Shader>({ scene->Internal_StringStorage_New(R"(\shaders\texture)") });

    enemy1.AddComponent<IsActive>({ true });
    enemy1.AddComponent<Position>({ {700, 200} });
    enemy1.AddComponent<Scale>({ { 100,100 } });
    enemy1.AddComponent<ZIndex>({ 10 });
    enemy1.AddComponent<Sprite>({
      scene->Internal_StringStorage_New(R"(\images\chess_knight.png)"),
      Vector3::One,
      Vector3::Zero,
      Vector3::Zero,
      Renderer2DProps::Alignment_Center
     });
    enemy1.AddComponent<Shader>({ scene->Internal_StringStorage_New(R"(\shaders\texture)") });

    enemy2.AddComponent<IsActive>({ true });
    enemy2.AddComponent<Position>({ {820, 200} });
    enemy2.AddComponent<Scale>({ { 100,100 } });
    enemy2.AddComponent<ZIndex>({ 10 });
    enemy2.AddComponent<Sprite>({
      scene->Internal_StringStorage_New(R"(\images\chess_bishop.png)"),
      Vector3::One,
      Vector3::Zero,
      Vector3::Zero,
      Renderer2DProps::Alignment_Center
     });
    enemy2.AddComponent<Shader>({ scene->Internal_StringStorage_New(R"(\shaders\texture)") });

    enemy3.AddComponent<IsActive>({ true });
    enemy3.AddComponent<Position>({ {940, 200} });
    enemy3.AddComponent<Scale>({ { 100,100 } });
    enemy3.AddComponent<ZIndex>({ 10 });
    enemy3.AddComponent<Sprite>({
      scene->Internal_StringStorage_New(R"(\images\chess_pawn.png)"),
      Vector3::One,
      Vector3::Zero,
      Vector3::Zero,
      Renderer2DProps::Alignment_Center
     });
    enemy3.AddComponent<Shader>({ scene->Internal_StringStorage_New(R"(\shaders\texture)") });
    #pragma endregion

    #pragma region Turn Order Display

    for (size_t i = 0; i < m_battlesystem.GetCharacterCount(); i++)
    {
      FlexECS::Entity display_slot = FlexECS::Scene::CreateEntity();
      display_slot.AddComponent<TurnOrderDisplay>({});
      display_slot.AddComponent<IsActive>({ false });
      display_slot.AddComponent<Position>({ });
      display_slot.AddComponent<Scale>({ { 60,60 } });
      display_slot.AddComponent<ZIndex>({ 10 });
      display_slot.AddComponent<Sprite>({});
      display_slot.AddComponent<Shader>({ scene->Internal_StringStorage_New(R"(\shaders\texture)") });
    }

    #pragma endregion





    MoveRegistry::RegisterMoves();

    FlexECS::Entity battle_state = FlexECS::Scene::CreateEntity("BattleState");
    battle_state.AddComponent<BattleState>({});

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
  }
}

#if 0

namespace ChronoShift
{
  //declaration of statics
  std::unordered_map<int, Move> Move::move_index;

  //Sorting functions
  struct SortLowestSpeed 
  {
    bool operator()(FlexECS::Entity e1, FlexECS::Entity e2){
      return e1.GetComponent<CharacterSpeed>()->current_speed < e2.GetComponent<CharacterSpeed>()->current_speed;
    }
  };


  Move Move::GetMoveFromID(int move_id)
  {
    //Todo: check for valid move
    return Move::move_index[move_id];
  }

  BattleSystem::BattleSystem() {

  }
  BattleSystem::~BattleSystem() {

  }
  void BattleSystem::AddCharacter(FlexECS::Entity character) {
    m_characters.push_back(character);
    //find correct position to place the character
  }

  void BattleSystem::BeginBattle()
  {
    auto scene = FlexECS::Scene::GetActiveScene();
    for (auto& entity : scene->View<CharacterHealth>()) 
    {
      std::cout << entity.GetComponent<CharacterHealth>()->health << std::endl;
      AddCharacter(entity);
      m_speedstack.push_back(entity);
    }
    m_speedstack.sort(SortLowestSpeed());

    std::cout << "Beginning Stack Order:\nName / HP / Speed \n";
    for (auto& entity : m_speedstack)
    {
      std::cout << scene->Internal_StringStorage_Get(entity.GetComponent<CharacterName>()->character_name)
                << "  HP: " << entity.GetComponent<CharacterHealth>()->health
                << "  Spd: " << entity.GetComponent<CharacterSpeed>()->current_speed << std::endl;
    }
    std::cout << "\n";

    m_battle_state = BS_PROCESSING;
  }

  void BattleSystem::Process()
  {
    auto scene = FlexECS::Scene::GetActiveScene();
    int speed_to_decrease = m_speedstack.front().GetComponent<CharacterSpeed>()->current_speed;
    for (auto& entity : m_speedstack)
    {
      entity.GetComponent<CharacterSpeed>()->current_speed -= speed_to_decrease;
      std::cout << scene->Internal_StringStorage_Get(entity.GetComponent<CharacterName>()->character_name)
        << "  HP: " << entity.GetComponent<CharacterHealth>()->health
        << "  Spd: " << entity.GetComponent<CharacterSpeed>()->current_speed << std::endl;
    }
    std::cout << "\n";

    //Now is either player of enemy's turn to move
    auto entity_to_move = m_speedstack.front();
    std::cout << "It is " << scene->Internal_StringStorage_Get(entity_to_move.GetComponent<CharacterName>()->character_name) << " turn to move\n";

    if (entity_to_move.GetComponent<IsPlayer>()->is_player) {
      m_battle_state = BS_PLAYER_TURN;
    }
    else {
      m_battle_state = BS_ENEMY_TURN;
      entity_to_move.GetComponent<Action>()->id = 1;
      entity_to_move.GetComponent<Action>()->target = m_characters[0];
    }
  }

  void BattleSystem::WaitForMove()
  {
    //The unit to move is confirmed to be at the front of the list.
    auto entity = m_speedstack.front();

    //Display List of available moves from MoveList
    //entity.GetComponent<MoveList>();

    //Select the move
    //Select target
    if (Input::GetKeyDown(GLFW_KEY_1)) {
      auto enemy1 = m_characters[2];
      auto enemy2 = m_characters[3];
      entity.GetComponent<Action>()->id = 1;
      entity.GetComponent<Action>()->target = enemy1;
      ExecuteMove();
    }
    if (Input::GetKeyDown(GLFW_KEY_2)) {
      auto enemy1 = m_characters[2];
      auto enemy2 = m_characters[3];
      entity.GetComponent<Action>()->id = 2;
      entity.GetComponent<Action>()->target = enemy2;
      ExecuteMove();
    }
  }

  void BattleSystem::ExecuteMove()
  {
    auto entity = m_speedstack.front();
    //Find move from map of moves
    Move move = Move::GetMoveFromID(entity.GetComponent<Action>()->id);
    move.effect(entity, { entity.GetComponent<Action>()->target }); //execute move
    
    //After using move, set action to unused and recalulate speed
    entity.GetComponent<Action>()->id = -1;
    entity.GetComponent<Action>()->target = -1;
    entity.GetComponent<CharacterSpeed>()->current_speed += entity.GetComponent<CharacterSpeed>()->base_speed + move.cost;
    
    //Display info
    auto scene = FlexECS::Scene::GetActiveScene();
    std::cout << scene->Internal_StringStorage_Get(entity.GetComponent<CharacterName>()->character_name) << " used move: " << move.name << "!\n";
    m_speedstack.sort(SortLowestSpeed());
    for (auto& entity : m_speedstack)
    {
      std::cout << scene->Internal_StringStorage_Get(entity.GetComponent<CharacterName>()->character_name)
        << "  HP: " << entity.GetComponent<CharacterHealth>()->health
        << "  Spd: " << entity.GetComponent<CharacterSpeed>()->current_speed << std::endl;
    }
    std::cout << "\n";

    m_battle_state = BS_PROCESSING;
  }

  void BattleLayer::SetupBattle()
  {
    auto scene = FlexECS::Scene::GetActiveScene();
    //Window* window = Application::GetCurrentWindow();

    #pragma region Registering Moves
    Move move_dual_strike = {"Dual Strike", "Shink Shank", 1, 4, 
      [](FlexECS::Entity& user, std::vector<FlexECS::Entity> targets) {
        for (auto entity : targets)
        {
          entity.GetComponent<CharacterHealth>()->health -= 2;
        }
      }
    };
    Move move_whirlwind = { "Whirlwind", "SWoowoons", 2, 7,
      [](FlexECS::Entity& user, std::vector<FlexECS::Entity> targets) {
        for (auto entity : targets)
        {
          entity.GetComponent<CharacterHealth>()->health -= 2;
        }
      }
    };

    Move::move_index.insert({ move_dual_strike.id, move_dual_strike });
    Move::move_index.insert({ move_whirlwind.id, move_whirlwind });

    #pragma endregion

    #pragma region Adding Components
    // create entities
    FlexECS::Entity player1 = FlexECS::Scene::CreateEntity("player1");
    player1.AddComponent<IsPlayer>({ true });
    player1.AddComponent<CharacterName>({ scene->Internal_StringStorage_New("Renkoooo") });
    player1.AddComponent<CharacterHealth>({ 20 });
    player1.AddComponent<CharacterSpeed>({ 5,5 });
    player1.AddComponent<Action>({});
    //player1.AddComponent<MoveList>({ std::vector<int>{1,2,3} });
    //player1.AddComponent<MoveList>({ {1,2,3,4} });
    //player1.AddComponent<MoveList>({ });

    //player1.AddComponent<ActionList>({});
    //player1.GetComponent<ActionList>()->action_list.push_back(1);
    //player1.GetComponent<ActionList>()->action_list.emplace_back(action_dual_strike);
    //player1.GetComponent<ActionList>()->action_list.push_back(action_block);

    FlexECS::Entity player2 = FlexECS::Scene::CreateEntity("player2");
    player2.AddComponent<IsPlayer>({ true });
    player2.AddComponent<CharacterName>({ scene->Internal_StringStorage_New("Cutemoew") });
    player2.AddComponent<CharacterHealth>({ 12 });
    player2.AddComponent<CharacterSpeed>({ 15,15 });
    player2.AddComponent<Action>({});
    //player2.AddComponent<ActionList>({});
    //player2.GetComponent<ActionList>()->action_list.emplace_back(action_moew);
    //player2.GetComponent<ActionList>()->action_list.emplace_back(action_gyude);

    FlexECS::Entity enemy1 = FlexECS::Scene::CreateEntity("enemy1");
    enemy1.AddComponent<IsPlayer>({ false });
    enemy1.AddComponent<CharacterName>({ scene->Internal_StringStorage_New("Voidling") });
    enemy1.AddComponent<CharacterHealth>({ 5 });
    enemy1.AddComponent<CharacterSpeed>({ 10,10 });
    enemy1.AddComponent<Action>({});
    //enemy1.AddComponent<ActionList>({});
    //enemy1.GetComponent<ActionList>()->action_list.emplace_back(action_slash);

    FlexECS::Entity enemy2 = FlexECS::Scene::CreateEntity("enemy2");
    enemy2.AddComponent<IsPlayer>({ false });
    enemy2.AddComponent<CharacterName>({ scene->Internal_StringStorage_New("Frogling") });
    enemy2.AddComponent<CharacterHealth>({ 8 });
    enemy2.AddComponent<CharacterSpeed>({ 12,12 });
    enemy2.AddComponent<Action>({});
    //enemy2.AddComponent<ActionList>({});
    //enemy2.GetComponent<ActionList>()->action_list.emplace_back(action_lick);


    player1.AddComponent<IsActive>({ true });
    player1.AddComponent<Position>({ {100, 500} });
    player1.AddComponent<Scale>({ { 100,100 } });
    player1.AddComponent<ZIndex>({ 10 });
    player1.AddComponent<Sprite>({
      scene->Internal_StringStorage_New(R"(\images\chess_queen.png)"),
      Vector3::One,
      Vector3::Zero,
      Vector3::One,
      Renderer2DProps::Alignment_Center
     });
    player1.AddComponent<Shader>({ scene->Internal_StringStorage_New(R"(\shaders\texture)") });
    
    player2.AddComponent<IsActive>({ true });
    player2.AddComponent<Position>({ {200, 500} });
    player2.AddComponent<Scale>({ { 100,100 } });
    player2.AddComponent<ZIndex>({ 10 });
    player2.AddComponent<Sprite>({
      scene->Internal_StringStorage_New(R"(\images\chess_king.png)"),
      Vector3::One,
      Vector3::Zero,
      Vector3::One,
      Renderer2DProps::Alignment_Center
     });
    player2.AddComponent<Shader>({ scene->Internal_StringStorage_New(R"(\shaders\texture)") });

    enemy1.AddComponent<IsActive>({ true });
    enemy1.AddComponent<Position>({ {500, 200} });
    enemy1.AddComponent<Scale>({ { 100,100 } });
    enemy1.AddComponent<ZIndex>({ 10 });
    enemy1.AddComponent<Sprite>({
      scene->Internal_StringStorage_New(R"(\images\chess_pawn.png)"),
      Vector3::One,
      Vector3::Zero,
      Vector3::One,
      Renderer2DProps::Alignment_Center
     });
    enemy1.AddComponent<Shader>({ scene->Internal_StringStorage_New(R"(\shaders\texture)") });

    enemy2.AddComponent<IsActive>({ true });
    enemy2.AddComponent<Position>({ {600, 200} });
    enemy2.AddComponent<Scale>({ { 100,100 } });
    enemy2.AddComponent<ZIndex>({ 10 });
    enemy2.AddComponent<Sprite>({
      scene->Internal_StringStorage_New(R"(\images\chess_pawn.png)"),
      Vector3::One,
      Vector3::Zero,
      Vector3::One,
      Renderer2DProps::Alignment_Center
     });
    enemy2.AddComponent<Shader>({ scene->Internal_StringStorage_New(R"(\shaders\texture)") });
    #pragma endregion

    //std::cout << std::endl << "Player1 test:\n";
    //std::cout << "Size: " << player1.GetComponent<MoveList>()->move_list.size() << std::endl;
    //for (int i : player1.GetComponent<MoveList>()->move_list)
    //{
    //  std::cout << i << std::endl;
    //}
    //player1.GetComponent<MoveList>()->move_list[1] = 500;
    //    for (int i : player1.GetComponent<MoveList>()->move_list)
    //{
    //  std::cout << i << std::endl;
    //}
    //player1.GetComponent<MoveList>()->move_list.push_back(30);

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
    if (Input::GetKeyDown(GLFW_KEY_A) && m_battlesystem.GetState() == BS_PROCESSING) {
      std::cout << "Progressing Battle\n";
      m_battlesystem.Process();
    }
    if (m_battlesystem.GetState() == BS_PLAYER_TURN) {
      m_battlesystem.WaitForMove();
    }
    if (m_battlesystem.GetState() == BS_ENEMY_TURN) {
      m_battlesystem.ExecuteMove();
    }
    if (m_battlesystem.GetState() == BS_PROCESSING) {
      //Wait until finish animation or somehting

    }

    RendererSprite2D();
  }



}
#endif