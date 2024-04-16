#pragma once

#include "Core/ecs.h"
#include "Reflection/base.h"

namespace FlexEngine
{
  
  class Transform
  { FLX_REFL_ECS_REGISTER(Transform)
  public:
    std::string ToString() const
    {
      return "Transform(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
    }

    // getter

    float GetX() const { return x; }
    float GetY() const { return y; }
    float GetZ() const { return z; }

    // setter

    void SetX(float value) { x = value; }
    void SetY(float value) { y = value; }
    void SetZ(float value) { z = value; }

  private:
    float x = 0;
    float y = 0;
    float z = 0;
  };

}
