#include "pch.h"

#include "entity.h"

#include "ecs.h"

namespace FlexEngine
{

  // static member initialization
  const Entity Entity::Null = { "Null", UUID::Null };

  Entity::Entity()
    : uuid(UUID::Null), name("Null")
  {
  }

  Entity::Entity(const std::string& name, UUID uuid)
    : uuid(uuid), name(name)
  {
  }

  Entity::operator bool() const { return uuid != UUID::Null; }
  Entity::operator UUID() const { return uuid; }
  Entity::operator std::string() const { return name; }

  std::string Entity::ToString() const
  {
    return name + " (" + std::to_string(uuid) + ")";
  }

#ifdef _DEBUG
  void Entity::Dump() const
  {
    Log::Debug(ToString());
  }
#endif

}
