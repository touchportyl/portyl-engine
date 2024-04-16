#include "pch.h"

#include "ecs.h"
#include "Component/transform.h"

namespace FlexEngine
{
  // static member initialization
  std::unordered_map<std::type_index, ECS::ComponentBucket*> ECS::s_buckets;
  std::unordered_map<UUID, std::string> ECS::s_entities;

  #pragma region Entity management functions

  Entity ECS::CreateEntity(const std::string& entity_name)
  {
    UUID uuid = UUID::Generate();
    s_entities[uuid] = entity_name;
    return { uuid, entity_name };
  }

  Entity ECS::GetEntity(const UUID& entity_uuid)
  {
    return { entity_uuid, s_entities[entity_uuid] };
  }

  Entity ECS::GetEntity(const std::string& entity_name)
  {
    for (auto& [uuid, name] : s_entities)
    {
      if (name == entity_name)
      {
        return { uuid, name };
      }
    }
    return Entity::Null;
  }

  void ECS::DestroyEntity(const UUID& entity_uuid)
  {
    s_entities.erase(entity_uuid);
  }

  void ECS::DestroyEntity(const std::string& entity_name)
  {
    for (auto& [uuid, name] : s_entities)
    {
      if (name == entity_name)
      {
        s_entities.erase(uuid);
        return;
      }
    }
  }

  #pragma endregion

  template <typename T>
  ECS::ComponentBucket ECS::GetComponentBucket()
  {
    return *s_buckets[typeid(T)];
  }

  template <typename T>
  void ECS::Internal_RegisterComponent(ComponentBucket* bucket)
  {
    // register component bucket in ECS
    s_buckets[typeid(T)] = bucket;
  }


#ifdef _DEBUG
  // hardcoded dump for now
  #define DUMP_COMPONENT(TYPE) \
    if (bucket.first == typeid(TYPE)) \
    { \
      for (auto& [entity, component] : GetComponentBucket<TYPE>()) \
      { \
        Log::Debug(std::string("Entity: ") + ECS::GetEntity(entity).ToString()); \
        Log::Debug(std::string("Component: ") + std::reinterpret_pointer_cast<TYPE>(component)->ToString()); \
      } \
    }
  
  void ECS::Dump()
  {
    Log::Debug(__FUNCTION__);

    // Dump entities

    Log::Debug("Entities:");
    for (auto& [uuid, name] : s_entities)
    {
      Log::Debug(Entity(uuid, name).ToString());
    }

    // Dump components

    Log::Info("The ECS::Dump() function does not automatically update the components.");
    Log::Info("If your component is not showing up, add it to the dump function manually.");
    for (auto& bucket : s_buckets)
    {
      Log::Debug(std::string("Bucket: ") + bucket.first.name());

      // manual registration of components
      DUMP_COMPONENT(Transform);
    }

    Log::Debug("End of dump.");
  }
#endif

}
