/*
* \author Yew Chong
  \brief This file contains the function to spawn an entity from a prefab file.
         This function only works within the context of the project, and the using namespace needs to be edited for it to work in other projects.

         Usage: Include Prefab.h in the source file where you want to spawn entities from prefab files.
                EntityID e = SpawnEntityFromPrefab("test") where test is the name of the prefab

                From there, you can use the entityID handle to manipulate the entity as you wish.

         Note: The prefab creation function is available from the Scene class and can be invoked as follows:
                FlexECS::Scene::GetActiveScene()->SaveEntityAsPrefab(object.Get() , "test");
*/
#pragma once
#include <FlexEngine.h> // Utility functions
#include "Components/battlecomponents.h"
#include "Components/rendering.h"

/*!
  \brief Spawns an entity with configurations from a prefab file.
  \param prefabName Name of the prefab file.
*/
FlexECS::Entity& LoadCharacter(const std::string& prefabName);
void SaveCharacters();
void ResetCharacters();