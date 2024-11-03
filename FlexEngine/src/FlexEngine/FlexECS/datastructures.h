#pragma once

#include "flx_api.h"

#include "flexid.h" // <cstdint>
#include "flexformatter.h"  // "Wrapper/datetime.h" "Wrapper/file.h" <sstream>
                            // <RapidJSON/document.h> <RapidJSON/istreamwrapper.h> <RapidJSON/ostreamwrapper.h>
                            // <RapidJSON/writer.h> <RapidJSON/prettywriter.h>
#include "flexlogger.h" // <filesystem> <fstream> <string>
#include "Reflection/base.h"  // "Wrapper/flexassert.h" <rapidjson/document.h>
                              // <cstddef> <iostream> <string> <sstream> <vector> <map> <unordered_map> <functional>
#include "Wrapper/file.h" // "Wrapper/path.h" <fstream>

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

    class Scene;
    class Entity;
    struct ArchetypeEdge;


    #pragma region Data Structures

    // Unique identifiers for entities, components, and archetypes
    // Incremented each time a new entity, component, or archetype is created

    // Loaded unique identifier.
    // The first 32 bits are the ID, the next 28 bits are the generation, and the last 4 bits are the flags.
    // Use FlexECS::ID functions to manage the ID
    using EntityID = uint64_t;

    // Stringified type descriptor for components
    // Use TYPE_DESCRIPTOR_LOOKUP to convert the string back to the TypeDescriptor
    using ComponentID = std::string;

    // Just a unique identifier for an archetype counting up from 0
    using ArchetypeID = uint64_t;


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

    // ComponentData<void> is a special internal type that can hold any component data.
    // The void* it holds is type erased and must be cast to the correct type.
    // The first sizeof(std::size_t) bytes are used to store the size of the data.
    template <typename T = void>
    using ComponentData = std::shared_ptr<T>;

    // Create a new ComponentData<void>
    __FLX_API ComponentData<void> Internal_CreateComponentData(std::size_t size, void* data);

    // Get the size and data pointer from a ComponentData<void>
    __FLX_API std::pair<std::size_t, void*> Internal_GetComponentData(ComponentData<void> data);


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
      //ArchetypeID archetype_id_add = 0;     // used during deserialization to reconnect the archetype ptr
      //ArchetypeID archetype_id_remove = 0;  // used during deserialization to reconnect the archetype ptr
    };





    // Record in entity_index with archetype and row
    // NOTE: It is trusted that the Archetype* is valid
    // The Archetype ptr is owned by archetype_index which is managed in the stack
    struct __FLX_API EntityRecord
    { FLX_REFL_SERIALIZABLE
      Archetype* archetype;
      ArchetypeID archetype_id; // used during deserialization to reconnect the archetype ptr
      std::size_t row;
    };





    // Record in component_index with component column for archetype
    struct __FLX_API ArchetypeRecord
    { FLX_REFL_SERIALIZABLE
      std::size_t column;
    };

    // Used to lookup components in archetypes
    using ArchetypeMap = std::unordered_map<ArchetypeID, ArchetypeRecord>;

    #pragma endregion


    #pragma region Classes

    // Macros for access to the ECS data structures

    // Find an archetype by its list of component ids
    #define ARCHETYPE_INDEX FlexEngine::FlexECS::Scene::GetActiveScene()->archetype_index

    // Find the archetype for an entity
    #define ENTITY_INDEX FlexEngine::FlexECS::Scene::GetActiveScene()->entity_index

    // Find the column for a component in an archetype
    #define COMPONENT_INDEX FlexEngine::FlexECS::Scene::GetActiveScene()->component_index


    // The scene holds all the entities and components.
    class __FLX_API Scene
    { FLX_REFL_SERIALIZABLE FLX_ID_SETUP

      // TODO: Implement reusing entity ids
      //EntityID next_entity_id = 0;

      static std::shared_ptr<Scene> s_active_scene;

    public:

      // Null scene for when the active scene is set to null
      static Scene Null;

      // ECS data structures

      std::unordered_map<ComponentIDList, Archetype> archetype_index;
      std::unordered_map<EntityID, EntityRecord> entity_index;
      std::unordered_map<ComponentID, ArchetypeMap> component_index;

      #pragma region String Storage

    public:
      using StringIndex = std::size_t;

    private:
      // String storage to prevent strings from being freed.
      // Components that are strings should store the index of the string in this vector.
      std::vector<std::string> string_storage;

      // Strings that are removed from the string storage are added to this list.
      // New strings will always use the first available index in this list.
      std::vector<StringIndex> string_storage_free_list;

    public:
      std::string& Internal_StringStorage_Get(StringIndex index);

      StringIndex Internal_StringStorage_New(const std::string& str);
      void Internal_StringStorage_Delete(StringIndex index);

      // Defragment the string storage by moving all strings to the front of the vector
      // void Internal_StringStorage_Defragment();

      #pragma endregion

      #pragma region ECS View

    public:
      // Returns an entity list based off the list of components
      template <typename... Ts>
      std::vector<Entity> View();
      //#define FLX_ECS_VIEW(...) for (FlexEngine::FlexECS::Entity& entity : FlexEngine::FlexECS::Scene::GetActiveScene()->View<__VA_ARGS__>())

      #pragma endregion

      #pragma region Scene management functions

    public:
      static std::shared_ptr<Scene> CreateScene();
      static std::shared_ptr<Scene> GetActiveScene();
      static void SetActiveScene(const Scene& scene);
      static void SetActiveScene(std::shared_ptr<Scene> scene);

      #pragma endregion

      #pragma region Entity management functions

    public:
      // Creates a new entity and gives it the default archetype which is for storing the entity's name.
      // Entities are registered this way. They are not stored in the scene, but in the actual ECS.
      static Entity CreateEntity(const std::string& name = "New Entity");

      // Removes an entity from the ECS
      static void DestroyEntity(EntityID entity);

      // Passthrough functions to edit the entity's flags.
      // They only work on the current active scene.
      static void SetEntityFlags(EntityID& entity, const uint8_t flags);

      static EntityID CloneEntity(EntityID entityToCopy);

      static void SaveEntityAsPrefab(EntityID entityToSave, const std::string& prefabName);

      #pragma endregion

      #pragma region Scene serialization functions

    public:
      // This is the interface for the reflection system to serialize and deserialize
      // the ECS data structures. Use this interface to save and load scenes.

      void Save(File& file);
      static std::shared_ptr<Scene> Load(File& file);
      static void SaveActiveScene(File& file);

      #pragma endregion

    private:
      // INTERNAL FUNCTION
      // After reconstructing the ECS from a saved state, the archetype pointers in the entity_index
      // need to be reconnected to the archetype_index.
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
    { FLX_REFL_SERIALIZABLE

      EntityID entity_id;

    public:

      // Null entity
      static Entity Null;

      Entity();
      Entity(EntityID id);

      EntityID& Get();

      #pragma region Operator Overloads

      bool operator==(const Entity& other) const;
      bool operator!=(const Entity& other) const;
      bool operator<(const Entity& other) const;

      // Conversion operator to EntityID for ease of use
      operator EntityID() const;

      #pragma endregion

      #pragma region Component Management

      // Checks if an entity has a component
      template <typename T>
      bool HasComponent();

      // Returns a nullptr if the component is not found
      template <typename T>
      T* GetComponent();

      // Specialization to get a component safely
      // out is not modified if the component is not found
      // Returns true if the component is found
      template <typename T>
      bool TryGetComponent(T* out);

      // Add a component to an entity
      // Usage: entity.AddComponent<Transform>({ 1, 2, 3 });
      // Usage: entity.AddComponent(Transform(1, 2, 3));
      template <typename T>
      void AddComponent(const T& data);

      // Remove a component from an entity
      // Usage: entity.RemoveComponent<Transform>();
      template <typename T>
      void RemoveComponent();

      #pragma endregion

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

// Template implementations for Scene
#include "scene.inl"

// Template implementations for Entity
#include "entity.inl"
