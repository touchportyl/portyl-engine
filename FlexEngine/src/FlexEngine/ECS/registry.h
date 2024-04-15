#pragma once

#include "uuid.h" // includes <functional>

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <any>

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

  // Stores all entities and components in a registry
  // Entities are defined by their UUID
  // Components are defined by their type
  class ECS
  {
  public:
    // Entity UUID and name
    using Entity = std::pair<UUID, std::string>;
    #define NullEntity Entity( 0, "" )

    // Component pointer and Entity UUID
    template <typename T>
    using Component = std::pair<std::weak_ptr<T>, UUID>;

    // Component storage
    template <typename T>
    using ComponentMap = std::unordered_map<UUID, std::shared_ptr<T>>;

  private:
    // List of entities
    // Maps entity UUID to entity name
    std::unordered_map<UUID, std::string> entities;

    // Registry
    // Maps component type to component map
    // Component map maps entity UUID to component pointer
    // Cast std::any to the component map type to access the component map
    // 
    // Each component type can be associated with multiple entities
    // But each entity can only have one instance of each component type
    std::unordered_map<std::type_index, std::unordered_map<UUID, std::shared_ptr<void>>> registry;

    // TODO: worry about this later
    //static std::vector<ECS> instances;

  public:

    // Entity management functions

    Entity CreateEntity(const std::string& name = "New Entity");

    void DestroyEntity(Entity& entity);
    void DestroyEntity(UUID entity);

    Entity FindEntityByName(const std::string& name);

    // Component management functions

    template <typename T>
    void AddComponent(Entity& entity, T component = {});
    template <typename T>
    void AddComponent(UUID entity, T component = {});

    template <typename T>
    void RemoveComponent(Entity& entity);
    template <typename T>
    void RemoveComponent(UUID entity);

    template <typename T>
    Component<T>& GetComponent(Entity& entity);
    template <typename T>
    Component<T>& GetComponent(UUID entity);

    // Find all entities with a specific set of components
    template <typename... Ts>
    std::vector<Entity> FindEntitiesByComponent();
  };

}
