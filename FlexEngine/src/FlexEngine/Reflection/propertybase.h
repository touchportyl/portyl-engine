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
    virtual void GetValueFromAny(void* value) = 0;

    /// <summary>
    /// Passthrough function to get the reference of the property.
    /// </summary>
    virtual void GetReferenceFromAny(void** reference) = 0;

    /// <summary>
    /// Passthrough function to set the value of the property.
    /// </summary>
    virtual void SetValueFromAny(const void* value) = 0;

    /// <summary>
    /// Passthrough function to serialize the property.
    /// </summary>
    virtual void Serialize(std::ostream& stream) const = 0;

    /// <summary>
    /// Passthrough function to deserialize the property.
    /// </summary>
    //virtual void Deserialize(std::istream& stream) = 0;

    /// <summary>
    /// Passthrough function to return the string representation of the property type.
    /// </summary>
    virtual std::string GetType() const = 0;

    /// <summary>
    /// Passthrough function to return the string representation of the property value.
    /// <para>Use GetValueFromAny to get the actual value instead.</para>
    /// </summary>
    virtual std::string ToString() const = 0;

    /// <returns>UUID of the property.</returns>
    UUID GetUUID() const { return uuid; }

  private:
    UUID uuid;
  };

}