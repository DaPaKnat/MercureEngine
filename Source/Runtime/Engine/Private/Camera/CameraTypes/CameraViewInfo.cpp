#include "PCH.hpp"

#include "Camera/CameraTypes/CameraViewInfo.hpp"

Matrix4x4   CameraViewInfo::ComputeProjectionMatrix ()  const
{
    if (m_projectionMode == ECameraProjectionMode::Orthographic)
    {
        return Matrix4x4::Ortho
        (
            -m_orthoWidth,
            m_orthoWidth,
            -m_orthoWidth,
            m_orthoWidth,
            m_nearClipPlane,
            m_farClipPlane
        );
    }

    else // (m_projectionMode == ECameraProjectionMode::Perspective)
    {
        return Matrix4x4::Perspective
        (
            Math::Max(0.001f, Math::DegToRad(m_fieldOfView)),
            m_aspectRatio,
            m_nearClipPlane,
            m_farClipPlane
        );
    }
}
