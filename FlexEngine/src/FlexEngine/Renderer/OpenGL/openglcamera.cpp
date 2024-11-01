#include "OpenGLCamera.h"

namespace FlexEngine 
{
    #pragma region Set functions
    void OpenGLCamera::SetDirection(const Vector3& direction) 
    {
        m_cameraData.direction = direction;
        UpdateViewMatrix(); // Recalculate the view matrix based on the new direction
    }

    void OpenGLCamera::SetFieldOfView(float fov) 
    {
        m_cameraData.fieldOfView = fov;
        UpdateProjectionMatrix(); // Update projection to reflect the new field of view
    }

    void OpenGLCamera::SetAspectRatio(float aspect) 
    {
        m_cameraData.aspectRatio = aspect;
        UpdateProjectionMatrix(); // Adjust projection matrix for new aspect ratio
    }

    void OpenGLCamera::SetClippingPlanes(float near, float far) 
    {
        m_cameraData.nearClip = near;
        m_cameraData.farClip = far;
        UpdateProjectionMatrix(); // Update projection matrix with new clipping planes
    }
    #pragma endregion
    #pragma region Get functions
    const Vector3& OpenGLCamera::GetPosition() const 
    {
        return m_cameraData.position;
    }

    const Matrix4x4& OpenGLCamera::GetViewMatrix() const 
    {
        return m_cameraData.viewMatrix;
    }

    const Matrix4x4& OpenGLCamera::GetProjectionMatrix() const 
    {
        return m_cameraData.projMatrix;
    }

    Vector2 OpenGLCamera::WorldToScreen(const Vector3& worldPos) const 
    {
        // Transform world position to view space
        Vector4 viewPos = m_cameraData.viewMatrix * Vector4(worldPos, 1.0f);

        // Transform view position to projection space
        Vector4 projPos = m_cameraData.projMatrix * viewPos;

        // Normalize to screen space (-1 to 1 range)
        Vector2 screenPos(projPos.x / projPos.w, projPos.y / projPos.w);

        // Convert from NDC (-1 to 1) to screen space (0 to 1)
        return (screenPos + Vector2(1.0f, 1.0f)) * 0.5f;
    }

    bool OpenGLCamera::IsInView(const Vector3& point) const 
    {
        // Transform the point to view space
        Vector4 viewPos = m_cameraData.viewMatrix * Vector4(point, 1.0f);

        // Check if point is within the view frustum
        return (viewPos.z > m_cameraData.nearClip && viewPos.z < m_cameraData.farClip);
    }
    #pragma endregion
    #pragma region Cam Management
    void OpenGLCamera::CreateCamera(bool ortho) {
        m_isOrthographic = ortho;
        UpdateProjectionMatrix(); // Initialize projection matrix based on camera type
    }

    void OpenGLCamera::Rotate(const Vector3& axis, float angle) 
    {
        // Create a rotation matrix from axis and angle
        Matrix4x4 rotationMatrix = Matrix4x4::Rotation(axis, angle);

        // Rotate direction and up vectors
        m_cameraData.direction = rotationMatrix * m_cameraData.direction;
        m_cameraData.up = rotationMatrix * m_cameraData.up;

        UpdateViewMatrix(); // Update view matrix to reflect rotation
    }

    void OpenGLCamera::LookAt(const Vector3& target) 
    {
        // Calculate new direction vector towards target
        m_cameraData.direction = (target - m_cameraData.position).Normalized();

        // Recalculate the view matrix with the updated direction
        UpdateViewMatrix();
    }

    void OpenGLCamera::Move(const Vector2& movement) 
    {
        // Translate camera based on movement input (x - right, y - up)
        m_cameraData.position += m_cameraData.right * movement.x + m_cameraData.up * movement.y;

        UpdateViewMatrix(); // Update view matrix to reflect new position
    }

    void OpenGLCamera::UpdateViewMatrix() 
    {
        // Calculate the right vector using cross product of up and direction
        m_cameraData.right = m_cameraData.up.Cross(m_cameraData.direction).Normalized();

        // Recalculate view matrix based on updated camera orientation and position
        m_cameraData.viewMatrix = Matrix4x4::LookAt(m_cameraData.position, m_cameraData.position + m_cameraData.direction, m_cameraData.up);
    }

    void OpenGLCamera::UpdateProjectionMatrix() 
    {
        if (m_isOrthographic) 
        {
            // Set orthographic projection matrix (parameters could be adjusted as needed)
            m_cameraData.projMatrix = Matrix4x4::Orthographic(-10, 10, -10 / m_cameraData.aspectRatio, 10 / m_cameraData.aspectRatio, m_cameraData.nearClip, m_cameraData.farClip);
        }
        else {
            // Set perspective projection matrix based on FOV, aspect ratio, near, and far planes
            m_cameraData.projMatrix = Matrix4x4::Perspective(m_cameraData.fieldOfView, m_cameraData.aspectRatio, m_cameraData.nearClip, m_cameraData.farClip);
        }
    }

    void OpenGLCamera::ToggleProjection() 
    {
        // Toggle between orthographic and perspective projection
        m_isOrthographic = !m_isOrthographic;
        UpdateProjectionMatrix(); // Rebuild projection matrix for the new mode
    }
    #pragma endregion
}