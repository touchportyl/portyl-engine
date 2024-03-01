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
  template <typename T>
  class PropertyReference : public PropertyBase
  {
  public:
    /// <summary>
    /// Constructor for the PropertyReference class.
    /// <para>Pass in a pointer to the Property</para>
    /// <para>FLX_REFL_FRIEND is required to access private/protected references</para>
    /// </summary>
    PropertyReference(T* reference)
      : reference(reference) {}

    /// <summary>
    /// Get the value of the property.
    /// <para>If a getter function is provided, it is used to get the value.</para>
    /// <para>If the getter function is not provided, the last value set is returned.</para>
    /// </summary>
    T* Get() { return reference; }

    // global getter and setter
    void GetValueFromAny(void* _value) override {}
    void GetReferenceFromAny(void* _reference) override { _reference = Get(); }
    void SetValueFromAny(const void* _value) override {}

    // returns a string representation of the value
    std::string ToString() const override {}// return std::to_string(Get()); }

  private:
    T* reference;
    std::string name = "PropertyReference";
    bool is_read_only = false;
  };

  template <typename T>
  std::ostream& operator<<(std::ostream& stream, const PropertyReference<T>& property) {
    return stream << property.Get();
  }

}