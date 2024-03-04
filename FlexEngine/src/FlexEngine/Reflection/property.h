#pragma once

#include "propertybase.h"
//#include "Component/base.h"

#include <iostream>
#include <sstream>
#include <functional>

namespace FlexEngine
{

  /// <summary>
  /// Property is a wrapper used to store values of different types in a uniform way.
  /// <para>It is used to store values in a generic way, and to provide a uniform interface to access them.</para>
  /// <para>The getter and setter functions are used to access the underlying value, if they are not null.</para>
  /// </summary>
  template <typename T, typename GetterFn, typename SetterFn>
  class Property : public PropertyBase
  {
  public:
    /// <summary>
    /// Constructor for the Property class.
    /// <para>This constructor is for variables</para>
    /// <para>The getter and setter functions are optional.</para>
    /// </summary>
    Property(std::string name, GetterFn getter, SetterFn setter) : name(name), getter_function(getter), setter_function(setter) {}

    /// <summary>
    /// Constructor for the Property class.
    /// <para>This constructor is for references</para>
    /// <para>The getter and setter functions are optional.</para>
    /// </summary>
    Property(T* reference, std::string name, GetterFn getter = nullptr, SetterFn setter = nullptr) : reference(reference), name(name), getter_function(getter), setter_function(setter) {}

    /// <summary>
    /// Get the value of the property.
    /// <para>If a getter function is provided, it is used to get the value.</para>
    /// <para>If the getter function is not provided, the last value set is returned.</para>
    /// </summary>
    T Get()
    {
      //if (reference) value = *reference;
      FLX_ASSERT(reference != nullptr, "Property reference is nullptr");
      /*else*/ if (getter_function) value = getter_function();
      return value;
    }
    /// <summary>
    /// Set the value of the property.
    /// <para>If a setter function is provided, it is used to set the value.</para>
    /// <para>If the setter function is not provided, the setter function is not called.</para>
    /// </summary>
    void Set(const T& new_value)
    {
      if (reference) *reference = new_value;
      else if (setter_function) setter_function(new_value);
      value = new_value;
    }
    /// <summary>
    /// Gets the original reference to the value.
    /// <para>If a reference is provided, it is returned.</para>
    /// <para>If a reference is not provided, a reference to the Property value is returned.</para>
    /// </summary>
    T* GetReference()
    {
      if (reference) return reference;
      else return nullptr;
    }

    // global getter and setter
    void GetValueFromAny(void* _value) override { *static_cast<T*>(_value) = Get(); }
    void GetReferenceFromAny(void** _reference) override { *_reference = GetReference(); }
    void SetValueFromAny(const void* _value) override { Set(*static_cast<const T*>(_value)); }

    // serialize
    void Serialize(std::ostream& stream) const override
    {
      stream << GetName() << " (" << GetType() << ") : " << ToString();
    }

    /// <returns>name of the property</returns>
    std::string GetName() const { return name; }

    /// <returns>string representation of the property type</returns>
    std::string GetType() const override { return typeid(T).name(); }

    /// <returns>string representation of the property value</returns>
    std::string ToString() const override// { return ToStringImpl(value); }
    {
      // currently supported objects:
      // - ComponentMap
      if (reference) return ToStringImpl(*reference);
      return ToStringImpl(value);
    }

  private:
    // general template for most types
    template <typename U>
    static std::string ToStringImpl(const U& val)
    {
      return std::to_string(val);
    }

    // specialization for ComponentMap
    template <typename K, typename V>
    static std::string ToStringImpl(const std::unordered_map<K, V>& map)
    {
      std::stringstream ss{};
      ss << "{ ";
      for (const auto& pair : map)
      {
        ss << ToString(pair.first) << ": " << ToString(pair.second) << ", ";
      }
      ss << "}";
      return ss.str();
    }

  private:
    std::string name;                     // name of the property
    T value{};                            // copy of the value
    T* reference = nullptr;               // reference to the original object (optional)
                                          // this must be set in the constructor,
                                          // and the original object must outlive the property
    GetterFn getter_function = nullptr;   // reference to the getter function (optional)
    SetterFn setter_function = nullptr;   // reference to the setter function (optional)
  };

}