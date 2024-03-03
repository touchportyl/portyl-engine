#pragma once

#include "Component/base.h"

namespace FlexEngine
{

  class Transform : public Component
  { FLX_REFL_SERIALIZABLE
  public:
    Transform() = default;
    ~Transform() = default;

  private:
    float x = 0;
    float y = 0;
    float z = 0;

    FLX_REFL_REGISTER_START
      FLX_REFL_REGISTER_PROPERTY(x)
      FLX_REFL_REGISTER_PROPERTY(y)
      FLX_REFL_REGISTER_PROPERTY(z)
    FLX_REFL_REGISTER_END_AND_LINK(Transform)
  };

}
