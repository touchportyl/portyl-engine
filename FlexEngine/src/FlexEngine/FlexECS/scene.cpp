#include "datastructures.h"

namespace FlexEngine
{
  namespace FlexECS
  {

    // static member initialization
    std::shared_ptr<Scene> Scene::s_active_scene = nullptr;
    Scene Scene::Null = Scene();

    std::shared_ptr<Scene> Scene::CreateScene()
    {
      FLX_FLOW_FUNCTION();

      // set itself as the active scene unless there is already an active scene
      if (s_active_scene == nullptr)
      {
        s_active_scene = std::make_shared<Scene>();
      }
      return s_active_scene;
    }

    std::shared_ptr<Scene> Scene::GetActiveScene()
    {
      // create a new scene if there isn't one
      if (s_active_scene == nullptr)
      {
        CreateScene();
      }
      return s_active_scene;
    }

    void Scene::SetActiveScene(Scene scene)
    {
      SetActiveScene(std::make_shared<Scene>(scene));
    }
    void Scene::SetActiveScene(std::shared_ptr<Scene> scene)
    {
      FLX_FLOW_FUNCTION();

      // guard
      if (scene == nullptr)
      {
        Log::Warning("Attempted to set active scene to nullptr. Use the Scene::Null to set the scene to null instead.");
        return;
      }
      s_active_scene = scene;
    }

    Entity Scene::CreateEntity(const std::string& name)
    {
      FLX_FLOW_FUNCTION();

      // manually register a name component
      // this is to register the entity in the entity index and archetype
      ComponentID component = typeid(std::string);

      // type erasure
      ComponentData<void> data_ptr = std::make_shared<std::string>(name);

      // Get the archetype for the entity
      ComponentIDList type = { component };

      // create a new archetype if it doesn't exist
      if (ARCHETYPE_INDEX.count(type) == 0)
      {
        Entity::Internal_CreateArchetype(type);
      }

      // update entity vector
      Archetype& archetype = ARCHETYPE_INDEX[type];
      archetype.entities.push_back(Scene::GetActiveScene()->next_entity_id);

      // update entity records
      EntityRecord entity_record = { &archetype, archetype.entities.size() - 1 };
      ENTITY_INDEX[Scene::GetActiveScene()->next_entity_id] = entity_record;

      // store the component data in the archetype
      //ArchetypeMap& archetype_map = COMPONENT_INDEX[component];
      //ArchetypeRecord& archetype_record = archetype_map[archetype.id];
      //archetype.archetype_table[archetype_record.column].push_back(data_ptr);
      archetype.archetype_table[0].push_back(data_ptr); // there is only one component in this archetype

      return Scene::GetActiveScene()->next_entity_id++;
    }


#ifdef _DEBUG

    void Scene::Dump() const
    {
      DumpArchetypeIndex();
      DumpEntityIndex();
      DumpComponentIndex();
    }

    void Scene::DumpArchetypeIndex() const
    {
      Log::Info("Dumping archetype_index");
      for (auto& [archetype, archetype_storage] : ARCHETYPE_INDEX)
      {
        Log::Debug("Archetype: " + std::to_string(archetype_storage.id));
        Log::Debug("- Number of entities: " + std::to_string(archetype_storage.entities.size()));
        Log::Debug("- Number of components: " + std::to_string(archetype_storage.archetype_table.size()));

        int i = 0;
        for (auto& column : archetype_storage.archetype_table)
        {
          Log::Debug("  Component(" + std::to_string(i) + "): " + std::string(archetype_storage.type[i].name()));
          Log::Debug("    Entities in component: " + std::to_string(column.size()));
          i++;
        }
      }
      Log::Info("End of dump.");
    }

    void Scene::DumpEntityIndex() const
    {
      Log::Info("Dumping entity_index");
      for (auto& [id, entity_record] : ENTITY_INDEX)
      {
        Log::Debug("Entity: " + std::to_string(id));
        Log::Debug("  Archetype ID: " + std::to_string(entity_record.archetype->id));
      }
      Log::Info("End of dump.");
    }

    void Scene::DumpComponentIndex() const
    {
      Log::Info("Dumping component_index");
      for (auto& [component_id, archetype_map] : COMPONENT_INDEX)
      {
        Log::Debug("Component: " + std::string(component_id.name()));
        for (auto& [archetype, archetype_record] : archetype_map)
        {
          Log::Debug("  Archetype ID: " + std::to_string(archetype));
          Log::Debug("    Column: " + std::to_string(archetype_record.column));
        }
      }
      Log::Info("End of dump.");
    }

#endif

  }
}