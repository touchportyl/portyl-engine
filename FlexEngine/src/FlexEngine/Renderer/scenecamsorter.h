/*!************************************************************************
 // WLVERSE [https://wlverse.web.app]
 // scenecamsorter.h
 //
 // This header file defines the `SceneCamSorter` class, a singleton utility
 // within the FlexEngine framework for managing camera entities in a 3D scene.
 // The `SceneCamSorter` class provides functionalities for registering and
 // tracking active camera entities, facilitating seamless transitions between
 // main and editor cameras. It also offers methods to retrieve and update
 // `CameraData` properties, which encapsulate essential camera attributes
 // such as position, orientation, and projection matrices.
 //
 // Key functionalities include:
 // - Adding, switching, and removing camera entities in the scene.
 // - Providing fast access to `CameraData`, including view and projection
 //   matrices, for any registered camera.
 // - Ensuring only one instance manages camera sorting, adhering to the
 //   singleton design pattern.
 //
 // The `CameraData` struct, also defined here, includes properties for world
 // position, orientation vectors, and other parameters essential for view
 // transformations in both perspective and orthographic projections.
 //
 // AUTHORS
 // [100%] Soh Wei Jie (weijie.soh@digipen.edu)
 //   - Main Author
 //   - Developed the camera management interface, singleton pattern, and
 //     entity registration features within FlexEngine.
 //
 // Copyright (c) 2024 DigiPen, All rights reserved.
 **************************************************************************/
#pragma once
#include <FlexEngine.h>
#include "FlexECS/datastructures.h"
#include <unordered_map>
//Will later shift the code to cpp
namespace FlexEngine
{
    /*!************************************************************************
   * \struct CameraData
   * \brief
   * Holds core camera properties, including position, orientation,
   * and transformation matrices.
   *************************************************************************/
    struct __FLX_API CameraData
    {
        FLX_REFL_SERIALIZABLE
            Vector3 position = Vector3::Zero;        /*!< Camera position in world space */
        Vector3 direction = Vector3::Zero;       /*!< Direction the camera is facing */
        Vector3 up = Vector3::Up;                /*!< Up vector for the camera's orientation */
        Vector3 right = Vector3::Right;          /*!< Right vector for the camera's orientation */
        Matrix4x4 viewMatrix = Matrix4x4::Zero;  /*!< View matrix calculated from position and orientation */
        Matrix4x4 projMatrix = Matrix4x4::Zero;  /*!< Projection matrix based on FOV and aspect ratio */
        float fieldOfView = 45.0f;               /*!< Field of view in degrees */
        float aspectRatio = 1.77f;               /*!< Aspect ratio (width/height) */
        float nearClip = 0.1f;                   /*!< Near clipping plane */
        float farClip = 100.0f;                  /*!< Far clipping plane */
        bool m_isOrthographic = false;           /*!< Flag indicating orthographic (true) or perspective (false) projection */
    };

    class __FLX_API SceneCamSorter
    {
    private:
        std::unordered_map<FlexECS::EntityID, CameraData> m_cameraEntities; /*!< Maps entity IDs to CameraData */
        FlexECS::EntityID m_currMainID = static_cast<uint64_t>(-1);         /*!< Current main camera entity ID */
        FlexECS::EntityID m_currEditorID = static_cast<uint64_t>(-1);       /*!< Current editor camera entity ID */

        // Private constructor to enforce singleton pattern
        SceneCamSorter();

    public:
        // Delete copy constructor and assignment operator to prevent copies
        SceneCamSorter(const SceneCamSorter&) = delete;
        SceneCamSorter& operator=(const SceneCamSorter&) = delete;

        // Static method to access the single instance
        static SceneCamSorter& GetInstance();

        #pragma region Camera Management
        /*!***************************************************************************
         * \brief
         * Adds an entity with CameraData to the camera sorter.
         *
         * \param entityID The ID of the entity to add.
         * \param cameraData The CameraData associated with the entity.
         * \return True if the camera entity was added successfully, false if it already exists.
         *****************************************************************************/
        bool AddCameraEntity(FlexECS::EntityID entityID, const CameraData& cameraData);

        /*!***************************************************************************
         * \brief
         * Changes the main camera to the specified entity ID, if it exists.
         *
         * \param entityID The ID of the entity to set as the main camera.
         * \return True if the main camera was successfully switched, false otherwise.
         *****************************************************************************/
        bool SwitchMainCamera(FlexECS::EntityID entityID);

        /*!***************************************************************************
         * \brief
         * Changes the editor camera to the specified entity ID, if it exists.
         *
         * \param entityID The ID of the entity to set as the editor camera.
         * \return True if the editor camera was successfully switched, false otherwise.
         *****************************************************************************/
        bool SwitchEditorCamera(FlexECS::EntityID entityID);

        void RemoveCameraEntities();

        /*!***************************************************************************
         * \brief
         * Checks if an entity has been added as a camera.
         *
         * \param entityID The ID of the entity to check.
         * \return True if the entity is a camera, false otherwise.
         *****************************************************************************/
        bool HasCameraEntity(FlexECS::EntityID entityID) const;
        #pragma endregion

        #pragma region Get functions
        /*!***************************************************************************
         * \brief
         * Retrieves the main camera entity ID.
         *
         * \return The entity ID of the main camera.
         *****************************************************************************/
        FlexECS::EntityID GetMainCamera() const;

        /*!***************************************************************************
         * \brief
         * Retrieves the editor camera entity ID.
         *
         * \return The entity ID of the editor camera.
         *****************************************************************************/
        FlexECS::EntityID GetEditorCamera() const;

        /*!***************************************************************************
         * \brief
         * Retrieves the CameraData for a specific entity ID.
         *
         * \param entityID The ID of the entity to retrieve CameraData for.
         * \return Pointer to the CameraData if found, nullptr otherwise.
         *****************************************************************************/
        CameraData* GetCameraData(FlexECS::EntityID entityID);

        /*!***************************************************************************
         * \brief
         * Updates the CameraData for a given entity ID.
         *
         * \param entityID The ID of the entity to update.
         * \param curr The new CameraData to update.
         *****************************************************************************/
        void UpdateData(const FlexECS::EntityID entityID, const CameraData& curr);
        #pragma endregion
    };
}


