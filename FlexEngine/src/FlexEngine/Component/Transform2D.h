#pragma once

#include "Core/ecs.h"
#include "Reflection/base.h"
#include "FlexMath/vector2.h"

namespace FlexEngine
{
  
  class Transform
  { FLX_REFL_ECS_REGISTER(Transform)
  public:
    std::string ToString() const
    {
      return "Transform" + position.ToString();
    }

    // getter

    const Vector2& GetPosition() const { return position; }

    // setter

    void SetPosition(const Vector2& value) { position = value; }

  private:
    Vector2 position;
  };

}
