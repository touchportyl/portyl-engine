/**************************************************************************
 * FLEXENGINE [https://flexengine.web.app]
 * SceneCamSorter.cpp
 *
 * This file implements the `SceneCamSorter` class, which is responsible
 * for managing camera entities in the game engine. The class provides
 * functionalities to add, remove, and switch between main and editor cameras,
 * and manages core camera properties through the `CameraData` structure.
 *
 * Key functionalities include:
 * - Adding and removing camera entities with associated data.
 * - Switching the main and editor cameras, with validation.
 * - Providing access to camera data based on entity ID.
 * - Enforcing a singleton design pattern to ensure consistent access.
 *
 * AUTHORS:
 * - [100%] Soh Wei Jie (weijie.soh@digipen.edu)
 *   - Main Author
 *   - Developed core camera management functionalities, singleton handling,
 *     and implemented data structures for storing camera properties.
 *
 * Copyright (c) 2024 DigiPen, All rights reserved.
 **************************************************************************/

#include "scenecamsorter.h"

namespace FlexEngine
{
	#pragma region Reflection
    FLX_REFL_REGISTER_START(CameraData)
        FLX_REFL_REGISTER_PROPERTY(position)
        FLX_REFL_REGISTER_PROPERTY(direction)
        FLX_REFL_REGISTER_PROPERTY(up)
        FLX_REFL_REGISTER_PROPERTY(right)
        FLX_REFL_REGISTER_PROPERTY(viewMatrix)
        FLX_REFL_REGISTER_PROPERTY(projMatrix)
        FLX_REFL_REGISTER_PROPERTY(fieldOfView)
        FLX_REFL_REGISTER_PROPERTY(aspectRatio)
        FLX_REFL_REGISTER_PROPERTY(nearClip)
        FLX_REFL_REGISTER_PROPERTY(farClip)
        FLX_REFL_REGISTER_PROPERTY(m_isOrthographic)
    FLX_REFL_REGISTER_END;
	#pragma endregion

    // Private constructor to enforce singleton pattern
    SceneCamSorter::SceneCamSorter() = default;

    // Static method to access the single instance
    SceneCamSorter& SceneCamSorter::GetInstance()
    {
        static SceneCamSorter instance;
        return instance;
    }

    #pragma region Camera Management
    /*!***************************************************************************
    * \brief
    * Adds an entity with CameraData to the camera sorter.
    *
    * \param entityID The ID of the entity to add.
    * \param cameraData The CameraData associated with the entity.
    * \return True if the camera entity was added successfully, false if it already exists.
    *****************************************************************************/
    bool SceneCamSorter::AddCameraEntity(FlexECS::EntityID entityID, const CameraData& cameraData)
    {
        auto result = m_cameraEntities.emplace(entityID, cameraData);
        if (result.second)
        {
            Log::Debug("AddCameraEntity(...) => Added camera " + std::to_string(entityID));
            return true;
        }
        else
        {
            Log::Fatal("AddCameraEntity(...) => Entity ID already exists. Cannot add camera with entityID " + std::to_string(entityID));
            return false;
        }
    }

    /*!***************************************************************************
    * \brief
    * Changes the main camera to the specified entity ID, if it exists.
    *
    * \param entityID The ID of the entity to set as the main camera.
    * \return True if the main camera was successfully switched, false otherwise.
    *****************************************************************************/
    bool SceneCamSorter::SwitchMainCamera(FlexECS::EntityID entityID)
    {
        if (m_cameraEntities.find(entityID) != m_cameraEntities.end())
        {
            m_currMainID = entityID;
            Log::Debug("SwitchMainCamera(...) => Switched Main to camera " + std::to_string(entityID));
            return true;
        }
        Log::Warning("SwitchMainCamera(...) => Unable to find camera with your entityID. You forgot to add it.");
        return false;
    }

    /*!***************************************************************************
    * \brief
    * Changes the editor camera to the specified entity ID, if it exists.
    *
    * \param entityID The ID of the entity to set as the editor camera.
    * \return True if the editor camera was successfully switched, false otherwise.
    *****************************************************************************/
    bool SceneCamSorter::SwitchEditorCamera(FlexECS::EntityID entityID)
    {
        if (m_cameraEntities.find(entityID) != m_cameraEntities.end())
        {
            m_currEditorID = entityID;
            Log::Debug("SwitchEditorCamera(...) => Switched Editor to camera " + std::to_string(entityID));
            return true;
        }
        Log::Warning("SwitchEditorCamera(...) => Unable to find camera with your entityID. You forgot to add it.");
        return false;
    }

    void SceneCamSorter::RemoveCameraEntities()
    {
      m_cameraEntities.clear();
      m_currMainID = static_cast<FlexECS::EntityID>(- 1);
      m_currEditorID = static_cast<FlexECS::EntityID>(-1);
    }

    /*!***************************************************************************
    * \brief
    * Updates the CameraData for a given entity ID.
    *
    * \param entityID The ID of the entity to update.
    * \param curr The new CameraData to update.
    *****************************************************************************/
    void SceneCamSorter::UpdateData(const FlexECS::EntityID entityID, const CameraData& curr)
    {
        auto it = m_cameraEntities.find(entityID);
        if (it != m_cameraEntities.end())
        {
            it->second = curr;
        }
        else
        {
            Log::Warning("UpdateData(...) => Entity ID not found.");
        }
    }
    #pragma endregion

    #pragma region Get functions
    /*!***************************************************************************
    * \brief
    * Retrieves the main camera entity ID.
    *
    * \return The entity ID of the main camera.
    *****************************************************************************/
    FlexECS::EntityID SceneCamSorter::GetMainCamera() const
    {
        if (m_currMainID == -1) Log::Warning("GetMainCamera() => Main Camera is of an invalid ID.");
        return m_currMainID;
    }

    /*!***************************************************************************
    * \brief
    * Retrieves the editor camera entity ID.
    *
    * \return The entity ID of the editor camera.
    *****************************************************************************/
    FlexECS::EntityID SceneCamSorter::GetEditorCamera() const
    {
        if (m_currEditorID == -1) Log::Warning("GetEditorCamera() => Editor Camera is of an invalid ID.");
        return m_currEditorID;
    }

    /*!***************************************************************************
    * \brief
    * Retrieves the CameraData for a specific entity ID.
    *
    * \param entityID The ID of the entity to retrieve CameraData for.
    * \return Pointer to the CameraData if found, nullptr otherwise.
    *****************************************************************************/
    CameraData* SceneCamSorter::GetCameraData(FlexECS::EntityID entityID)
    {
        auto it = m_cameraEntities.find(entityID);
        if (it != m_cameraEntities.end())
        {
            return &it->second;
        }
        Log::Warning("GetCameraData(...) => Entity ID not found.");
        return nullptr;
    }

    /*!***************************************************************************
    * \brief
    * Checks if an entity has been added as a camera.
    *
    * \param entityID The ID of the entity to check.
    * \return True if the entity is a camera, false otherwise.
    *****************************************************************************/
    bool SceneCamSorter::HasCameraEntity(FlexECS::EntityID entityID) const
    {
        return m_cameraEntities.find(entityID) != m_cameraEntities.end();
    }
    #pragma endregion

}
