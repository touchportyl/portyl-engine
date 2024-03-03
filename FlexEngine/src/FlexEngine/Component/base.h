#pragma once

#include "Reflection/base.h"

namespace FlexEngine
{

  class Component
  { FLX_REFL_SERIALIZABLE

  public:
    virtual ~Component() = default;
  };

  // Component Map (Unordered)
  using ComponentMap = std::unordered_map<std::type_index, std::shared_ptr<Component>>;

}
