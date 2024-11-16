#include "camera2d.h"

namespace FlexEngine 
{
    #pragma region Set Functions

    void Camera2D::SetDirection(CameraData& curr, const Vector3& direction)
    {
        curr.direction = direction;
        curr.right = Cross(curr.up, direction).Normalize();
        curr.viewMatrix = Matrix4x4::LookAt(curr.position, curr.position + curr.direction, curr.up);
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
        curr.viewMatrix = Matrix4x4::LookAt(curr.position, curr.position + curr.direction, curr.up);
    }

    void Camera2D::UpdateProjectionMatrix(CameraData& curr)
    {
        if (curr.m_isOrthographic)
        {
            curr.projMatrix = Matrix4x4::Orthographic(-1.0f, 1.0f, -1.0f, 1.0f, curr.nearClip, curr.farClip);
        }
        else
        {
            curr.projMatrix = Matrix4x4::Perspective(curr.fieldOfView, curr.aspectRatio, curr.nearClip, curr.farClip);
        }
    }

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


    //void Camera2D::Shake(CameraData& curr, float amplitude, float frequency, float duration)
    //{
    //    using namespace std::chrono;
    //    auto startTime = high_resolution_clock::now();

    //    while (true)
    //    {
    //        auto elapsed = duration_cast<duration<float>>(high_resolution_clock::now() - startTime).count();
    //        if (elapsed > duration) break;

    //        Vector3 offset = GenerateRandomOffset(amplitude);
    //        curr.position += offset;
    //        UpdateViewMatrix(curr);

    //        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1000.0f / frequency)));
    //        curr.position -= offset;
    //        UpdateViewMatrix(curr);
    //    }
    //}

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

    void Camera2D::CreateCamera(CameraData& curr, bool ortho)
    {
        curr.m_isOrthographic = ortho;
        if (ortho)
        {
            curr.projMatrix = Matrix4x4::Orthographic(-1.0f, 1.0f, -1.0f, 1.0f, curr.nearClip, curr.farClip);
        }
        else
        {
            curr.projMatrix = Matrix4x4::Perspective(curr.fieldOfView, curr.aspectRatio, curr.nearClip, curr.farClip);
        }
    }

    void Camera2D::LookAt(CameraData& curr, const Vector3& target)
    {
        curr.direction = (target - curr.position).Normalize();
        curr.right = Cross(curr.up, curr.direction).Normalize();
        curr.viewMatrix = Matrix4x4::LookAt(curr.position, target, curr.up);
    }

    void Camera2D::ToggleProjection(CameraData& curr)
    {
        curr.m_isOrthographic = !curr.m_isOrthographic;
        if (curr.m_isOrthographic)
        {
            curr.projMatrix = Matrix4x4::Orthographic(-1.0f, 1.0f, -1.0f, 1.0f, curr.nearClip, curr.farClip);
        }
        else
        {
            curr.projMatrix = Matrix4x4::Perspective(curr.fieldOfView, curr.aspectRatio, curr.nearClip, curr.farClip);
        }
    }

    #pragma endregion
}