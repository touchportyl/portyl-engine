#include "scenecamsorter.h"

namespace FlexEngine
{
    #pragma region Static variables
    // Initialize static members
    OpenGLCamera* SceneCamSorter::mainCam = nullptr;
    OpenGLCamera* SceneCamSorter::editorCam = nullptr;
    #pragma endregion

    #pragma region Set functions
    /*!************************************************************************
    * \brief Sets the main camera.
    * \param mainCamtoSet The camera to set as the main camera.
    *************************************************************************/
    void SceneCamSorter::SetMainCamera(OpenGLCamera& mainCamtoSet)
    {
        mainCam = &mainCamtoSet;
    }

    /*!************************************************************************
    * \brief Sets the editor camera.
    * \param editorCamtoSet The camera to set as the editor camera.
    *************************************************************************/
    void SceneCamSorter::SetEditorCamera(OpenGLCamera& editorCamtoSet)
    {
        editorCam = &editorCamtoSet;
    }

    #pragma endregion

    #pragma region Get functions
    /*!************************************************************************
    * \brief Retrieves the main camera.
    * \return Pointer to the main camera.
    *************************************************************************/
    OpenGLCamera* SceneCamSorter::GetMainCamera()
    {
        return mainCam;
    }

    /*!************************************************************************
    * \brief Retrieves the editor camera.
    * \return Pointer to the editor camera.
    *************************************************************************/
    OpenGLCamera* SceneCamSorter::GetEditorCamera()
    {
        return editorCam;
    }

    /*!************************************************************************
    * \brief Checks if the main camera is currently active.
    * \return True if the main camera is active; false otherwise.
    *************************************************************************/
    bool SceneCamSorter::IsMainCameraActive()
    {
        return mainCam != nullptr;
    }

    /*!************************************************************************
    * \brief Checks if the editor camera is currently active.
    * \return True if the editor camera is active; false otherwise.
    *************************************************************************/
    bool SceneCamSorter::IsEditorCameraActive()
    {
        return editorCam != nullptr;
    }
    #pragma endregion
}