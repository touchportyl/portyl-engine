#pragma once

#include "flexassert.h"
#include "uuid.h"

#include <string>

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
    //virtual void GetValueFromAny(void* value) const = 0;
    virtual void GetValueFromAny(void* value) = 0;

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

}