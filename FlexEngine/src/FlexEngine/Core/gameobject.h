#pragma once

//#include "Reflection/base.h"
#include "Component/base.h"

namespace FlexEngine
{

  class GameObject
  { FLX_REFL_SERIALIZABLE
  public:
    template <typename T>
    T* Internal_AddComponent()
    {
      Reflection::TypeDescriptor* type_desc = Reflection::TypeResolver<T>::Get();
      components[type_desc->name] = std::make_shared<T>();
      return dynamic_cast<T*>(components[type_desc->name].get());
    }

    //template <typename T>
    //T* Internal_AddComponent()
    //{
    //  auto type = std::type_index(typeid(T));
    //  components[type] = std::make_shared<T>();
    //  return dynamic_cast<T*>(components[type].get());
    //}

    //template <typename T>
    //T* GetComponent()
    //{
    //  auto type = std::type_index(typeid(T));
    //  auto it = components.find(type);
    //  if (it != components.end())
    //  {
    //    return dynamic_cast<T*>(it->second.get());
    //  }
    //  return nullptr;
    //}

    ComponentMap components;

  private:
    // create a functor
    struct Functor_AddComponent
    {
      ComponentMap& m_components;
      template <typename T>
      T* Invoke() const
      {
        return Internal_AddComponent<T>(m_components);
      }
    };
  public:
    // get functor to AddComponent
    Functor_AddComponent AddComponent{ components };
  };

  // GameObject Map (Unordered)
  using GameObjectMap = std::unordered_map<std::string, std::shared_ptr<GameObject>>;

}
