#pragma once

#include "uuid.h"

#include <string>

namespace FlexEngine
{

  // This does not actually contain any data, but is used to identify an entity.
  // The actual list of entities is stored and handled by the ECS.
  struct Entity
  {
    UUID uuid;
    std::string name;

    Entity();
    Entity(const std::string& name = "New Entity", UUID uuid = UUID::Generate());

    // Null entity, used to represent an entity that does not exist.
    static const Entity Null;

    // Conversion operators

    operator bool() const;
    operator UUID() const;
    operator std::string() const;

    std::string ToString() const;

#ifdef _DEBUG
    void Dump() const;
#endif
  };

}
