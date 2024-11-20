#include "camera2d.h"

namespace FlexEngine 
{
    #pragma region Set Functions

    void Camera2D::SetDirection(CameraData& curr, const Vector3& target)
    {
        curr.target = target;
        curr.right = Cross(curr.up, target).Normalize();
        curr.viewMatrix = Matrix4x4::LookAt(curr.position, curr.position + curr.target, curr.up);
    }

    void Camera2D::SetFieldOfView(CameraData& curr, float fov)
    {
        curr.fieldOfView = fov;
        curr.projMatrix = Matrix4x4::Perspective(fov, curr.aspectRatio, curr.nearClip, curr.farClip);
    }

    void Camera2D::SetAspectRatio(CameraData& curr, float aspect)
    {
        curr.aspectRatio = aspect;
        curr.projMatrix = Matrix4x4::Perspective(curr.fieldOfView, aspect, curr.nearClip, curr.farClip);
    }

    void Camera2D::SetClippingPlanes(CameraData& curr, float near, float far)
    {
        curr.nearClip = near;
        curr.farClip = far;
        curr.projMatrix = Matrix4x4::Perspective(curr.fieldOfView, curr.aspectRatio, near, far);
    }

    #pragma endregion

    #pragma region Camera Management

    void Camera2D::UpdateViewMatrix(CameraData& curr)
    {
        curr.viewMatrix = Matrix4x4::LookAt(Vector3::Zero, Vector3::Forward, Vector3::Up/*curr.position, curr.position+ curr.direction, curr.up*/);
        curr.proj_viewMatrix = curr.projMatrix * curr.viewMatrix;
    }

    void Camera2D::UpdateProjectionMatrix(CameraData& curr)
    {
        if (curr.m_isOrthographic)
        {
            curr.projMatrix = Matrix4x4::Orthographic(
                curr.position.x, curr.position.x + FlexEngine::Application::GetCurrentWindow()->GetWidth(),
                curr.position.y + FlexEngine::Application::GetCurrentWindow()->GetHeight(), curr.position.y,
                curr.nearClip, curr.farClip);
        }
        else
        {
            curr.projMatrix = Matrix4x4::Perspective(curr.fieldOfView, curr.aspectRatio, curr.nearClip, curr.farClip);
        }
        curr.proj_viewMatrix = curr.projMatrix * curr.viewMatrix;
    }

    // Converts a world space position to a screen space position
    //Vector3 Camera2D::WorldToScreen(const CameraData& curr, const Vector2& screenDimensions)
    //{
    //    // Combine view and projection matrices
    //    Matrix4x4 viewProjection = curr.proj_viewMatrix;

    //    // Transform the world position to clip space
    //    Vector4 clipSpacePos = viewProjection * Vector4(curr.position, 1.0f);

    //    // Perform perspective divide to normalize coordinates
    //    if (clipSpacePos.w != 0.0f)
    //    {
    //        clipSpacePos.x /= clipSpacePos.w;
    //        clipSpacePos.y /= clipSpacePos.w;
    //    }

    //    // Convert from normalized device coordinates (NDC) to screen coordinates
    //    Vector2 screenPos;
    //    screenPos.x = (clipSpacePos.x * 0.5f + 0.5f) * screenDimensions.x;
    //    screenPos.y = (1.0f - (clipSpacePos.y * 0.5f + 0.5f)) * screenDimensions.y;

    //    return screenPos;
    //}

    //// Converts a screen space position to a world space position
    //Vector3 Camera2D::ScreenToWorld(const Vector2& screenPosition, const Vector2& screenDimensions)
    //{
    //    // Convert screen position to normalized device coordinates (NDC)
    //    Vector4 ndcPos;
    //    ndcPos.x = (2.0f * screenPosition.x) / screenDimensions.x - 1.0f;
    //    ndcPos.y = 1.0f - (2.0f * screenPosition.y) / screenDimensions.y;
    //    ndcPos.z = 1.0f; // Assuming we're dealing with a 2D plane
    //    ndcPos.w = 1.0f;

    //    // Combine view and projection matrices
    //    Matrix4x4 viewProjection = m_projectionMatrix * m_viewMatrix;
    //    Matrix4x4 inverseViewProjection = viewProjection.Inverse();

    //    // Transform the NDC position to world space
    //    Vector4 worldPos = inverseViewProjection * ndcPos;

    //    // Perform perspective divide
    //    if (worldPos.w != 0.0f)
    //    {
    //        worldPos.x /= worldPos.w;
    //        worldPos.y /= worldPos.w;
    //        worldPos.z /= worldPos.w;
    //    }

    //    return Vector3(worldPos.x, worldPos.y, worldPos.z);
    //}
    #pragma endregion

    #pragma region Camera Movement
    
    void Camera2D::Move(CameraData& curr, const Vector2& movement)
    {
        curr.position += curr.right * movement.x + curr.up * movement.y;
        UpdateViewMatrix(curr);
    }

    void Camera2D::MoveTo(CameraData& curr, const Vector3& targetPosition)
    {
        curr.position = targetPosition;
        UpdateViewMatrix(curr);
    }

    void Camera2D::LerpTo(CameraData& curr, const Vector3& targetPosition, float t)
    {
        curr.position = Lerp(curr.position, targetPosition, t);
        UpdateViewMatrix(curr);
    }

    //void Camera2D::Cam_Rotate(CameraData& curr, const Vector3& axis, float angle)
    //{
    //    Matrix4x4 rotationMatrix = Matrix4x4::Rotate(angle, axis);
    //    curr.direction = (rotationMatrix * curr.direction).Normalize();
    //    curr.right = (rotationMatrix * curr.right).Normalize();
    //    curr.up = Cross(curr.direction, curr.right).Normalize();
    //    UpdateViewMatrix(curr);
    //}

    #pragma endregion

    #pragma region Camera Transitions

    void Camera2D::LookAt(CameraData& curr, const Vector3& target)
    {
        curr.target = target;
        curr.right = Cross(curr.up, curr.position - curr.target).Normalize();
        curr.viewMatrix = Matrix4x4::LookAt(curr.position, target, curr.up);
    }

    #pragma endregion
}