#include "pch.h"

#include "ecs.h"
#include "Component/Transform2D.h"

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
    return { entity_name, uuid };
  }

  Entity ECS::GetEntity(const UUID& entity_uuid)
  {
    return { s_entities[entity_uuid], entity_uuid };
  }

  Entity ECS::GetEntity(const std::string& entity_name)
  {
    for (auto& [uuid, name] : s_entities)
    {
      if (name == entity_name)
      {
        return { name, uuid };
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


  //template <typename... Ts>
  //std::vector<Entity> ECS::FindEntitiesByComponent()
  //{
  //  std::vector<Entity> entities;
  //  for (auto& [uuid, name] : s_entities)
  //  {
  //    if (((Internal_GetComponentBucket<Ts>().find(uuid) != Internal_GetComponentBucket<Ts>().end()) && ...))
  //    {
  //      entities.push_back({ name, uuid });
  //    }
  //  }
  //  return entities;
  //}
  // TODO: Make this a python script that generates the template instantiations
  // WHY MUST THIS BE HARDCODED
  //template std::vector<Entity> ECS::FindEntitiesByComponent<Transform>();

  template <typename T>
  ECS::ComponentBucket& ECS::Internal_GetComponentBucket()
  {
    return *s_buckets[typeid(T)];
  }

  std::unordered_map<UUID, std::string>& ECS::Internal_GetEntities() { return s_entities; }

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
      for (auto& [entity, component] : Internal_GetComponentBucket<TYPE>()) \
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
      Log::Debug(Entity(name, uuid).ToString());
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
