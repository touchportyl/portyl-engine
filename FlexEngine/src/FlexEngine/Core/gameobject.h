#pragma once

#include "Reflection/base.h"
#include "Component/base.h"

namespace FlexEngine
{

  class GameObject
  { FLX_REFL_SERIALIZABLE FLX_REFL_SERIALIZE
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

    std::unordered_map<std::type_index, std::shared_ptr<Component>> components;

    friend class PropertyBase;
    friend class PropertyMap;

  public:
    FlexEngine::PropertyMap properties =
    {
      {
        "components",
        std::make_shared<
          FlexEngine::Property<
            decltype(components),
            std::function<decltype(components)()>,
            std::function<void(decltype(components))>
          >(&components)
        >()
      }
    };
  };

}