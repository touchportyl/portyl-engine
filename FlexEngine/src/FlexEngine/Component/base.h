#pragma once

#include "Reflection/base.h"

namespace FlexEngine
{

  class Component
  { FLX_REFL_SERIALIZABLE FLX_REFL_SERIALIZE_VIRTUAL

  public:
    virtual ~Component() = default;
  };

}