#pragma once

#include "property.h"

#include <unordered_map>

namespace FlexEngine
{

  /// <summary>
  /// Property map.
  /// <para>Map of registered properties with string keys.</para>
  /// <para>Used for reflection and serialization.</para>
  /// </summary>
  class PropertyMap
  {
  public:
    PropertyMap(std::initializer_list<std::pair<const std::string, std::shared_ptr<PropertyBase>>> init)
      : properties(init) {}

    template <typename T>
    T GetProperty(const std::string& name)
    {
      auto it = properties.find(name);
      if (it != properties.end())
      {
        T temp{};
        it->second->GetValueFromAny(&temp);
        return temp;
      }
      return T();
    }
    
    template <typename T>
    void SetProperty(const std::string& name, T value)
    {
      auto it = properties.find(name);
      if (it != properties.end())
      {
        it->second->SetValueFromAny(&value);
      }
    }

  private:
    std::unordered_map<std::string, std::shared_ptr<PropertyBase>> properties;
  };

}