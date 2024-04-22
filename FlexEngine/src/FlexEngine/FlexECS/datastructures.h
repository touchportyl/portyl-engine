#pragma once

#include "flx_api.h"

#include "uuid.h" // <functional> <string>
#include "flexlogger.h" // <filesystem> <fstream> <string>
#include "Reflection/base.h" // <cstddef> <iostream> <vector> <map> <functional> "flexassert.h" "flexformatter.h"

//#include <vector>
#include <unordered_map>
#include <algorithm> // std::sort
#include <typeindex> // std::type_index
#include <memory> // std::shared_ptr

namespace FlexEngine
{
  namespace FlexECS
  {

    // The FlexEngine Entity-Component-System (ECS) implementation
    // 
    // This uses the archetype pattern to store entities and components in a way
    // that is cache friendly and allows for efficient queries.
    // 
    // References:
    // https://ajmmertens.medium.com/building-an-ecs-1-where-are-my-entities-and-components-63d07c7da742
    // https://ajmmertens.medium.com/building-an-ecs-2-archetypes-and-vectorization-fe21690805f9
    // https://www.youtube.com/watch?v=71RSWVyOMEY


    // Forward declarations

    class State;
    class Scene;
    class Entity;
    struct ArchetypeEdge;


    #pragma region Data Structures

    // Unique identifiers for entities, components, and archetypes
    // Incremented each time a new entity, component, or archetype is created
    // TODO: Implement loaded entity ids to store lifetime, flags, and functionality to reuse ids

    using EntityID = std::size_t;
    //using ComponentID = std::type_index;
    //using ComponentID = Reflection::TypeDescriptor*;
    using ComponentID = std::string; // Use TYPE_DESCRIPTOR_LOOKUP to get the TypeDescriptor
    using ArchetypeID = std::size_t;


    // A vector of ComponentIDs
    // Unique identifier for an archetype
    // Make sure to sort the list of component ids before using it as a key
    using ComponentIDList = std::vector<ComponentID>;

    #pragma region Specializations for std::hash and std::equal_to

  }
}

namespace std
{

  // Specialize std::hash for FlexEngine::FlexECS::ComponentIDList
  template <>
  struct hash<FlexEngine::FlexECS::ComponentIDList>
  {
    std::size_t operator()(const FlexEngine::FlexECS::ComponentIDList& list) const
    {
      // Implement the hash function
      std::size_t hash_value = 0;
      for (const auto& id : list)
        hash_value ^= std::hash<FlexEngine::FlexECS::ComponentID>()(id);
      return hash_value;
    }
  };

  // Specialize std::equal_to for FlexEngine::FlexECS::ComponentIDList
  template <>
  struct equal_to<FlexEngine::FlexECS::ComponentIDList>
  {
    bool operator()(
      const FlexEngine::FlexECS::ComponentIDList& lhs,
      const FlexEngine::FlexECS::ComponentIDList& rhs
    ) const
    {
      // Implement the equality comparison
      return lhs == rhs;
    }
  };

}

namespace FlexEngine
{
  namespace FlexECS
  {

    #pragma endregion


    // Component table
    template <typename T = void>
    using ComponentData = std::shared_ptr<T>;

    using Column = std::vector<ComponentData<void>>;
    using Row = std::vector<Column>;
    using ArchetypeTable = Row;

    // Type used to store each unique component list only once
    // This is the main data structure used to store entities and components
    struct __FLX_API Archetype
    { FLX_REFL_SERIALIZABLE
      ArchetypeID id{};
      ComponentIDList type;
      ArchetypeTable archetype_table; // This is where the components are stored
      std::vector<EntityID> entities;
      std::unordered_map<ComponentID, ArchetypeEdge> edges;
    };

    // Edges to other archetypes
    // Use pointers instead of references for lazy initialization
    struct ArchetypeEdge
    {
      Archetype* add = nullptr;
      Archetype* remove = nullptr;
    };

    // Find an archetype by its list of component ids
    //extern std::unordered_map<ComponentIDList, Archetype> archetype_index;





    // Record in entity_index with archetype and row
    // NOTE: It is trusted that the Archetype* is valid
    // The Archetype ptr is owned by archetype_index which is managed in the stack
    struct __FLX_API EntityRecord
    { FLX_REFL_SERIALIZABLE
      Archetype* archetype;
      ArchetypeID archetype_id; // used during deserialization to reconnect the archetype ptr
      std::size_t row;
    };

