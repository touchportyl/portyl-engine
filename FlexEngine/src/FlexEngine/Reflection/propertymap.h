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
    PropertyMap(
      std::initializer_list<std::pair<const std::string, std::shared_ptr<PropertyBase>>> init,
      std::string name
    )
      : name(name), m_property_list(init) {}

    PropertyMap(std::initializer_list<std::pair<const std::string, std::shared_ptr<PropertyBase>>> init)
      : m_property_list(init), name("Unnamed PropertyMap") {}

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
    /// <para>Returns nullptr if the property does not exist.</para>
    /// </summary>
    template <typename T>
    T* GetPropertyReference(const std::string& name)
    {
      auto it = m_property_list.find(name);
      if (it != m_property_list.end())
      {
        T* temp{};
        it->second->GetReferenceFromAny(reinterpret_cast<void**>(&temp));
        return temp;
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
        it->second->SetValueFromAny(std::addressof(value));
      }
    }

    void Serialize(std::ostream& stream) const
    {
      stream << name << '\n';
      for (const auto& [_name, _property] : m_property_list)
      {
        _property->Serialize(stream);
        stream << '\n';
      }
    }

    /// <summary>
    /// Set the name of the PropertyMap.
    /// </summary>
    void SetName(const std::string& _name) { name = _name; }

    /// <returns>Name of the PropertyMap</returns>
    std::string GetName() const { return name; }

    /// <returns>UUID of the PropertyMap</returns>
    UUID GetUUID() const { return uuid; }

  private:
    std::unordered_map<std::string, std::shared_ptr<PropertyBase>> m_property_list;
    std::string name;
    UUID uuid;
  };

}