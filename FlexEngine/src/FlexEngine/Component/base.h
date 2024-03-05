#pragma once

#include "Reflection/base.h"

#include <unordered_map>
#include <typeindex>

namespace FlexEngine
{

  class Component
  {
  public:
    virtual ~Component() = default;
  };

  // Component Map (Unordered)
  using ComponentMap = std::unordered_map<std::type_index, std::shared_ptr<Component>>;

}
