#pragma once

#include <iostream>
#include <string>

#include <functional>
#include <iomanip>

namespace FlexEngine
{

  /// <summary>
  /// Base class for properties.
  /// </summary>
  class PropertyBase
  {
  public:
    virtual ~PropertyBase() = default;
    virtual void GetValueFromAny(void* value) const = 0;
    virtual void SetValueFromAny(const void* value) = 0;
    virtual std::string ToString() const { return uuid; }
    UUID uuid;
  };

  /// <summary>
  /// Property is a wrapper used to store values of different types in a uniform way.
  /// <para>The getter and setter functions are used to access the underlying value, if they are not null.</para>
  /// </summary>
  template <typename T, typename GetterFn, typename SetterFn>
  class Property : public PropertyBase
  {
  public:
    // constructor
    Property(GetterFn getter, SetterFn setter) : getter_function(getter), setter_function(setter) {}

    // getter and setter
    T Get() const { if (getter_function) { return getter_function(); } return value; }
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