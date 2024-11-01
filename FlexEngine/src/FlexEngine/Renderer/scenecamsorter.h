#pragma once
//its a me weijie
#include <FlexEngine.h>
#include "OpenGL/openglcamera.h"

namespace FlexEngine
{
    class __FLX_API SceneCamSorter
    {
        static OpenGLCamera* mainCam;   /*!< Pointer to the main camera */
        static OpenGLCamera* editorCam; /*!< Pointer to the editor camera */
    public:
        #pragma region Set functions
        /*!************************************************************************
        * \brief Sets the main camera.
        * \param mainCamtoSet The camera to set as the main camera.
        *************************************************************************/
        static void SetMainCamera(OpenGLCamera& mainCamtoSet); 

        /*!************************************************************************
        * \brief Sets the editor camera.
        * \param editorCamtoSet The camera to set as the editor camera.
        *************************************************************************/
        static void SetEditorCamera(OpenGLCamera& editorCamtoSet);
        #pragma endregion

        #pragma region Get functions

        /*!************************************************************************
        * \brief Retrieves the main camera.
        * \return Pointer to the main camera.
        *************************************************************************/
        static OpenGLCamera* GetMainCamera();

        /*!************************************************************************
        * \brief Retrieves the editor camera.
        * \return Pointer to the editor camera.
        *************************************************************************/
        static OpenGLCamera* GetEditorCamera();

        /*!************************************************************************
        * \brief Checks if the main camera is currently active.
        * \return True if the main camera is active; false otherwise.
        *************************************************************************/
        static bool IsMainCameraActive();

        /*!************************************************************************
        * \brief Checks if the editor camera is currently active.
        * \return True if the editor camera is active; false otherwise.
        *************************************************************************/
        static bool IsEditorCameraActive();
        #pragma endregion
    };
}


