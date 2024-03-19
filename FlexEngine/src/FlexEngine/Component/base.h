#pragma once

#include "Reflection/base.h"

#include <unordered_map>

namespace FlexEngine
{

  class Component
  {
  public:
    virtual ~Component() = default;
  };

  // Component Map (Unordered)
  using ComponentMap = std::unordered_map<std::string, std::shared_ptr<Component>>;

}
