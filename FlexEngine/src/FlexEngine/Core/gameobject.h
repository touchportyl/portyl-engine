#pragma once

#include "Reflection/base.h"
#include "Component/base.h"

namespace FlexEngine
{

  class GameObject
  { FLX_REFL_SERIALIZABLE
  //public:
  //  void Serialize(std::ostream& stream) const { properties.Serialize(stream); }
  public:
    template <typename T>
    T* AddComponent()
    {
      auto type = std::type_index(typeid(T));
      components[type] = std::make_shared<T>();
      return dynamic_cast<T*>(components[type].get());
    }

    template <typename T>
    T* GetComponent()
    {
      auto type = std::type_index(typeid(T));
      auto it = components.find(type);
      if (it != components.end()) {
        return dynamic_cast<T*>(it->second.get());
      }
      return nullptr;
    }

    ComponentMap components;

    FLX_REFL_REGISTER_START
      FLX_REFL_REGISTER_REFERENCE(components)
    FLX_REFL_REGISTER_END_AND_LINK(GameObject)
  };

  // GameObject Map (Unordered)
  using GameObjectMap = std::unordered_map<std::string, std::shared_ptr<GameObject>>;

}
