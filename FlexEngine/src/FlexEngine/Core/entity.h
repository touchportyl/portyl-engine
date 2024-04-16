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

    Entity(UUID uuid = UUID::Generate(), const std::string& name = "New Entity");

    std::string ToString() const;

    static Entity Null;

#ifdef _DEBUG
    void Dump();
#endif
  };

}
