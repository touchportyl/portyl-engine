#pragma once

#include "PropertyBase.h"

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
  class Property : public PropertyBase
  {
  public:
    /// <summary>
    /// Constructor for the Property class.
    /// <para>This constructor is for variables</para>
    /// <para>The getter and setter functions are optional.</para>
    /// </summary>
    Property(GetterFn getter, SetterFn setter) : getter_function(getter), setter_function(setter) {}
    Property(T* reference) : reference(reference) {}

    /// <summary>
    /// Get the value of the property.
    /// <para>If a getter function is provided, it is used to get the value.</para>
    /// <para>If the getter function is not provided, the last value set is returned.</para>
    /// </summary>
    T Get() const
    {
      if (reference) value = *reference;
      else if (getter_function) value = getter_function();
      return value;
    }
    /// <summary>
    /// Set the value of the property.
    /// <para>If a setter function is provided, it is used to set the value.</para>
    /// <para>If the setter function is not provided, the setter function is not called.</para>
    /// </summary>
    void Set(const T& new_value) { value = new_value; if (setter_function) { setter_function(new_value); } }
    /// <summary>
    /// Gets the original reference to the value.
    /// </summary>
    T* GetReference() { if (reference) { return reference; } }

    // returns a string representation of the value
    std::string ToString() const override { return std::to_string(Get()); }

  private:
    T value = 0;                        // copy of the value
    T* reference = nullptr;             // reference to the original object (optional)
                                        // this must be set in the constructor,
                                        // and the original object must outlive the property
    GetterFn getter_function = nullptr; // reference to the getter function (optional)
    SetterFn setter_function = nullptr; // reference to the setter function (optional)
  };

  template <typename T, typename GetterFn, typename SetterFn>
  std::ostream& operator<<(std::ostream& stream, const Property<T, GetterFn, SetterFn>& property)
  {
    return stream << property.Get();
  }

}