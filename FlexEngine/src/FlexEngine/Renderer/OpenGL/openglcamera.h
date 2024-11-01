#pragma once

#include "flx_api.h"

#include "FlexMath/matrix4x4.h"


namespace FlexEngine 
{
   
    /*!************************************************************************
* \struct CameraData
* \brief
* Holds core camera properties, including position, orientation,
* and transformation matrices.
*************************************************************************/
    struct CameraData
    {
        Vector3 position;        /*!< Camera position in world space */
        Vector3 direction;       /*!< Direction the camera is facing */
        Vector3 up;              /*!< Up vector for the camera's orientation */
        Vector3 right;           /*!< Right vector for the camera's orientation */
        Matrix4x4 viewMatrix;    /*!< View matrix calculated from position and orientation */
        Matrix4x4 projMatrix;    /*!< Projection matrix based on FOV and aspect ratio */
        float fieldOfView = 45.0f; /*!< Field of view in degrees */
        float aspectRatio = 1.77f; /*!< Aspect ratio (width/height) */
        float nearClip = 0.1f;   /*!< Near clipping plane */
        float farClip = 100.0f;  /*!< Far clipping plane */
    };

    class __FLX_API OpenGLCamera
    {
        CameraData m_cameraData; /*!< Stores camera's data and settings */
        bool m_isOrthographic = false; /*!< Flag indicating orthographic (true) or perspective (false) projection */
    public:
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

        #pragma region Get functions
        /*!************************************************************************
        * \brief
        * Retrieves the camera's position.
        * \return Camera's position vector.
        *************************************************************************/
        const Vector3& GetPosition() const;

        /*!************************************************************************
        * \brief
        * Retrieves the view matrix of the camera.
        * \return Camera's view matrix.
        *************************************************************************/
        const Matrix4x4& GetViewMatrix() const;

        /*!************************************************************************
        * \brief
        * Retrieves the projection matrix of the camera.
        * \return Camera's projection matrix.
        *************************************************************************/
        const Matrix4x4& GetProjectionMatrix() const;

        /*!************************************************************************
        * \brief
        * Converts a world-space position to screen-space coordinates.
        * \param worldPos World-space position.
        * \return 2D screen-space position.
        *************************************************************************/
        Vector2 WorldToScreen(const Vector3& worldPos) const;

        /*!************************************************************************
        * \brief
        * Checks if a point is within the camera's view frustum.
        * \param point Point in world space.
        * \return True if the point is within view, false otherwise.
        *************************************************************************/
        bool IsInView(const Vector3& point) const;
        #pragma endregion

        #pragma region Cam Management

        /*!************************************************************************
        * \brief
        * Initializes the camera with either an orthographic or perspective projection.
        * \param ortho True to create an orthographic camera, false for perspective.
        *************************************************************************/
        void CreateCamera(bool ortho);
        /*!************************************************************************
        * \brief
        * Rotates the camera around a specified axis by an angle in degrees.
        * \param axis Axis to rotate around.
        * \param angle Angle in degrees.
        *************************************************************************/
        void Rotate(const Vector3& axis, float angle);

        /*!************************************************************************
        * \brief
        * Points the camera at a target position.
        * \param target Target position to look at.
        *************************************************************************/
        void LookAt(const Vector3& target);

        /*!************************************************************************
        * \brief
        * Moves the camera in world space based on input movement direction.
        * \param movement Movement vector where x corresponds to right/left movement,
        *                 and y corresponds to up/down movement.
        *************************************************************************/
        void Move(const Vector2& movement);

        /*!************************************************************************
        * \brief
        * Updates the view matrix based on current position, direction, and up vector.
        *************************************************************************/
        void UpdateViewMatrix();

        /*!************************************************************************
        * \brief
        * Updates the projection matrix based on current FOV, aspect ratio, near, and far planes.
        *************************************************************************/
        void UpdateProjectionMatrix();

        /*!************************************************************************
        * \brief
        * Toggles between perspective and orthographic projection.
        *************************************************************************/
        void ToggleProjection();
        #pragma endregion
    };
}