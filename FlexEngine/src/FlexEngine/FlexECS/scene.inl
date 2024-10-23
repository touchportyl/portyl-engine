// inline functions for Scene class

// Steps:
// 1. Loop through each archetype and check if the archetype has the requested components
// 2. Get the entities from the archetype
template <typename... Ts>
std::vector<FlexEngine::FlexECS::Entity> FlexEngine::FlexECS::Scene::View()
{
  // TODO: when param is transform, isactive, doesnt work
  std::vector<Entity> entities;

  // 1. Loop through each archetype and check if the archetype has the requested components
  for (auto& [archetype, archetype_storage] : ARCHETYPE_INDEX)
  {
    bool has_components{ false };

    // Generate a lambda function for each unnamed parameter in the parameter pack
    auto checkComponents = [&archetype_storage, &entities](auto&& param)
    {
      if (param == "Transform" || param == "IsActive")
      {
        return true;
      }

      // If adding this component is possible or removing isn't possible - means it isn't part of it
      if (archetype_storage.edges[param].add != nullptr || archetype_storage.edges[param].remove == nullptr)
      {
        return false;
      }

      return true;
    };

    // 2. Get componentID via type resolver, and pass it to the lambda function. The lambda function will run for each component in the parameter pack
    has_components = (checkComponents(Reflection::TypeResolver<Ts>::Get()->name), ...);

    // 3. Copy all entities from the archetype to the entities vector if it meets the requirements
    if (has_components)
    {
      entities.insert(entities.end(), archetype_storage.entities.begin(), archetype_storage.entities.end());
    }
  }

  return entities;
}
