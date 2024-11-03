#pragma once

#include <FlexEngine.h>
using namespace FlexEngine;

namespace ChronoShift
{
  class CharacterInput
  {
    FLX_REFL_SERIALIZABLE
  public:
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
  };
}
