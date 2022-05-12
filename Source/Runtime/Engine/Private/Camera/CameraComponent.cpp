#include "PCH.hpp"

#include "Camera/CameraComponent.hpp"

// ==============================[Public Constructor + Destructor]============================== //

CameraComponent::CameraComponent    ()
{
    m_aspectRatio               = 1.5f;
    m_constraintAspectRatio     = false;
    m_farClipPlane              = 100.0f;
    m_fieldOfView               = 60.0f;
    m_nearClipPlane             = 0.1f;
    m_orthoWidth                = 1.5f;
    m_projectionMode            = ECameraProjectionMode::Perspective;
    m_usePawnControlRotation    = false;
}

// ==============================[Virtual Public Local Methods]============================== //

CameraViewInfo  CameraComponent::GetCameraView  ()
{
    CameraViewInfo  cameraViewInfo;

    if (m_useAdditiveOffset)
    {
        Transform const offsetCameraToWorld {GetWorldTransform() * m_additiveOffset};

        cameraViewInfo.m_location = offsetCameraToWorld.m_translation;
        cameraViewInfo.m_rotation = offsetCameraToWorld.m_rotation;
    }
    
    else
    {
        cameraViewInfo.m_location = GetWorldTransform().m_translation;
        cameraViewInfo.m_rotation = GetWorldTransform().m_rotation;
    }

    cameraViewInfo.m_fieldOfView            = m_fieldOfView;
    cameraViewInfo.m_orthoWidth             = m_orthoWidth;
    cameraViewInfo.m_nearClipPlane          = m_nearClipPlane;
    cameraViewInfo.m_farClipPlane           = m_farClipPlane;
    cameraViewInfo.m_aspectRatio            = m_aspectRatio;
    cameraViewInfo.m_constrainAspectRatio   = m_constraintAspectRatio;
    cameraViewInfo.m_projectionMode         = m_projectionMode;
    
    return cameraViewInfo;
}

// ==============================[Public Local Methods]============================== //

void    CameraComponent::AddAdditiveOffset      (Transform const&   p_newAdditiveOffset,
                                                 float const        p_newAdditiveFOVOffset)
{
    m_useAdditiveOffset =  true;
    m_additiveOffset    =  p_newAdditiveOffset * m_additiveOffset;
    m_additiveFOVOffset += p_newAdditiveFOVOffset;
}

void    CameraComponent::ClearAdditiveOffset()
{
    m_useAdditiveOffset = false;
    m_additiveOffset    = Transform::Identity;
    m_additiveFOVOffset = 0.0f;
}