#pragma once

#include <FlexEngine.h>

namespace FlexEditor
{

  class SplashState : public FlexEngine::State
  {
    std::shared_ptr<FlexEngine::Window> window;

  public:
    SplashState() = default;
    ~SplashState() = default;

    void OnEnter() override;
    void Update() override;
    void OnExit() override;
  };

}