#include "camera2d.h"

namespace FlexEngine 
{
    namespace camera2D
    {
        #pragma region Set functions
        //void SetDirection(const Vector3& direction)
        //{
        //    //m_cameraData.direction = direction;
        //    UpdateViewMatrix(); // Recalculate the view matrix based on the new direction
        //}

        //void SetFieldOfView(float fov)
        //{
        //    //m_cameraData.fieldOfView = fov;
        //    UpdateProjectionMatrix(); // Update projection to reflect the new field of view
        //}

        //void SetAspectRatio(float aspect)
        //{
        //    //m_cameraData.aspectRatio = aspect;
        //    UpdateProjectionMatrix(); // Adjust projection matrix for new aspect ratio
        //}

        //void SetClippingPlanes(float near, float far)
        //{
        //    /*m_cameraData.nearClip = near;
        //    m_cameraData.farClip = far;*/
        //    UpdateProjectionMatrix(); // Update projection matrix with new clipping planes
        //}
        #pragma endregion
        #pragma region Get functions


        //bool IsInView(const Vector3& point)
        //{
        //    //// Transform the point to view space
        //    //Vector4 viewPos = m_cameraData.viewMatrix * Vector4(point, 1.0f);

        //    //// Check if point is within the view frustum
        //    //return (viewPos.z > m_cameraData.nearClip && viewPos.z < m_cameraData.farClip);
        //    return false;
        //}
        //#pragma endregion
        //#pragma region Cam Management
        //void CreateCamera(bool ortho) {
        //    //m_isOrthographic = ortho;
        //    UpdateProjectionMatrix(); // Initialize projection matrix based on camera type
        //}

        //void Rotate(const Vector3& axis, float angle)
        //{
        //    //// Create a rotation matrix from axis and angle
        //    //Matrix4x4 rotationMatrix = Matrix4x4::Rotation(axis, angle);

        //    //// Rotate direction and up vectors
        //    //m_cameraData.direction = rotationMatrix * m_cameraData.direction;
        //    //m_cameraData.up = rotationMatrix * m_cameraData.up;

        //    //UpdateViewMatrix(); // Update view matrix to reflect rotation
        //}

        //void LookAt(const Vector3& target)
        //{
        //    //// Calculate new direction vector towards target
        //    //m_cameraData.direction = (target - m_cameraData.position).Normalized();

        //    //// Recalculate the view matrix with the updated direction
        //    //UpdateViewMatrix();
        //}

        void Move(const Vector2& movement, CameraData& currpos)
        {
            // Translate camera based on movement input (x - right, y - up)
            curr.position += curr.right * movement.x + curr.up * movement.y;

            //UpdateViewMatrix(); // Update view matrix to reflect new position
        }

        //void MoveTo(const Vector3& targetPosition)
        //{
        //    // Set the camera's position to the target position
        //    //m_cameraData.position = targetPosition;

        //    // Update the view matrix to reflect the new position
        //    UpdateViewMatrix();
        //}

        //void UpdateViewMatrix()
        //{
        //    //// Calculate the right vector using cross product of up and direction
        //    //m_cameraData.right = m_cameraData.up.Cross(m_cameraData.direction).Normalized();

        //    //// Recalculate view matrix based on updated camera orientation and position
        //    //m_cameraData.viewMatrix = Matrix4x4::LookAt(m_cameraData.position, m_cameraData.position + m_cameraData.direction, m_cameraData.up);
        //}

        //void UpdateProjectionMatrix()
        //{
        //    //if (m_isOrthographic)
        //    //{
        //    //    // Set orthographic projection matrix (parameters could be adjusted as needed)
        //    //    m_cameraData.projMatrix = Matrix4x4::Orthographic(-10, 10, -10 / m_cameraData.aspectRatio, 10 / m_cameraData.aspectRatio, m_cameraData.nearClip, m_cameraData.farClip);
        //    //}
        //    //else {
        //    //    // Set perspective projection matrix based on FOV, aspect ratio, near, and far planes
        //    //    m_cameraData.projMatrix = Matrix4x4::Perspective(m_cameraData.fieldOfView, m_cameraData.aspectRatio, m_cameraData.nearClip, m_cameraData.farClip);
        //    //}
        //}

        //void ToggleProjection()
        //{
        //    // Toggle between orthographic and perspective projection
        //    //m_isOrthographic = !m_isOrthographic;
        //    UpdateProjectionMatrix(); // Rebuild projection matrix for the new mode
        //}
        #pragma endregion
    }
}