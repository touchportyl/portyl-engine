#pragma once

#include "property.h"
#include "propertyreference.h"

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
      : m_property_list(init) {}

    /// <summary>
    /// Get a copy to a property.
    /// <para>It is read-only, use SetProperty to modify the property.</para>
    /// </summary>
    template <typename T>
    T GetProperty(const std::string& name)
    {
      auto it = m_property_list.find(name);
      if (it != m_property_list.end())
      {
        T temp{};
        it->second->GetValueFromAny(&temp);
        return temp;
      }
      return T();
    }

    /// <summary>
    /// Get the reference to the property.
    /// </summary>
    template <typename T>
    T* GetPropertyReference(const std::string& name)
    {
      auto it = m_property_list.find(name);
      if (it != m_property_list.end())
      {
        return it->second->GetReferenceFromAny(temp);
      }
      return nullptr;
    }
    
    /// <summary>
    /// Set a property.
    /// <para>It is good practice to specify the type of the property explicitly.</para>
    /// </summary>
    template <typename T>
    void SetProperty(const std::string& name, const T& value)
    {
      auto it = m_property_list.find(name);
      if (it != m_property_list.end())
      {
        it->second->SetValueFromAny(value);
      }
    }

  private:
    std::unordered_map<std::string, std::shared_ptr<PropertyBase>> m_property_list;
  };

}