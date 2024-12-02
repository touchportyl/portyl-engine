// WLVERSE [https://wlverse.web.app]
// main.cpp
// 
// Main entry point pattern.
//
// AUTHORS
// [100%] Chan Wen Loong (wenloong.c\@digipen.edu)
//   - Main Author
// 
// Copyright (c) 2024 DigiPen, All rights reserved.

#include <FlexEngine.h>
#include <FlexEngine/Core/EntryPoint.h>

#include "States.h"

namespace ChronoShift
{
  class FlexApplication : public FlexEngine::Application
  {
  public:
    FlexApplication()
    {
      FlexEngine::ApplicationStateManager::SetState<MainState>();
    }
  };

}

namespace FlexEngine
{
  Application* CreateApplication()
  {
    return new ChronoShift::FlexApplication();
  }
}
