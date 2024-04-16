#include "pch.h"

#include "entity.h"

#include "ecs.h"

namespace FlexEngine
{

  // static member initialization
  Entity Entity::Null = { UUID::Null, "Null" };

  Entity::Entity(UUID uuid, const std::string& name)
    : uuid(uuid), name(name)
  {
  }

  std::string Entity::ToString() const
  {
    return name + " (" + std::to_string(uuid) + ")";
  }

#ifdef _DEBUG
  void Entity::Dump()
  {
    Log::Debug(ToString());
  }
#endif

}
