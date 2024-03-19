#pragma once

#include "Reflection/base.h"

#include <unordered_map>

namespace FlexEngine
{

  class Component
  { //FLX_REFL_SERIALIZABLE
  public:
    virtual ~Component() = default;
    std::shared_ptr<Component> derived = nullptr;
  };

  // Component Map (Unordered)
  using ComponentMap = std::unordered_map<std::string, std::shared_ptr<Component>>;

}
