#include "registry.h"

namespace FlexEngine
{

  #pragma region Entity management functions

  ECS::Entity ECS::CreateEntity(const std::string& name)
  {
    // Generate a unique UUID for the entity
    // Make sure the UUID is not already in use
    UUID uuid = UUID::Generate();
    while ((uuid.Get() != 0) && (entities.find(uuid) != entities.end()))
    {
      uuid = UUID::Generate();
    }

    // Add the entity to the list of entities
    entities[uuid] = name;
    return { uuid, name };
  }

  void ECS::DestroyEntity(Entity& entity) { DestroyEntity(entity.first); }
  void ECS::DestroyEntity(UUID entity)
  {
    // remove all components associated with the entity
    for (auto& [type, component_map] : registry)
    {
      component_map.erase(entity);
    }
    entities.erase(entity);
  }

  ECS::Entity ECS::FindEntityByName(const std::string& name)
  {
    for (auto& [uuid, entity_name] : entities)
    {
      if (entity_name == name)
      {
        return { uuid, entity_name };
      }
    }
    return NullEntity;
  }

  #pragma endregion

  #pragma region Component management functions

  template <typename T>
  void ECS::AddComponent(Entity& entity, T component) { AddComponent<T>(entity.first, component); }
  template <typename T>
  void ECS::AddComponent(UUID entity, T component) { registry[typeid(T)][entity] = component; }

  template <typename T>
  void ECS::RemoveComponent(Entity& entity) { RemoveComponent<T>(entity.first); }
  template <typename T>
  void ECS::RemoveComponent(UUID entity) { registry[typeid(T)].erase(entity); }

  template <typename T>
  ECS::Component<T>& ECS::GetComponent(Entity& entity) { return GetComponent<T>(entity.first); }
  template <typename T>
  ECS::Component<T>& ECS::GetComponent(UUID entity) { return { registry[typeid(T)][entity], entity }; }

  template <typename... Ts>
  std::vector<ECS::Entity> ECS::FindEntitiesByComponent()
  {
    std::vector<Entity> out{};

    // check the specific bucket for each component type
    for (auto& [type, component_map] : registry)
    {
      // check if the type is one of the types in the list
      if (((type == typeid(Ts)) || ...))
      {
        // add all entities with the component to the output list
        // TODO: cache this
        for (auto& [entity, component] : component_map)
        {
          out.push_back({ entity, entities[entity] });
        }
      }
    }

    return out;
  }

  #pragma endregion

}
