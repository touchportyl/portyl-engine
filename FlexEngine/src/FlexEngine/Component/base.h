#pragma once

#include "Reflection/base.h"

namespace FlexEngine
{

  class Component
  { FLX_REFL_SERIALIZABLE FLX_REFL_SERIALIZE_VIRTUAL

  public:
    virtual ~Component() = default;
  };

  // Component Map (Unordered)
  using ComponentMap = std::unordered_map<std::type_index, std::shared_ptr<Component>>;

}
