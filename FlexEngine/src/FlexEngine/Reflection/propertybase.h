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
    template <typename T>
    T* As()
    {
      return dynamic_cast<T*>(this);
    }

    /// <summary>
    /// Returns a string representation of the PropertyBase UUID
    /// <para>Property overrides this function to return a string representation of the value.</para>
    /// </summary>
    virtual std::string ToString() const { return uuid; }

    UUID uuid;
  };

}