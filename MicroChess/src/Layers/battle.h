#pragma once
#include <FlexEngine.h>

#include "CharacterMoves/moves.h"
using namespace FlexEngine;

namespace ChronoShift
{



  class BattleSystem {
  public:
    BattleSystem();
    ~BattleSystem();

    void InitializeBattleSlots();
    void SetMovesForCharacter(FlexECS::Entity character);
    void AddCharacter(FlexECS::Entity character, int position);
    
    void BeginBattle();
    void Update();

    void UpdateSpeedStack();
    void PlayerMoveSelection();
    void PlayerTargetSelection();
    void ExecuteMove();

    size_t GetCharacterCount() { return m_characters.size(); };
    std::list<FlexECS::Entity>& GetTurnOrder() { return m_speedstack; };
  private:
    std::vector<FlexECS::Entity> m_characters;
    std::list<FlexECS::Entity> m_speedstack;
    std::array<FlexECS::Entity, 9> m_slots; //player takes up first 4, enemy last 5

    //BattleState m_battle_state;
  };

  class BattleLayer : public FlexEngine::Layer
  {
  public:
    BattleLayer() : Layer("Battle Layer") {}
    ~BattleLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void Update() override;

  private:
    void SetupBattle();

    BattleSystem m_battlesystem;
  };



}
