#pragma once

#include "Component/base.h"

namespace FlexEngine
{

  class Transform : public Component
  { FLX_REFL_SERIALIZABLE FLX_REFL_SERIALIZE_OVERRIDE
  public:
    Transform() = default;
    ~Transform() = default;

  private:
    float x = 0;

    FLX_REFL_REGISTER
    {
      FLX_REFL_REGISTER_PROPERTY(x)
    };
  };

}