#pragma once

#include <functional> // std::hash

namespace FlexEngine
{

  class UUID
  {
    unsigned long long m_value;

  public:
    UUID();
    UUID(unsigned long long value);
    UUID(const UUID& other);
    UUID& operator=(const UUID& other);

    // Conversion operators

    operator unsigned long long() const;

    // Accessors

    unsigned long long Get() const;
    void Set(unsigned long long value);

    // Comparison operators

    bool operator==(const UUID& other) const;
    bool operator!=(const UUID& other) const;

    bool operator<(const UUID& other) const;
    bool operator>(const UUID& other) const;

    // Null UUID
    static UUID Null;

    static UUID Generate();
  };

}

// Hash function specialization for UUID
namespace std
{
  template <>
  struct hash<FlexEngine::UUID>
  {
    size_t operator()(const FlexEngine::UUID& uuid) const
    {
      // Compute a hash for the UUID (e.g., using its underlying integer or string representation)
      return std::hash<unsigned long long>{}(uuid);
    }
  };
}