    // Find the archetype for an entity
    //extern std::unordered_map<EntityID, EntityRecord> entity_index;





    // Record in component_index with component column for archetype
    struct __FLX_API ArchetypeRecord
    { FLX_REFL_SERIALIZABLE
      std::size_t column;
    };

    // Used to lookup components in archetypes
    using ArchetypeMap = std::unordered_map<ArchetypeID, ArchetypeRecord>;

    // Find the column for a component in an archetype
    //extern std::unordered_map<ComponentID, ArchetypeMap> component_index;

    #pragma endregion


    #pragma region Classes

    // Macros for access to the ECS data structures

    #define ARCHETYPE_INDEX Scene::GetActiveScene()->archetype_index
    #define ENTITY_INDEX Scene::GetActiveScene()->entity_index
    #define COMPONENT_INDEX Scene::GetActiveScene()->component_index


    // The scene holds all the entities and components.
    class __FLX_API Scene
    { FLX_REFL_SERIALIZABLE
      // ecs data structures
      std::unordered_map<ComponentIDList, Archetype> archetype_index;
      std::unordered_map<EntityID, EntityRecord> entity_index;
      std::unordered_map<ComponentID, ArchetypeMap> component_index;

      // TODO: Implement reusing entity ids
      EntityID next_entity_id = 0;

      static std::shared_ptr<Scene> s_active_scene;

      // allow access internal ecs data structures
      friend class Entity;

    public:

      static Scene Null;

      // Scene management functions

      static std::shared_ptr<Scene> CreateScene();
      static std::shared_ptr<Scene> GetActiveScene();
      static void SetActiveScene(Scene scene);
      static void SetActiveScene(std::shared_ptr<Scene> scene);

      // Entity management functions

      // Creates a new entity and gives it the default archetype which is for storing the entity's name.
      // Entities are registered this way. They are not stored in the scene, but in the actual ECS.
      static Entity CreateEntity(const std::string& name = "New Entity");

      // Scene serialization functions
      // This is the interface for the reflection system to serialize and deserialize
      // the ECS data structures. Use this interface to save and load scenes.

      void Save();
      void Load();
      static void SaveActiveScene();

    private:
      // INTERNAL FUNCTION
      // After reconstructing the ECS from a saved state, the archetype pointers in the entity_index
      // need to be reconnected to the archetype_index
      void Internal_RelinkEntityArchetypePointers();

#ifdef _DEBUG
    public:
      void Dump() const;
      void DumpArchetypeIndex() const;
      void DumpEntityIndex() const;
      void DumpComponentIndex() const;
#endif
    };

    // The entity class is a handle to an entity in the ECS.
    // It is used to add, remove, and get components from an entity,
    // but it does not store anything other than the entity id,
    // which is used to look up the entity in the ECS.
    class __FLX_API Entity
    {
      EntityID entity_id;

    public:
      Entity(EntityID id);

      // Conversion operator to EntityID for ease of use
      operator EntityID() const;

      // Checks if an entity has a component
      template <typename T>
      bool HasComponent();

      // Returns a nullptr if the component is not found
      template <typename T>
      std::shared_ptr<T> GetComponent();

      // Specialization to get a component safely
      // Returns true if the component is found
      template <typename T>
      bool TryGetComponent(std::shared_ptr<T>& out);

      // Add a component to an entity
      // Usage example: entity.AddComponent<Transform>({ 1, 2, 3 });
      // Usage example: entity.AddComponent(Transform(1, 2, 3));
      template <typename T>
      void AddComponent(const T& data);

      // Remove a component from an entity
      // Usage example: entity.RemoveComponent<Transform>();
      template <typename T>
      void RemoveComponent();

    private:
      // Allow the scene class to access internal functions
      friend class FlexECS::Scene;

      // INTERNAL FUNCTION
      // Used to create a new archetype
      static Archetype& Internal_CreateArchetype(ComponentIDList type);

      // INTERNAL FUNCTION
      // Used to move an entity from one archetype to another
      static void Internal_MoveEntity(EntityID entity, Archetype& from, size_t from_row, Archetype& to);
    };

    #pragma endregion

  }
}

// Template implementations for Entity
#include "entity.inl"
