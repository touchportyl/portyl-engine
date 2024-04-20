#pragma once

#include "uuid.h"

#include "entity.h"

#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>


// ECS (Entity-Component-System) framework
// 
// Components have no game code (behavior) inside of them.
// The components don't have to be located physically together with the entity,
// but should be easy to find and access using the entity.
// 
// "Each System runs continuously (as though each System had its own private thread)
// and performs global actions on every Entity that possesses a Component or Components
// that match that System's query."
// - https://en.wikipedia.org/wiki/Entity_component_system
// 
// This ECS class is a unique implementation of the ECS framework.
// It is inspired by the Unity ECS framework.
// 
// Component buckets refer to the storage of components in their own individual containers.
// System views refer to the list of entities that have the queried components.
// 
// The unique feature of this ECS implementation is that the component buckets and system views
// can be registered from anywhere in the code. This means that the ECS class is not limited to
// a specific set of components and systems. Any developer can easily create new components and
// systems by registering them in the ECS class.
// 
// Registering a new component is as easy as putting the component type in the template argument.
// A new bucket will be created for the component type through the std::unordered_map.
// 
// TODO: Implement the system view functionality
// Currently, the ECS class does not cache the system views.
// Use FindEntitiesByComponent to find entities with specific components.


namespace FlexEngine
{

  class ECS
  {
  public:
    // Component bucket type
    // Use std::reinterpret_pointer_cast to cast to the correct type
    using ComponentBucket = std::unordered_map<UUID, std::shared_ptr<void>>;

  private:
    // umap of all component buckets
    // The pointer is safe because the buckets are handled as static variables
    static std::unordered_map<std::type_index, ComponentBucket*> s_buckets;

    // umap of all entities
    static std::unordered_map<UUID, std::string> s_entities;

  public:

    // Entity management functions

    static Entity CreateEntity(const std::string& entity_name = "New Entity");
    static Entity GetEntity(const UUID& entity_uuid);
    static Entity GetEntity(const std::string& entity_name);
    static void DestroyEntity(const UUID& entity_uuid);
    static void DestroyEntity(const std::string& entity_name);

    // This is a temporary version of a system view
    // System views are for querying entities with specific components
    //template <typename... Ts>
    //static std::vector<Entity> FindEntitiesByComponent();

    // Alternate method for querying entities with specific components

    #define FLX_ECS_SYSTEM_VIEW_START(ENTITY_LIST) \
      std::vector<Entity> ENTITY_LIST; \
      for (auto& [uuid, name] : ECS::Internal_GetEntities()) \
      { \
        if (

    #define FLX_ECS_SYSTEM_VIEW_QUERY(TYPE) \
          ECS::Internal_GetComponentBucket<TYPE>().find(uuid) != ECS::Internal_GetComponentBucket<TYPE>().end() &&

    #define FLX_ECS_SYSTEM_VIEW_END(ENTITY_LIST) \
        true ) \
        { \
          ENTITY_LIST.push_back({ name, uuid }); \
        } \
      }

    // INTERNAL FUNCTION
    // Get component bucket by type
    // This exposes the component bucket to the public
    // Do not use this function unless you know what you are doing
    template <typename T>
    static ECS::ComponentBucket& Internal_GetComponentBucket();

    // INTERNAL FUNCTION
    // Get the component buckets
    // This exposes the component buckets to the public
    // Do not use this function unless you know what you are doing
    static std::unordered_map<std::type_index, ComponentBucket*>& Internal_GetComponentBuckets();

    // INTERNAL FUNCTION
    // Get the entity list
    // This exposes the entity list to the public
    // Do not use this function unless you know what you are doing
    static std::unordered_map<UUID, std::string>& Internal_GetEntities();

    // INTERNAL FUNCTION
    // Registers a component type with the ECS
    // This is done automatically by the component at startup
    // Do not call this function manually
    template <typename T>
    static void Internal_RegisterComponent(ComponentBucket* bucket);

    #ifdef _DEBUG
    // Dumps all components in the ECS to the console
    static void Dump();
    #endif
  };

}
