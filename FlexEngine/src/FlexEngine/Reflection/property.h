#pragma once

#include <iostream>
#include <string>
#include <functional>

namespace FlexEngine
{

  /// <summary>
  /// Base class for properties.
  /// </summary>
  class PropertyBase
  {
  public:
    /// <summary>
    /// Passthrough function to get the value of the property.
    /// </summary>
    virtual void GetValueFromAny(void* value) const = 0;

    /// <summary>
    /// Passthrough function to set the value of the property.
    /// </summary>
    virtual void SetValueFromAny(const void* value) = 0;

    /// <summary>
    /// Returns a string representation of the PropertyBase UUID
    /// <para>Property overrides this function to return a string representation of the value.</para>
    /// </summary>
    virtual std::string ToString() const { return uuid; }

    UUID uuid;
  };

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
    /// <para>The getter and setter functions are optional.</para>
    /// </summary>
    Property(GetterFn getter, SetterFn setter) : getter_function(getter), setter_function(setter) {}

    /// <summary>
    /// Get the value of the property.
    /// <para>If a getter function is provided, it is used to get the value.</para>
    /// <para>If the getter function is not provided, the last value set is returned.</para>
    /// </summary>
    T Get() const { if (getter_function) { value = getter_function(); } return value; }
    /// <summary>
    /// Set the value of the property.
    /// <para>If a setter function is provided, it is used to set the value.</para>
    /// <para>If the setter function is not provided, the setter function is not called.</para>
    /// </summary>
    void Set(const T& new_value) { value = new_value; if (setter_function) { setter_function(new_value); } }

    // global getter and setter
    void GetValueFromAny(void* _value) const override { *static_cast<T*>(_value) = Get(); }
    void SetValueFromAny(const void* _value) override { Set(*static_cast<const T*>(_value)); }

    // returns a string representation of the value
    std::string ToString() const override { return std::to_string(Get()); }

  private:
    T value;
    GetterFn getter_function = nullptr;
    SetterFn setter_function = nullptr;
  };

  template <typename T, typename GetterFn, typename SetterFn>
  std::ostream& operator<<(std::ostream& stream, const Property<T, GetterFn, SetterFn>& property) {
    return stream << property.Get();
  }

}