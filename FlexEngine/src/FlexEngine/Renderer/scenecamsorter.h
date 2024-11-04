#pragma once
#include <FlexEngine.h>
#include "FlexECS/datastructures.h"
#include <vector>
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
    { FLX_REFL_SERIALIZABLE
        Vector3 position = Vector3::Zero;        /*!< Camera position in world space */
        Vector3 direction = Vector3::Zero;       /*!< Direction the camera is facing */
        Vector3 up = Vector3::Up;              /*!< Up vector for the camera's orientation */
        Vector3 right = Vector3::Right;           /*!< Right vector for the camera's orientation */
        Matrix4x4 viewMatrix = Matrix4x4::Zero;    /*!< View matrix calculated from position and orientation */
        Matrix4x4 projMatrix = Matrix4x4::Zero;    /*!< Projection matrix based on FOV and aspect ratio */
        float fieldOfView = 45.0f; /*!< Field of view in degrees */
        float aspectRatio = 1.77f; /*!< Aspect ratio (width/height) */
        float nearClip = 0.1f;   /*!< Near clipping plane */
        float farClip = 100.0f;  /*!< Far clipping plane */

        bool m_isOrthographic = false; /*!< Flag indicating orthographic (true) or perspective (false) projection */
    };

    class __FLX_API SceneCamSorter
    {
        std::unordered_map<FlexECS::EntityID, CameraData> m_cameraEntities; // Maps entity IDs to CameraData
        FlexECS::EntityID m_currMainID = (uint64_t) -1; // Default to invalid ID
        FlexECS::EntityID m_currEditorID = (uint64_t) -1; // Default to invalid ID

        // Private constructor to enforce singleton pattern
        SceneCamSorter() = default;

    public:
        // Delete copy constructor and assignment operator to prevent copies
        SceneCamSorter(const SceneCamSorter&) = delete;
        SceneCamSorter& operator=(const SceneCamSorter&) = delete;

        // Static method to access the single instance
        static SceneCamSorter& GetInstance()
        {
            static SceneCamSorter instance;
            return instance;
        }

        #pragma region Camera Management
        // Add an entity with CameraData to the sorter
        bool AddCameraEntity(FlexECS::EntityID entityID, const CameraData& cameraData)
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

        // Change the current m_currMainID to the given entity ID if it exists in m_cameraEntities
        bool SwitchMainCamera(FlexECS::EntityID entityID)
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

        // Change the current m_currEditorID to the given entity ID if it exists in m_cameraEntities
        bool SwitchEditorCamera(FlexECS::EntityID entityID)
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

        // Utility to check if an entity has been added as a camera
        bool HasCameraEntity(FlexECS::EntityID entityID) const
        {
            return m_cameraEntities.find(entityID) != m_cameraEntities.end();
        }
        #pragma endregion

        #pragma region Get functions
        // Retrieve the main camera entity ID
        FlexECS::EntityID GetMainCamera() const
        {
            if (m_currMainID == -1) Log::Warning("GetMainCamera() => Main Camera is of an invalid ID.");
            return m_currMainID;
        }

        // Retrieve the editor camera entity ID
        FlexECS::EntityID GetEditorCamera() const
        {
            if (m_currEditorID == -1) Log::Warning("GetEditorCamera() => Editor Camera is of an invalid ID.");
            return m_currEditorID;
        }

        // Retrieve the CameraData for a specific entity ID
        CameraData* GetCameraData(FlexECS::EntityID entityID)
        {
            auto it = m_cameraEntities.find(entityID);
            if (it != m_cameraEntities.end())
            {
                return &it->second;
            }
            Log::Warning("GetCameraData(...) => Entity ID not found.");
            return nullptr;
        }

        void UpdateData(const FlexECS::EntityID entityID, const CameraData& curr)
        {
            auto it = m_cameraEntities.find(entityID);
            if (it != m_cameraEntities.end())
            {
                it->second = curr;
            }
            else
            Log::Warning("UpdateData(...) => Entity ID not found.");
        }
        #pragma endregion
    };
}


