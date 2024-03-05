#pragma once

#include "Component/base.h"

namespace FlexEngine
{
  
  class Transform : public Component
  { FLX_REFL_SERIALIZABLE
  public:
    Transform() = default;
    ~Transform() = default;

    Transform(float x, float y, float z) : x(x), y(y), z(z) {}

  private:
    float x = 0;
    float y = 0;
    float z = 0;
  };

}
