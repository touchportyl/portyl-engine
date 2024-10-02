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
    m_battlesystem.InitializeBattleSlots();
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
    scene->Internal_StringStorage_New("White Queen");
    scene->Internal_StringStorage_New("White King");
    scene->Internal_StringStorage_New("Black Knight");
    scene->Internal_StringStorage_New("Black Bishop");
    scene->Internal_StringStorage_New("Black Pawn");

    scene->Internal_StringStorage_New("Enemy Weapon");
    scene->Internal_StringStorage_New("Dummy Weapon");
    scene->Internal_StringStorage_New("Poke");

    scene->Internal_StringStorage_New("Plasma Blade");
    scene->Internal_StringStorage_New("Blade made of plasma");
    scene->Internal_StringStorage_New("Dual Strike");
    scene->Internal_StringStorage_New("Whirlwind");
    scene->Internal_StringStorage_New("Block");

    scene->Internal_StringStorage_New("Electric Railgun");
    scene->Internal_StringStorage_New("Railgun that uses electromagnetic force");
    scene->Internal_StringStorage_New("Rift Surge");
    scene->Internal_StringStorage_New("Flashbang");
    scene->Internal_StringStorage_New("Temporal Wave");

    scene->Internal_StringStorage_New("Demo Chrono Gear");
    scene->Internal_StringStorage_New("Revives a dead drifter");
    scene->Internal_StringStorage_New("Instant Win");

    FlexECS::Scene::StringIndex shdr_path = scene->Internal_StringStorage_New(R"(\shaders\texture)");
    scene->Internal_StringStorage_New(R"(\images\chess_queen.png)");
    scene->Internal_StringStorage_New(R"(\images\chess_king.png)");
    scene->Internal_StringStorage_New(R"(\images\chess_knight.png)");
    scene->Internal_StringStorage_New(R"(\images\chess_bishop.png)");
    scene->Internal_StringStorage_New(R"(\images\chess_pawn.png)");
    
    FlexECS::Entity player1 = LoadCharacter("White Queen");
    FlexECS::Entity player2 = LoadCharacter("White King");
    FlexECS::Entity enemy1 = LoadCharacter("Black Knight");
    FlexECS::Entity enemy2 = LoadCharacter("Black Bishop");
    FlexECS::Entity enemy3 = LoadCharacter("Black Pawn");

    m_battlesystem.AddCharacter(player1, 0);
    m_battlesystem.AddCharacter(player2, 1);
    m_battlesystem.AddCharacter(enemy1, 4);
    m_battlesystem.AddCharacter(enemy2, 5);
    m_battlesystem.AddCharacter(enemy3, 6);
    
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
      display_slot.AddComponent<Rotation>({});
      display_slot.AddComponent<Shader>({ shdr_path });
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
    
    if (Input::GetKeyDown(GLFW_KEY_S)) {
      SaveCharacters();
    }
    else if (Input::GetKeyDown(GLFW_KEY_R)) {
      ResetCharacters();
    }
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