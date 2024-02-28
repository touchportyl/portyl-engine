#pragma once

#include "propertybase.h"

#include <iostream>
#include <functional>

namespace FlexEngine
{

  /// <summary>
  /// Property is a wrapper used to store values of different types in a uniform way.
  /// <para>It is used to store values in a generic way, and to provide a uniform interface to access them.</para>
  /// <para>The getter and setter functions are used to access the underlying value, if they are not null.</para>
  /// </summary>
  template <typename T, typename GetterFn, typename SetterFn>
  class PropertyReference : public PropertyBase
  {
  public:
    /// <summary>
    /// Constructor for the PropertyReference class.
    /// <para>The getter and setter functions are optional.</para>
    /// </summary>
    PropertyReference(T& reference, GetterFn getter, SetterFn setter)
      : reference(reference), getter_function(getter), setter_function(setter) {}

    /// <summary>
    /// Get the value of the property.
    /// <para>If a getter function is provided, it is used to get the value.</para>
    /// <para>If the getter function is not provided, the last value set is returned.</para>
    /// </summary>
    T& Get() { if (getter_function) { reference = getter_function(); } return reference; }
    /// <summary>
    /// Set the value of the property.
    /// <para>If a setter function is provided, it is used to set the value.</para>
    /// <para>If the setter function is not provided, the setter function is not called.</para>
    /// </summary>
    void Set(const T& new_reference)
    { // if the property is read only, do not set the value
      FLX_ASSERT(!is_read_only, "Property is read only.");
      if (setter_function) { setter_function(new_reference);
      }
    }

    // global getter and setter
    void GetValueFromAny(void* _value) override { *static_cast<T*>(_value) = Get(); }
    void SetValueFromAny(const void* _value) override { Set(*static_cast<const T*>(_value)); }

    // returns a string representation of the value
    std::string ToString() const override {}// return std::to_string(Get()); }

  private:
    T& reference;
    std::string name = "PropertyReference";
    bool is_read_only = false;
    GetterFn getter_function = nullptr;
    SetterFn setter_function = nullptr;
  };

  template <typename T, typename GetterFn, typename SetterFn>
  std::ostream& operator<<(std::ostream& stream, const PropertyReference<T, GetterFn, SetterFn>& property) {
    return stream << property.Get();
  }

}