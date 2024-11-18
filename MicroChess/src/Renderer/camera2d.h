#pragma once

#include "flx_api.h"

#include "FlexMath/matrix4x4.h"
#include <Renderer/scenecamsorter.h>


namespace FlexEngine
{
    //Still debating on this (class is still in development)
    namespace camera2D
    {
        #pragma region Set functions
        /*!************************************************************************
        * \brief
        * Sets the camera's direction vector.
        * \param direction New direction vector.
        *************************************************************************/
        void SetDirection(const Vector3& direction);

        /*!************************************************************************
        * \brief
        * Sets the camera's field of view for perspective projection.
        * \param fov Vertical field of view in degrees.
        *************************************************************************/
        void SetFieldOfView(float fov);

        /*!************************************************************************
        * \brief
        * Sets the aspect ratio of the camera.
        * \param aspect Aspect ratio (width/height).
        *************************************************************************/
        void SetAspectRatio(float aspect);

        /*!************************************************************************
        * \brief
        * Sets the near and far clipping planes.
        * \param near Near clipping distance.
        * \param far Far clipping distance.
        *************************************************************************/
        void SetClippingPlanes(float near, float far);

        #pragma endregion

        #pragma region Cam Management

        ///*!************************************************************************
        //* \brief
        //* Initializes the camera with either an orthographic or perspective projection.
        //* \param ortho True to create an orthographic camera, false for perspective.
        //*************************************************************************/
        //void CreateCamera(bool ortho);
        ///*!************************************************************************
        //* \brief
        //* Rotates the camera around a specified axis by an angle in degrees.
        //* \param axis Axis to rotate around.
        //* \param angle Angle in degrees.
        //*************************************************************************/
        //void Rotate(const Vector3& axis, float angle);

        ///*!************************************************************************
        //* \brief
        //* Points the camera at a target position.
        //* \param target Target position to look at.
        //*************************************************************************/
        //void LookAt(const Vector3& target);

        /*!************************************************************************
        * \brief
        * Moves the camera in world space based on input movement direction.
        * \param movement Movement vector where x corresponds to right/left movement,
        *                 and y corresponds to up/down movement.
        *************************************************************************/
        void Move(const Vector2& movement, CameraData& curr);

        ///*!************************************************************************
        //* \brief
        //* Moves the camera in world space to argument position.
        //*************************************************************************/
        //void MoveTo(const Vector3& targetPosition);

        ///*!************************************************************************
        //* \brief
        //* Updates the view matrix based on current position, direction, and up vector.
        //*************************************************************************/
        //void UpdateViewMatrix();

        ///*!************************************************************************
        //* \brief
        //* Updates the projection matrix based on current FOV, aspect ratio, near, and far planes.
        //*************************************************************************/
        //void UpdateProjectionMatrix();

        ///*!************************************************************************
        //* \brief
        //* Toggles between perspective and orthographic projection.
        //*************************************************************************/
        //void ToggleProjection();
        #pragma endregion
    }
}