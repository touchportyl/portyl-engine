#include "scenecamsorter.h"

namespace FlexEngine
{
    #pragma region Static variables
    // Initialize static members
    OpenGLCamera* SceneCamSorter::mainCam = nullptr;
    OpenGLCamera* SceneCamSorter::editorCam = nullptr;
    OpenGLCamera* SceneCamSorter::activeCam = nullptr;
    #pragma endregion

    #pragma region Set functions
    /*!************************************************************************
    * \brief Sets the main camera.
    * \param mainCamtoSet The camera to set as the main camera.
    *************************************************************************/
    void SceneCamSorter::SetMainCamera(OpenGLCamera& mainCamtoSet)
    {
        mainCam = &mainCamtoSet;
        if (activeCam == nullptr) activeCam = mainCam; // Set active if none
    }

    /*!************************************************************************
    * \brief Sets the editor camera.
    * \param editorCamtoSet The camera to set as the editor camera.
    *************************************************************************/
    void SceneCamSorter::SetEditorCamera(OpenGLCamera& editorCamtoSet)
    {
        editorCam = &editorCamtoSet;
    }

    /*!************************************************************************
    * \brief Switches the active camera to either main or editor camera.
    * \param useMain If true, sets the main camera as active; otherwise, editor.
    *************************************************************************/
    void SceneCamSorter::SetActiveCamera(bool useMain)
    {
        if (useMain && mainCam)
        {
            activeCam = mainCam;
        }
        else if (editorCam)
        {
            activeCam = editorCam;
        }
    }
    #pragma endregion

    #pragma region Get functions
    /*!************************************************************************
    * \brief Retrieves the current active camera.
    * \return Pointer to the active camera (either main or editor).
    *************************************************************************/
    OpenGLCamera* SceneCamSorter::GetActiveCamera()
    {
        return activeCam;
    }

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
        return activeCam == mainCam;
    }

    /*!************************************************************************
    * \brief Checks if the editor camera is currently active.
    * \return True if the editor camera is active; false otherwise.
    *************************************************************************/
    bool SceneCamSorter::IsEditorCameraActive()
    {
        return activeCam == editorCam;
    }
    #pragma endregion
}