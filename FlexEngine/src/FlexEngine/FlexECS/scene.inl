// inline functions for Scene class

// Steps:
// 1. Loop through each archetype and check if the archetype has the requested components
// 2. Get the entities from the archetype

/*
  \brief Performs a query that returns a list of entities that have the requested components, but assumes that you don't want to cache it as it is volatile.
*/
template <typename... Ts>
std::vector<FlexEngine::FlexECS::Entity> FlexEngine::FlexECS::Scene::Query()
{
  Log::Debug("Query was called");
  std::vector<Entity> entities;

  // 1. Loop through each archetype and check if the archetype has the requested components
  for (auto& [archetype, archetype_storage] : ARCHETYPE_INDEX)
  {
    // find the type in the archetype
    bool has_requested_components = (
      (std::find(
        archetype_storage.type.begin(),
        archetype_storage.type.end(),
        Reflection::TypeResolver<Ts>::Get()->name
      ) != archetype_storage.type.end()) && ...
    );

    // check if the archetype has the requested components
    if (has_requested_components)
    {
      // 2. Get the entities from the archetype
      entities.insert(entities.end(), archetype_storage.entities.begin(), archetype_storage.entities.end());
    }
  }

  return entities;
}

#include "timer.h"
/*
  \brief Performs a query that returns a pre-cached list of entities that have the requested components.
         If the query has not been performed before, it will perform the query and cache the result.

  \para Now you might say, why cache the query result? Well, the reason is that the query result is not going to change
        unless you add or remove components from the entities. So, if you are going to perform the same query multiple times,
        it is better to cache the result and return the cached result instead of performing the query multiple times.

        We could always create a archetype map that fully covers every connection, but computing that would literally run 10^15 times just for like 20+ components
        Clearly that isn't viable. So following the principles of pick 2 of the 3 to save - speed, memory and time we choose to save speed and tim
*/
template <typename... Ts>
std::vector<FlexEngine::FlexECS::Entity>& FlexEngine::FlexECS::Scene::CachedQuery()
{
  // Check if the query exists
  std::vector<std::string> component_id_list = { Reflection::TypeResolver<Ts>::Get()->name... };
  std::sort(component_id_list.begin(), component_id_list.end());

 /* Log time to execute query...
  {
    FLX_SCOPED_TIMER("SORTING");
    std::sort(component_id_list.begin(), component_id_list.end());
  }

  {
    FLX_SCOPED_TIMER("FOR LOOP");
    if (query_cache.find(component_id_list) != query_cache.end())
    {
      return query_cache[component_id_list];
    }
  }*/
  
  if (query_cache.find(component_id_list) != query_cache.end())
  {
    return query_cache[component_id_list];
  }

  // If no such query exists, perform normal query and cache the result
  Log::Debug("CachedQuery was called, but no cache was available");
  query_cache[component_id_list] = Query<Ts...>();

  return query_cache[component_id_list];
}
