#pragma once

#include <iostream>
#include <string>

namespace FlexEngine
{

  using Hash = unsigned long long;

  class UUID
  {
  public:
    UUID();
    UUID(Hash uuid);
    UUID(const UUID&) = default;

    operator Hash() const { return m_UUID; }
    operator std::string() const { return std::to_string(m_UUID); }
    std::ostream& operator<<(std::ostream& stream) const { return stream << m_UUID; }
  private:
    const Hash m_UUID;
  };

}