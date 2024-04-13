#pragma once

namespace FlexEngine
{

  // abstract class for states
  class State
  {
  public:
    virtual ~State() = default;
    virtual void OnEnter() = 0;
    virtual void Update() = 0;
    virtual void OnExit() = 0;
  };

}
