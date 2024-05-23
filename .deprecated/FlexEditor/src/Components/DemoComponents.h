#pragma once

#include <FlexEngine.h>
using namespace FlexEngine;

namespace FlexEditor
{

  // ECS Components
  // This is a simple example.
  class IsAlive
  { FLX_REFL_SERIALIZABLE
  public:
    bool value;
  };

  class Amount
  { FLX_REFL_SERIALIZABLE
  public:
    int value;
  };
  
  class Health
  { FLX_REFL_SERIALIZABLE
  public:
    float value;
  };
  
  class Transform
  { FLX_REFL_SERIALIZABLE
  public:
    float x;
    float y;
    float z;
  };

}