/* Start Header
*****************************************************************/
/*!
WLVERSE [https://wlverse.web.app]
\file      overworld.h
\author    [100%] Soh Wei Jie (weijie.soh\@digipen.edu)
           - Camera code, add registering
\date      24 November 2024
\brief     Declaration for the overworld layer.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/* End Header
*******************************************************************/

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
