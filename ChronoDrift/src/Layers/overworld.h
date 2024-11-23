#pragma once
#include <FlexEngine.h>

#include "BattleSystems/battlesystem.h"
#include "CharacterMoves/moves.h"
using namespace FlexEngine;

namespace ChronoDrift
{
  class OverworldLayer : public FlexEngine::Layer
  {
  public:
    OverworldLayer() = delete;
    OverworldLayer(CameraManager* CamManager) : Layer("Battle Layer") , m_CamM_Instance(CamManager){}
    ~OverworldLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void Update() override;

  private:
      void SetupWorld();

      CameraManager* m_CamM_Instance = nullptr;
  };



}
